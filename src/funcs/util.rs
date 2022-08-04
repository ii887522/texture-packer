use crate::structs::{Atlas, AtlasRegion, EnvArgs, TextureRegion};
use iron_ingot::UVec2;
use png::{BitDepth, ColorType, Compression, FilterType, ScaledFloat, SourceChromaticities};
use rayon::prelude::*;

use std::{
  borrow::Cow,
  cell::RefCell,
  fs::{self, File},
  io::BufWriter,
};

pub(crate) fn flip_vertically(pixels: &[u8], pitch: u32) -> Cow<[u8]> {
  pixels
    .par_chunks_exact(pitch as _)
    .rev()
    .flatten()
    .cloned()
    .collect()
}

pub fn find_name(file_path: &str) -> &str {
  &file_path[(file_path
    .rfind(|ch| ch == '/' || ch == '\\')
    .map(|id| id as isize)
    .unwrap_or(-1)
    + 1) as _..file_path.rfind('.').unwrap_or(file_path.len())]
}

pub(crate) fn save_atlas_toml_file(tex_regions: &mut [RefCell<TextureRegion>], env_args: &EnvArgs) {
  let atlas_toml = toml::to_string_pretty(&Atlas {
    regions: tex_regions
      .iter_mut()
      .map(|tex_region| AtlasRegion::from(tex_region.get_mut().clone()))
      .collect(),
  })
  .unwrap();
  fs::create_dir_all(&env_args.output_dir_path).unwrap();
  fs::write(
    format!(
      "{}/{}.toml",
      env_args.output_dir_path,
      &env_args.input_dir_path[(env_args
        .input_dir_path
        .rfind('/')
        .map(|id| id as isize)
        .unwrap_or(-1)
        + 1) as _..],
    ),
    atlas_toml,
  )
  .unwrap();
}

pub(crate) fn save_atlas_png_file(size: UVec2, env_args: &EnvArgs) {
  const CHANNEL_COUNT: u32 = 4;
  let mut atlas = Vec::<u8>::new();
  atlas.resize((size.get_x() * size.get_y() * CHANNEL_COUNT) as _, 0);
  unsafe {
    gl::ReadnPixels(
      0,
      0,
      size.get_x() as _,
      size.get_y() as _,
      gl::RGBA,
      gl::UNSIGNED_BYTE,
      atlas.len() as _,
      atlas.as_mut_ptr() as *mut _,
    );
  }
  fs::create_dir_all(&env_args.output_dir_path).unwrap();
  save_png_file(
    &format!(
      "{}/{}.png",
      env_args.output_dir_path,
      &env_args.input_dir_path[(env_args
        .input_dir_path
        .rfind('/')
        .map(|id| id as isize)
        .unwrap_or(-1)
        + 1) as _..]
    ),
    size,
    &atlas
      .par_rchunks_exact((size.get_x() * CHANNEL_COUNT) as _)
      .flatten()
      .map(|&data| data)
      .collect::<Vec<_>>(),
  );
}

pub(crate) fn save_png_file(file_path: &str, size: UVec2, image: &[u8]) {
  let mut encoder = png::Encoder::new(
    BufWriter::new(File::create(file_path).unwrap()),
    size.get_x(),
    size.get_y(),
  );
  encoder.set_color(ColorType::Rgba);
  encoder.set_depth(BitDepth::Eight);
  encoder.set_source_gamma(ScaledFloat::from_scaled(45455));
  encoder.set_source_chromaticities(SourceChromaticities::new(
    (0.31270, 0.32900),
    (0.64000, 0.33000),
    (0.30000, 0.60000),
    (0.15000, 0.06000),
  ));
  encoder.set_compression(Compression::Fast);
  encoder.set_filter(FilterType::Paeth);
  encoder
    .write_header()
    .unwrap()
    .write_image_data(image)
    .unwrap();
}
