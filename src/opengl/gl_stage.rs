use sdl2::video::Window;
use std::fmt::Debug;

pub(crate) trait GLStage: Debug {
  fn show(&mut self, window: &Window);
}
