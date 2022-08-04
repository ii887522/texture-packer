use serde::Deserialize;

#[derive(Copy, Clone, Debug, Deserialize)]
pub(crate) struct Size {
  pub w: u32,
  pub h: u32,
}

impl Size {
  pub(crate) const fn validate(&self) {
    if self.w == 0 {
      panic!("Width must be greater than or equal to 1!");
    }
    if self.h == 0 {
      panic!("Height must be greater than or equal to 1!");
    }
  }
}
