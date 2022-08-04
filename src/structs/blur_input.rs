use super::Size;
use serde::Deserialize;
use std::collections::{HashMap, HashSet};

#[derive(Debug, Deserialize)]
pub(crate) struct BlurInput {
  pub img_file_names: Option<HashSet<String>>,
  pub rects: Option<HashMap<String, Size>>,
}

impl BlurInput {
  pub(crate) fn validate(&self) {
    if let Some(rects) = &self.rects {
      for rect in rects.values() {
        rect.validate();
      }
    }
  }
}
