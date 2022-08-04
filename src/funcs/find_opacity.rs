use crate::any::Opacity;
use rayon::prelude::*;
use sdl2::pixels::PixelFormatEnum;

pub(crate) fn find_opacity(pixels: &[u8], format: PixelFormatEnum) -> Opacity {
  match format {
    PixelFormatEnum::RGB332 => Opacity::Opaque,
    PixelFormatEnum::RGBA4444 => find_opacity_when_rgba4444(pixels),
    PixelFormatEnum::ABGR4444 => find_opacity_when_abgr4444(pixels),
    PixelFormatEnum::BGRA4444 => find_opacity_when_bgra4444(pixels),
    PixelFormatEnum::RGBA5551 => find_opacity_when_rgba5551(pixels),
    PixelFormatEnum::ABGR1555 => find_opacity_when_abgr1555(pixels),
    PixelFormatEnum::BGRA5551 => find_opacity_when_bgra5551(pixels),
    PixelFormatEnum::RGB565 => Opacity::Opaque,
    PixelFormatEnum::BGR565 => Opacity::Opaque,
    PixelFormatEnum::RGB24 => Opacity::Opaque,
    PixelFormatEnum::BGR24 => Opacity::Opaque,
    PixelFormatEnum::RGB888 => Opacity::Opaque,
    PixelFormatEnum::BGR888 => Opacity::Opaque,
    PixelFormatEnum::RGBA8888 => find_opacity_when_rgba8888(pixels),
    PixelFormatEnum::ABGR8888 => find_opacity_when_abgr8888(pixels),
    PixelFormatEnum::BGRA8888 => find_opacity_when_bgra8888(pixels),
    _ => panic!("Could not determine the opacity of the pixels given!"),
  }
}

fn find_opacity_when_rgba4444(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(2)
    .find_map_any(|pixel| {
      if (pixel[1] & 0x0F) == 0x0F {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_abgr4444(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(2)
    .find_map_any(|pixel| {
      if (pixel[0] & 0xF0) == 0xF0 {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_bgra4444(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(2)
    .find_map_any(|pixel| {
      if (pixel[1] & 0x0F) == 0x0F {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_rgba5551(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(2)
    .find_map_any(|pixel| {
      if (pixel[1] & 0x01) == 0x01 {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_abgr1555(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(2)
    .find_map_any(|pixel| {
      if (pixel[0] & 0x80) == 0x80 {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_bgra5551(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(2)
    .find_map_any(|pixel| {
      if (pixel[1] & 0x01) == 0x01 {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_rgba8888(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(4)
    .find_map_any(|pixel| {
      if pixel[3] == 0xFF {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_abgr8888(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(4)
    .find_map_any(|pixel| {
      if pixel[0] == 0xFF {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}

fn find_opacity_when_bgra8888(pixels: &[u8]) -> Opacity {
  pixels
    .par_chunks_exact(4)
    .find_map_any(|pixel| {
      if pixel[3] == 0xFF {
        None
      } else {
        Some(Opacity::Translucent)
      }
    })
    .unwrap_or(Opacity::Opaque)
}
