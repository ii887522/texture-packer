use gl::types::*;
use sdl2::pixels::PixelFormatEnum;

#[derive(Copy, Clone, Debug)]
pub(crate) struct GLPixelFormat {
  format: GLenum,
  type_: GLenum,
}

impl GLPixelFormat {
  pub(crate) const fn get_format(&self) -> GLenum {
    self.format
  }

  pub(crate) const fn get_type(&self) -> GLenum {
    self.type_
  }
}

impl From<PixelFormatEnum> for GLPixelFormat {
  fn from(value: PixelFormatEnum) -> Self {
    match value {
      PixelFormatEnum::RGB332 => Self {
        format: gl::RGB,
        type_: gl::UNSIGNED_BYTE_3_3_2,
      },
      PixelFormatEnum::RGBA4444 => Self {
        format: gl::RGBA,
        type_: gl::UNSIGNED_SHORT_4_4_4_4,
      },
      PixelFormatEnum::ABGR4444 => Self {
        format: gl::RGBA,
        type_: gl::UNSIGNED_SHORT_4_4_4_4,
      },
      PixelFormatEnum::BGRA4444 => Self {
        format: gl::BGRA,
        type_: gl::UNSIGNED_SHORT_4_4_4_4,
      },
      PixelFormatEnum::RGBA5551 => Self {
        format: gl::RGBA,
        type_: gl::UNSIGNED_SHORT_5_5_5_1,
      },
      PixelFormatEnum::ABGR1555 => Self {
        format: gl::RGBA,
        type_: gl::UNSIGNED_SHORT_1_5_5_5_REV,
      },
      PixelFormatEnum::BGRA5551 => Self {
        format: gl::BGRA,
        type_: gl::UNSIGNED_SHORT_5_5_5_1,
      },
      PixelFormatEnum::RGB565 => Self {
        format: gl::RGB,
        type_: gl::UNSIGNED_SHORT_5_6_5,
      },
      PixelFormatEnum::BGR565 => Self {
        format: gl::BGR,
        type_: gl::UNSIGNED_SHORT_5_6_5,
      },
      PixelFormatEnum::RGB24 => Self {
        format: gl::RGB,
        type_: gl::UNSIGNED_BYTE,
      },
      PixelFormatEnum::BGR24 => Self {
        format: gl::BGR,
        type_: gl::UNSIGNED_BYTE,
      },
      PixelFormatEnum::RGB888 => Self {
        format: gl::RGB,
        type_: gl::UNSIGNED_BYTE,
      },
      PixelFormatEnum::BGR888 => Self {
        format: gl::BGR,
        type_: gl::UNSIGNED_BYTE,
      },
      PixelFormatEnum::RGBA8888 => Self {
        format: gl::RGBA,
        type_: gl::UNSIGNED_BYTE,
      },
      PixelFormatEnum::ABGR8888 => Self {
        format: gl::RGBA,
        type_: gl::UNSIGNED_BYTE,
      },
      PixelFormatEnum::BGRA8888 => Self {
        format: gl::BGRA,
        type_: gl::UNSIGNED_BYTE,
      },
      value => panic!("Could not convert {:?} to a GL pixel format!", value),
    }
  }
}
