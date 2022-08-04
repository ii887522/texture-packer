mod any;
mod funcs;
mod gl_scene;
mod gl_stage;
mod opengl;
mod structs;

use iron_ingot::UVec2;
use opengl::GLApp;

fn main() {
  GLApp::new("Texture Packer", UVec2::new((1600, 900))).start();
}
