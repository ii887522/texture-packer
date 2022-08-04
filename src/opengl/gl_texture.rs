use super::GLPixelFormat;
use gl::types::*;
use iron_ingot::UVec2;
use sdl2::{image::LoadSurface, surface::Surface};
use texture_packer::funcs::util::find_name;

use crate::{
  any::{image::Image, Opacity},
  funcs::{find_opacity, util::flip_vertically},
};

#[derive(Debug)]
pub(crate) enum ImgAttr {
  Path(String),
  Size(UVec2),
  Image(Image),
}

#[derive(Debug)]
pub(crate) struct Arg {
  pub img_attr: ImgAttr,
  pub tex_unit: GLuint,
  pub min_filter: GLenum,
  pub mag_filter: GLenum,
  pub wrap_s: GLenum,
  pub wrap_t: GLenum,
}

#[derive(Debug)]
pub(crate) struct GLTexture {
  id: GLuint,
  img_attr: ImgAttr,
  tex_unit: GLuint,
  min_filter: GLenum,
  mag_filter: GLenum,
  wrap_s: GLenum,
  wrap_t: GLenum,
  size: UVec2,
  is_opaque: bool,
}

impl GLTexture {
  pub(crate) fn new(arg: Arg) -> Self {
    let Arg {
      img_attr,
      tex_unit,
      min_filter,
      mag_filter,
      wrap_s,
      wrap_t,
    } = arg;
    if let ImgAttr::Size(size) = img_attr {
      debug_assert!(
        size.get_x() >= 1,
        "Width must be greater than or equal to 1!"
      );
      debug_assert!(
        size.get_y() >= 1,
        "Height must be greater than or equal to 1!"
      );
    }
    Self {
      id: 0,
      img_attr,
      tex_unit,
      min_filter,
      mag_filter,
      wrap_s,
      wrap_t,
      size: UVec2::new(()),
      is_opaque: false,
    }
  }

  pub(crate) fn get_name(&self) -> Option<&str> {
    if let ImgAttr::Path(img_path) = &self.img_attr {
      Some(find_name(img_path))
    } else {
      None
    }
  }

  pub(crate) const fn get_id(&self) -> GLuint {
    self.id
  }

  pub(crate) const fn get_size(&self) -> UVec2 {
    self.size
  }

  pub(crate) const fn is_opaque(&self) -> bool {
    self.is_opaque
  }

  pub(crate) fn init(&mut self) {
    unsafe {
      gl::CreateTextures(gl::TEXTURE_2D, 1, &mut self.id);
      gl::BindTextureUnit(self.tex_unit, self.id);
      gl::TextureParameteri(self.id, gl::TEXTURE_MIN_FILTER, self.min_filter as _);
      gl::TextureParameteri(self.id, gl::TEXTURE_MAG_FILTER, self.mag_filter as _);
      gl::TextureParameteri(self.id, gl::TEXTURE_WRAP_S, self.wrap_s as _);
      gl::TextureParameteri(self.id, gl::TEXTURE_WRAP_T, self.wrap_t as _);
      gl::TextureParameterfv(
        self.id,
        gl::TEXTURE_BORDER_COLOR,
        &[1.0f32, 1.0f32, 1.0f32, 1.0f32] as *const _,
      );
      match &mut self.img_attr {
        ImgAttr::Path(img_path) => {
          let mut img = Surface::from_file(img_path).unwrap();
          self.size = UVec2::new((img.width(), img.height()));
          gl::TextureStorage2D(self.id, 1, gl::RGBA8, img.width() as _, img.height() as _);
          let pixel_format = img.pixel_format_enum();
          let gl_pixel_format = GLPixelFormat::from(pixel_format);
          let pitch = img.pitch();
          let width = img.width();
          let height = img.height();
          let pixels = img.without_lock_mut().unwrap();
          if find_opacity(pixels, pixel_format) == Opacity::Opaque {
            self.is_opaque = true;
          }
          flip_vertically(pixels, pitch);
          gl::TextureSubImage2D(
            self.id,
            0,
            0,
            0,
            width as _,
            height as _,
            gl_pixel_format.get_format(),
            gl_pixel_format.get_type(),
            pixels as *const _ as *const _,
          );
        }
        &mut ImgAttr::Size(size) => {
          self.size = size;
          gl::TextureStorage2D(self.id, 1, gl::RGBA8, size.get_x() as _, size.get_y() as _)
        }
        ImgAttr::Image(img) => {
          let mut img = Surface::from(img);
          self.size = UVec2::new((img.width(), img.height()));
          gl::TextureStorage2D(self.id, 1, gl::RGBA8, img.width() as _, img.height() as _);
          let gl_pixel_format = GLPixelFormat::from(img.pixel_format_enum());
          let pitch = img.pitch();
          let width = img.width();
          let height = img.height();
          let pixels = img.without_lock_mut().unwrap();
          flip_vertically(pixels, pitch);
          gl::TextureSubImage2D(
            self.id,
            0,
            0,
            0,
            width as _,
            height as _,
            gl_pixel_format.get_format(),
            gl_pixel_format.get_type(),
            pixels as *const _ as *const _,
          );
        }
      }
    }
  }

  pub(crate) fn grow(&mut self) {
    if let ImgAttr::Size(size) = self.img_attr {
      self.img_attr = ImgAttr::Size(if size.get_x() == size.get_y() {
        UVec2::new((self.size.get_x() << 1, self.size.get_y()))
      } else {
        UVec2::new((self.size.get_x(), self.size.get_y() << 1))
      });
      unsafe {
        gl::DeleteTextures(1, &self.id);
      }
      self.init();
    }
  }
}

impl Drop for GLTexture {
  fn drop(&mut self) {
    unsafe {
      gl::DeleteTextures(1, &self.id);
    }
  }
}
