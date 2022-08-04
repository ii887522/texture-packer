use sdl2::{pixels::PixelMasks, surface::Surface};

#[derive(Debug)]
pub(crate) struct Image {
  pub data: Vec<u8>,
  pub width: u32,
  pub height: u32,
  pub pitch: u32,
  pub bpp: u8,
  pub rmask: u32,
  pub gmask: u32,
  pub bmask: u32,
  pub amask: u32,
}

impl<'a> From<&'a mut Image> for Surface<'a> {
  fn from(
    Image {
      data,
      width,
      height,
      pitch,
      bpp,
      rmask,
      gmask,
      bmask,
      amask,
    }: &'a mut Image,
  ) -> Self {
    Surface::from_data_pixelmasks(
      &mut data[..],
      *width,
      *height,
      *pitch,
      PixelMasks {
        bpp: *bpp,
        rmask: *rmask,
        gmask: *gmask,
        bmask: *bmask,
        amask: *amask,
      },
    )
    .unwrap()
  }
}
