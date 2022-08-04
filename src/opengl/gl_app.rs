use std::{
  fmt::{self, Debug, Formatter},
  ptr,
};

use super::GLStage;
use crate::{funcs::gl_ext::cfg_gl_attr, gl_stage::GLBasicStage};
use iron_ingot::UVec2;

use sdl2::{
  event::Event,
  video::{GLContext, Window},
  Sdl, VideoSubsystem,
};

#[cfg(windows)]
use winapi::um::shellscalingapi::{SetProcessDpiAwareness, PROCESS_PER_MONITOR_DPI_AWARE};

#[cfg(debug_assertions)]
use crate::funcs::gl_ext::on_debug_message;

pub(crate) struct GLApp {
  sdl: Sdl,
  video_subsys: VideoSubsystem,
  window: Window,
  _gl_ctx: GLContext,
  stage: Box<dyn GLStage>,
  is_first_show: bool,
}

impl Debug for GLApp {
  fn fmt(&self, formatter: &mut Formatter<'_>) -> fmt::Result {
    formatter
      .debug_struct("GLApp")
      .field("video_subsys", &self.video_subsys)
      .finish_non_exhaustive()
  }
}

impl GLApp {
  pub(crate) fn new(title: &str, size: UVec2) -> Self {
    debug_assert!(
      size.get_x() >= 1,
      "Width must be greater than or equal to 1!"
    );
    debug_assert!(
      size.get_y() >= 1,
      "Height must be greater than or equal to 1!"
    );

    #[cfg(windows)]
    unsafe {
      SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    }

    let sdl = sdl2::init().unwrap();
    let video_subsys = sdl.video().unwrap();
    cfg_gl_attr(&video_subsys);
    let window = video_subsys
      .window(title, size.get_x(), size.get_y())
      .allow_highdpi()
      .opengl()
      .position_centered()
      .hidden()
      .build()
      .unwrap();
    let gl_ctx = window.gl_create_context().unwrap();
    gl::load_with(|name| video_subsys.gl_get_proc_address(name) as *const _);

    #[cfg(debug_assertions)]
    unsafe {
      gl::DebugMessageCallback(Some(on_debug_message), ptr::null());
      gl::Enable(gl::DEBUG_OUTPUT_SYNCHRONOUS);
    }

    video_subsys.gl_set_swap_interval(0).unwrap();
    let stage: Box<dyn GLStage> = Box::new(GLBasicStage::new(size));
    unsafe {
      gl::ReleaseShaderCompiler();
      gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);
    }
    Self {
      sdl,
      video_subsys,
      window,
      _gl_ctx: gl_ctx,
      stage,
      is_first_show: true,
    }
  }

  pub(crate) fn start(&mut self) {
    self.stage.show(&self.window);
  }
}
