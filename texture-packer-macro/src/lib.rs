use std::fs::File;
use png::{BitDepth, ColorType, OutputInfo, Transformations};
use proc_macro::TokenStream;
use quote::quote;

use syn::{
  Expr::{self, Lit},
  Lit::Str,
};

#[proc_macro]
pub fn load_png_file(input: TokenStream) -> TokenStream {
  let ast: Expr = syn::parse(input).expect("Argument of load_png_file function macro must be a literal string that represents a path to the PNG file!");
  if let Lit(expr_lit) = ast {
    if let Str(file_path) = expr_lit.lit {
      let mut decoder = png::Decoder::new(
        File::open(file_path.value())
          .unwrap_or_else(|_| panic!("Could not open the file \"{}\"", file_path.value())),
      );
      decoder.set_transformations(Transformations::all());
      let mut reader = decoder.read_info().unwrap();
      let mut buffer = vec![0; reader.output_buffer_size()];
      let info = reader.next_frame(&mut buffer).unwrap();
      let data = &buffer[..info.buffer_size()];
      let width = info.width;
      let height = info.height;
      let pitch = info.line_size as u32;
      let bpp = find_bpp(&info);
      let rmask = find_rmask(&info);
      let gmask = find_gmask(&info);
      let bmask = find_bmask(&info);
      let amask = find_amask(&info);
      quote! {
        {
          use crate::any::Image;

          Image {
            data: (&[#( #data, )*] as &[u8]).to_owned(),
            width: #width,
            height: #height,
            pitch: #pitch,
            bpp: #bpp,
            rmask: #rmask,
            gmask: #gmask,
            bmask: #bmask,
            amask: #amask,
          }
        }
      }
      .into()
    } else {
      panic!("Argument of load_png_file function macro must be a literal string that represents a path to the PNG file!");
    }
  } else {
    panic!("Argument of load_png_file function macro must be a literal string that represents a path to the PNG file!");
  }
}

fn find_bpp(info: &OutputInfo) -> u8 {
  match info.color_type {
    ColorType::Rgb => match info.bit_depth {
      BitDepth::One => 3,
      BitDepth::Two => 6,
      BitDepth::Four => 12,
      BitDepth::Eight => 24,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    ColorType::Rgba => match info.bit_depth {
      BitDepth::One => 4,
      BitDepth::Two => 8,
      BitDepth::Four => 16,
      BitDepth::Eight => 32,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    color_type => panic!(
      "Could not find out the bpp from this color type: {:?}",
      color_type
    ),
  }
}

fn find_rmask(info: &OutputInfo) -> u32 {
  match info.color_type {
    ColorType::Rgb => match info.bit_depth {
      BitDepth::One => 0b0001,
      BitDepth::Two => 0b00_00_00_11,
      BitDepth::Four => 0x000F,
      BitDepth::Eight => 0x00_00_00_FF,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    ColorType::Rgba => match info.bit_depth {
      BitDepth::One => 0b0010,
      BitDepth::Two => 0b00_00_11_00,
      BitDepth::Four => 0x00F0,
      BitDepth::Eight => 0x00_00_FF_00,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    color_type => panic!(
      "Could not find out the bpp from this color type: {:?}",
      color_type
    ),
  }
}

fn find_gmask(info: &OutputInfo) -> u32 {
  match info.color_type {
    ColorType::Rgb => match info.bit_depth {
      BitDepth::One => 0b0010,
      BitDepth::Two => 0b00_00_11_00,
      BitDepth::Four => 0x00F0,
      BitDepth::Eight => 0x00_00_FF_00,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    ColorType::Rgba => match info.bit_depth {
      BitDepth::One => 0b0100,
      BitDepth::Two => 0b00_11_00_00,
      BitDepth::Four => 0x0F00,
      BitDepth::Eight => 0x00_FF_00_00,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    color_type => panic!(
      "Could not find out the bpp from this color type: {:?}",
      color_type
    ),
  }
}

fn find_bmask(info: &OutputInfo) -> u32 {
  match info.color_type {
    ColorType::Rgb => match info.bit_depth {
      BitDepth::One => 0b0100,
      BitDepth::Two => 0b00_11_00_00,
      BitDepth::Four => 0x0F00,
      BitDepth::Eight => 0x00_FF_00_00,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    ColorType::Rgba => match info.bit_depth {
      BitDepth::One => 0b1000,
      BitDepth::Two => 0b_1100_00_00,
      BitDepth::Four => 0xF000,
      BitDepth::Eight => 0xFF_00_00_00,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    color_type => panic!(
      "Could not find out the bpp from this color type: {:?}",
      color_type
    ),
  }
}

fn find_amask(info: &OutputInfo) -> u32 {
  match info.color_type {
    ColorType::Rgb => match info.bit_depth {
      BitDepth::One => 0b0000,
      BitDepth::Two => 0b00_00_00_00,
      BitDepth::Four => 0x0000,
      BitDepth::Eight => 0x00_00_00_00,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    ColorType::Rgba => match info.bit_depth {
      BitDepth::One => 0b0001,
      BitDepth::Two => 0b00_00_00_11,
      BitDepth::Four => 0x000F,
      BitDepth::Eight => 0x00_00_00_FF,
      BitDepth::Sixteen => panic!("16-bit depth is not supported for finding out the bpp!"),
    },
    color_type => panic!(
      "Could not find out the bpp from this color type: {:?}",
      color_type
    ),
  }
}
