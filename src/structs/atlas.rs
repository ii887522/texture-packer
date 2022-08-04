use super::AtlasRegion;
use serde::Serialize;

#[derive(Clone, Debug, Serialize)]
pub(crate) struct Atlas {
  pub regions: Vec<AtlasRegion>,
}
