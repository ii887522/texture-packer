use crate::{funcs::gl_ext::new_gl_program, opengl::GLFramebuffer, structs::Rect};
use gl::types::*;
use iron_ingot::UVec2;
use std::mem::{size_of, size_of_val};

#[derive(Debug)]
pub(crate) struct GLBasicScene {
  virtual_size: UVec2,
  vao: GLuint,
  vbo: GLuint,
  fbo: GLFramebuffer,
  program: GLuint,
}

impl GLBasicScene {
  pub(crate) fn new(physical_size: UVec2, virtual_size: UVec2, tex_unit: GLuint) -> Self {
    let mut this = Self {
      virtual_size,
      vao: 0,
      vbo: 0,
      fbo: GLFramebuffer::new(physical_size, tex_unit),
      program: 0,
    };
    this.init(virtual_size);
    this
  }

  fn init(&mut self, virtual_size: UVec2) {
    self.init_vao();
    self.init_vbo();
    self.fbo.init();
    self.program = new_gl_program(
      include_bytes!("../../res/basic.vert"),
      include_bytes!("../../res/basic.frag"),
    );
    self.init_uniforms(virtual_size);
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
        0.0f32, 1.0f32, 0.0f32, 1.0f32, // bottom left corner vertex
        1.0f32, 1.0f32, 1.0f32, 1.0f32, // bottom right corner vertex
        1.0f32, 0.0f32, 1.0f32, 0.0f32, // top right corner vertex
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

  fn init_uniforms(&self, virtual_size: UVec2) {
    unsafe {
      gl::ProgramUniform1i(self.program, 7, 0);
      gl::ProgramUniform2f(
        self.program,
        0,
        virtual_size.get_x() as _,
        virtual_size.get_y() as _,
      );
    }
  }

  pub(crate) const fn get_physical_size(&self) -> UVec2 {
    self.fbo.get_size()
  }

  pub(crate) const fn get_virtual_size(&self) -> UVec2 {
    self.virtual_size
  }

  pub(crate) const fn get_fbo(&self) -> &GLFramebuffer {
    &self.fbo
  }

  pub(crate) fn set_atlas_size(&self, value: UVec2) {
    debug_assert!(
      value.get_x() >= 1,
      "Width must be greater than or equal to 1!"
    );
    debug_assert!(
      value.get_y() >= 1,
      "Height must be greater than or equal to 1!"
    );
    unsafe {
      gl::ProgramUniform2f(self.program, 1, value.get_x() as _, value.get_y() as _);
    }
  }

  pub(crate) fn set_rect(&self, rect: Rect) {
    debug_assert!(
      rect.size.get_x() >= 1,
      "Width must be greater than or equal to 1!"
    );
    debug_assert!(
      rect.size.get_y() >= 1,
      "Height must be greater than or equal to 1!"
    );
    debug_assert!(
      rect.tex_size.get_x() >= 1,
      "Texture width must be greater than or equal to 1!"
    );
    debug_assert!(
      rect.tex_size.get_y() >= 1,
      "Texture height must be greater than or equal to 1!"
    );
    unsafe {
      gl::ProgramUniform2f(
        self.program,
        2,
        rect.position.get_x(),
        rect.position.get_y(),
      );
      gl::ProgramUniform2f(
        self.program,
        3,
        rect.size.get_x() as _,
        rect.size.get_y() as _,
      );
      gl::ProgramUniform2f(
        self.program,
        4,
        rect.tex_position.get_x() as _,
        rect.tex_position.get_y() as _,
      );
      gl::ProgramUniform2f(
        self.program,
        5,
        rect.tex_size.get_x() as _,
        rect.tex_size.get_y() as _,
      );
      gl::ProgramUniform1i(self.program, 6, if rect.is_rotated { 1 } else { 0 });
    }
  }

  pub(crate) fn show(&self) {
    unsafe {
      gl::BindVertexArray(self.vao);
      gl::UseProgram(self.program);
      gl::DrawArrays(gl::TRIANGLE_FAN, 0, 4);
    }
  }

  pub(crate) fn grow(&mut self) {
    self.grow_virtual_size();
    self.fbo.grow();
    unsafe {
      gl::ProgramUniform2f(
        self.program,
        0,
        self.virtual_size.get_x() as _,
        self.virtual_size.get_y() as _,
      );
    }
  }

  fn grow_virtual_size(&mut self) {
    self.virtual_size = if self.virtual_size.get_x() == self.virtual_size.get_y() {
      UVec2::new((self.virtual_size.get_x() << 1, self.virtual_size.get_y()))
    } else {
      UVec2::new((self.virtual_size.get_x(), self.virtual_size.get_y() << 1))
    };
  }
}

impl Drop for GLBasicScene {
  fn drop(&mut self) {
    unsafe {
      gl::DeleteProgram(self.program);
      gl::DeleteBuffers(1, &self.vbo);
      gl::DeleteVertexArrays(1, &self.vao);
    }
  }
}
