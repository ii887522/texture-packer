use iron_ingot::{FVec2, UVec2};
use std::hash::{Hash, Hasher};

#[derive(Copy, Clone, Debug)]
pub(crate) struct Rect {
  pub position: FVec2,
  pub size: UVec2,
  pub tex_position: UVec2,
  pub tex_size: UVec2,
  pub is_rotated: bool,
}

impl Default for Rect {
  fn default() -> Self {
    Self {
      position: FVec2::new((0.0, 0.0)),
      size: UVec2::new((1, 1)),
      tex_position: UVec2::new((u32::MAX, u32::MAX)),
      tex_size: UVec2::new((1, 1)),
      is_rotated: false,
    }
  }
}

impl PartialEq for Rect {
  fn eq(&self, other: &Self) -> bool {
    self.size == other.size
      && self.tex_position == other.tex_position
      && self.tex_size == other.tex_size
      && self.is_rotated == other.is_rotated
  }
}

impl Eq for Rect {}

impl Hash for Rect {
  fn hash<H: Hasher>(&self, state: &mut H) {
    self.size.hash(state);
    self.tex_position.hash(state);
    self.tex_size.hash(state);
    self.is_rotated.hash(state);
  }
}
