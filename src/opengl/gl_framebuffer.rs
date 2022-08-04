use super::GLTexture;
use crate::opengl::gl_texture::{self, ImgAttr};
use gl::types::*;
use iron_ingot::UVec2;

#[derive(Debug)]
pub(crate) struct GLFramebuffer {
  id: GLuint,
  tex: GLTexture,
}

impl GLFramebuffer {
  pub(crate) fn new(size: UVec2, tex_unit: GLuint) -> Self {
    debug_assert!(
      size.get_x() >= 1,
      "Width must be greater than or equal to 1!"
    );
    debug_assert!(
      size.get_y() >= 1,
      "Height must be greater than or equal to 1!"
    );
    Self {
      id: 0,
      tex: GLTexture::new(gl_texture::Arg {
        img_attr: ImgAttr::Size(size),
        tex_unit,
        min_filter: gl::LINEAR,
        mag_filter: gl::LINEAR,
        wrap_s: gl::CLAMP_TO_EDGE,
        wrap_t: gl::CLAMP_TO_EDGE,
      }),
    }
  }

  pub(crate) const fn get_tex_id(&self) -> GLuint {
    self.tex.get_id()
  }

  pub(crate) const fn get_size(&self) -> UVec2 {
    self.tex.get_size()
  }

  pub(crate) fn init(&mut self) {
    unsafe {
      gl::CreateFramebuffers(1, &mut self.id);
      self.tex.init();
      gl::NamedFramebufferTexture(self.id, gl::COLOR_ATTACHMENT0, self.tex.get_id(), 0);
    }
  }

  pub(crate) fn grow(&mut self) {
    self.tex.grow();
    unsafe {
      gl::NamedFramebufferTexture(self.id, gl::COLOR_ATTACHMENT0, self.tex.get_id(), 0);
    }
  }

  pub(crate) fn use_(&self) {
    unsafe {
      gl::BindFramebuffer(gl::FRAMEBUFFER, self.id);
      gl::Clear(gl::COLOR_BUFFER_BIT);
    }
  }
}

impl Drop for GLFramebuffer {
  fn drop(&mut self) {
    unsafe {
      gl::DeleteFramebuffers(1, &self.id);
    }
  }
}
