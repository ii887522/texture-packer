use super::TextureRegion;
use serde::Serialize;

#[derive(Clone, Debug, Serialize)]
pub(crate) struct AtlasRegion {
  pub name: String,
  pub atlas_width: u32,
  pub atlas_height: u32,
  pub x: u32,
  pub y: u32,
  pub w: u32,
  pub h: u32,
  pub is_rotated: bool,
  pub is_opaque: bool,
}

impl From<TextureRegion> for AtlasRegion {
  fn from(
    TextureRegion {
      name,
      atlas_size,
      position,
      size,
      is_rotated,
      is_opaque,
      ..
    }: TextureRegion,
  ) -> Self {
    Self {
      name,
      atlas_width: atlas_size.get_x(),
      atlas_height: atlas_size.get_y(),
      x: position.get_x(),
      y: position.get_y(),
      w: size.get_x(),
      h: size.get_y(),
      is_rotated,
      is_opaque,
    }
  }
}
