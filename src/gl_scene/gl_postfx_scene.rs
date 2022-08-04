use crate::{
  funcs::gl_ext::{new_gl_program, use_default_gl_framebuffer},
  opengl::GLFramebuffer,
};

use gl::types::*;
use iron_ingot::{FVec2, UVec2};
use std::mem::{size_of, size_of_val};

#[derive(Copy, Clone, Debug)]
pub(crate) struct Arg<'a, F: FnOnce(GLuint)> {
  pub size: UVec2,
  pub frag_shader_code: &'a [u8],
  pub from_tex_unit: GLuint,
  pub to_tex_unit: Option<GLuint>,
  pub init_uniforms: F,
}

#[derive(Debug)]
pub(crate) struct GLPostFXScene {
  vao: GLuint,
  vbo: GLuint,
  fbo: Option<GLFramebuffer>,
  program: GLuint,
}

impl GLPostFXScene {
  pub(crate) fn new(arg: Arg<impl FnOnce(GLuint)>) -> Self {
    let Arg {
      size,
      frag_shader_code,
      from_tex_unit,
      to_tex_unit,
      init_uniforms,
    } = arg;
    let mut this = Self {
      vao: 0,
      vbo: 0,
      fbo: to_tex_unit.map(|to_tex_unit| GLFramebuffer::new(size, to_tex_unit)),
      program: 0,
    };
    this.init(frag_shader_code, from_tex_unit, init_uniforms);
    this
  }

  fn init(
    &mut self,
    frag_shader_code: &[u8],
    from_tex_unit: GLuint,
    init_uniforms: impl FnOnce(GLuint),
  ) {
    self.init_vao();
    self.init_vbo();
    if let Some(fbo) = &mut self.fbo {
      fbo.init();
    }
    self.program = new_gl_program(include_bytes!("../../res/postfx.vert"), frag_shader_code);
    self.set_scale(FVec2::new((1.0, 1.0)));
    self.set_from_tex_unit(from_tex_unit);
    init_uniforms(self.program);
  }

  fn init_vao(&mut self) {
    unsafe {
      gl::CreateVertexArrays(1, &mut self.vao);
      gl::BindVertexArray(self.vao);

      // vPosition
      gl::VertexArrayAttribBinding(self.vao, 0, 0);
      gl::VertexArrayAttribFormat(self.vao, 0, 2, gl::FLOAT, gl::FALSE, 0);
      gl::EnableVertexArrayAttrib(self.vao, 0);

      // vTexCoord
      gl::VertexArrayAttribBinding(self.vao, 1, 0);
      gl::VertexArrayAttribFormat(
        self.vao,
        1,
        2,
        gl::FLOAT,
        gl::FALSE,
        (2 * size_of::<f32>()) as _,
      );
      gl::EnableVertexArrayAttrib(self.vao, 1);

      gl::VertexArrayBindingDivisor(self.vao, 0, 0);
    }
  }

  fn init_vbo(&mut self) {
    unsafe {
      gl::CreateBuffers(1, &mut self.vbo);
      const VERTICES: &[f32] = &[
        // vPosition    vTexCoord
        0.0f32, 0.0f32, 0.0f32, 0.0f32, // top left corner vertex
        0.0f32, -2.0f32, 0.0f32, -1.0f32, // bottom left corner vertex
        2.0f32, -2.0f32, 1.0f32, -1.0f32, // bottom right corner vertex
        2.0f32, 0.0f32, 1.0f32, 0.0f32, // top right corner vertex
      ];
      gl::NamedBufferData(
        self.vbo,
        size_of_val(VERTICES) as _,
        VERTICES as *const _ as *const _,
        gl::STATIC_DRAW,
      );
      gl::VertexArrayVertexBuffer(self.vao, 0, self.vbo, 0, (4 * size_of::<f32>()) as _);
    }
  }

  pub(crate) fn get_tex_id(&self) -> Option<GLuint> {
    self.fbo.as_ref().map(|fbo| fbo.get_tex_id())
  }

  pub(crate) fn set_scale(&self, value: FVec2) {
    unsafe {
      gl::ProgramUniform2f(self.program, 0, value.get_x(), value.get_y());
    }
  }

  pub(crate) fn set_from_tex_unit(&self, value: GLuint) {
    unsafe {
      gl::ProgramUniform1i(self.program, 1, value as _);
    }
  }

  pub(crate) fn show(&self) {
    if let Some(fbo) = &self.fbo {
      fbo.use_();
    } else {
      use_default_gl_framebuffer();
    }
    unsafe {
      gl::UseProgram(self.program);
      gl::BindVertexArray(self.vao);
      gl::DrawArrays(gl::TRIANGLE_FAN, 0, 4);
    }
  }

  pub(crate) fn grow(&mut self) {
    if let Some(fbo) = &mut self.fbo {
      fbo.grow();
    }
  }
}

impl Drop for GLPostFXScene {
  fn drop(&mut self) {
    unsafe {
      gl::DeleteProgram(self.program);
      gl::DeleteBuffers(1, &self.vbo);
      gl::DeleteVertexArrays(1, &self.vao);
    }
  }
}
