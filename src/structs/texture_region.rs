use super::AtlasRegion;
use gl::types::*;
use iron_ingot::UVec2;
use std::cell::RefCell;

#[derive(Clone, Debug)]
pub(crate) struct TextureRegion {
  pub name: String,
  pub atlas_id: GLuint,
  pub atlas_size: UVec2,
  pub prev_position: UVec2,
  pub position: UVec2,
  pub size: UVec2,
  pub is_rotated: bool,
  pub is_opaque: bool,
}

impl TextureRegion {
  pub(crate) fn reset_position(&mut self) {
    self.position = self.prev_position;
  }

  pub(crate) fn reset_positions(tex_regions: &[&RefCell<TextureRegion>]) {
    for &tex_region in tex_regions {
      tex_region.borrow_mut().reset_position();
    }
  }
}

impl From<AtlasRegion> for TextureRegion {
  fn from(
    AtlasRegion {
      name,
      atlas_width,
      atlas_height,
      x,
      y,
      w,
      h,
      is_rotated,
      is_opaque,
    }: AtlasRegion,
  ) -> Self {
    Self {
      name,
      atlas_id: 0,
      atlas_size: UVec2::new((atlas_width, atlas_height)),
      prev_position: UVec2::new((x, y)),
      position: UVec2::new((x, y)),
      size: UVec2::new((w, h)),
      is_rotated,
      is_opaque,
    }
  }
}
