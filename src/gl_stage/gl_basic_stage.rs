use iron_ingot::{FVec2, UVec2};
use rayon::prelude::*;
use sdl2::video::Window;
use texture_packer_macro::load_png_file;

use std::{
  cell::{Cell, RefCell},
  env,
  error::Error,
  fmt::{self, Display, Formatter},
  fs,
  io::ErrorKind,
};

use crate::{
  any::consts::{BASIC_ATLAS_GAP, BLUR_INPUT_ATLAS_GAP, INITIAL_SIZE},
  funcs::util::{save_atlas_png_file, save_atlas_toml_file},
  gl_scene::{gl_postfx_scene, GLBasicScene, GLPostFXScene},
  opengl::{
    gl_texture::{self, ImgAttr},
    GLStage, GLTexture,
  },
  structs::{BlurInput, EnvArgs, Rect, TextureRegion},
};

#[derive(Copy, Clone, Debug)]
struct AtlasOverflowError;

impl Error for AtlasOverflowError {}

impl Display for AtlasOverflowError {
  fn fmt(&self, formatter: &mut Formatter<'_>) -> fmt::Result {
    write!(
      formatter,
      "There is not enough space to fill the texture given!"
    )
  }
}

#[derive(Debug)]
pub(crate) struct GLBasicStage {
  env_args: EnvArgs,
  // size: UVec2,
  _dot: GLTexture,
  _blur_input_texs: Option<Vec<GLTexture>>,
  blur_input_tex_regions: Option<Vec<RefCell<TextureRegion>>>,
  _texs: Vec<GLTexture>,
  tex_regions: Vec<RefCell<TextureRegion>>,
  blur_input_scene: Option<RefCell<GLBasicScene>>,
  blur_scenes: Option<RefCell<Vec<GLPostFXScene>>>,
  basic_scene: GLBasicScene,
  // identity_scene: GLPostFXScene,
  current_tex_position: Cell<UVec2>,
  max_tex_h: Cell<u32>,
  used_size: Cell<UVec2>,
}

impl GLBasicStage {
  pub(crate) fn new(_size: UVec2) -> Self {
    let mut command_line_args = env::args().take(3);
    command_line_args.next();
    let input_dir_path = command_line_args
      .next()
      .expect("The first argument which is an input directory path is missing and it is required!")
      .trim_end_matches('/')
      .to_owned();
    let output_dir_path = command_line_args
      .next()
      .expect(
        "The second argument which is an output directory path is missing and it is required!",
      )
      .trim_end_matches('/')
      .to_owned();
    let blur_input =
      if let Ok(blur_input) = fs::read_to_string(format!("{input_dir_path}/blur.toml")) {
        let blur_input = toml::from_str::<BlurInput>(&blur_input).unwrap_or_else(|err| {
          if let Some((line, col)) = err.line_col() {
            panic!("Syntax error occured at line {line}, col {col} in {input_dir_path}/blur.toml!");
          } else {
            panic!("Syntax error occured in {input_dir_path}/blur.toml!");
          }
        });
        blur_input.validate();
        Some(blur_input)
      } else {
        None
      };
    let blur_input_texs = if let Some(blur_input) = &blur_input {
      blur_input.img_file_names.as_ref().map(|img_file_names|
        img_file_names.iter().map(|img_file_name| {
          if fs::metadata(format!("{input_dir_path}/{img_file_name}")).unwrap_or_else(|err| {
            match err.kind() {
              ErrorKind::NotFound => panic!("\"{input_dir_path}/{img_file_name}\" does not exist!"),
              ErrorKind::PermissionDenied => panic!("\"{input_dir_path}/{img_file_name}\" exists but insufficient permissions to access it!"),
              _ => panic!("{err:?}"),
            }
          }).is_file() && img_file_name.ends_with(".png") {
            let mut tex = GLTexture::new(gl_texture::Arg {
              img_attr: ImgAttr::Path(format!("{input_dir_path}/{img_file_name}")),
              tex_unit: 0,
              min_filter: gl::NEAREST,
              mag_filter: gl::NEAREST,
              wrap_s: gl::CLAMP_TO_EDGE,
              wrap_t: gl::CLAMP_TO_EDGE
            });
            tex.init();
            tex
          } else {
            panic!("\"{input_dir_path}/{img_file_name}\" is not a PNG file!");
          }
        }).collect::<Vec<_>>()
      )
    } else {
      None
    };
    let texs = fs::read_dir(&input_dir_path).unwrap_or_else(|err| {
      match err.kind() {
        ErrorKind::NotFound => panic!("The input directory at \"{input_dir_path}\" does not exist!"),
        ErrorKind::PermissionDenied => panic!("The input directory at \"{input_dir_path}\" exists but insufficient permissions to access it!"),
        _ => panic!("{err:?}"),
      }
    }).filter_map(|entry| {
      let entry = entry.unwrap();
      if entry.file_type().unwrap().is_file() && entry.file_name().to_string_lossy().ends_with(".png") {
        let mut tex = GLTexture::new(gl_texture::Arg {
          img_attr: ImgAttr::Path(entry.path().to_string_lossy().to_string()),
          tex_unit: 0,
          min_filter: gl::NEAREST,
          mag_filter: gl::NEAREST,
          wrap_s: gl::CLAMP_TO_EDGE,
          wrap_t: gl::CLAMP_TO_EDGE
        });
        tex.init();
        Some(tex)
      } else {
        None
      }
    }).collect::<Vec<_>>();
    let mut dot = GLTexture::new(gl_texture::Arg {
      img_attr: ImgAttr::Image(load_png_file!("res/dot.png")),
      tex_unit: 0,
      min_filter: gl::NEAREST,
      mag_filter: gl::NEAREST,
      wrap_s: gl::CLAMP_TO_EDGE,
      wrap_t: gl::CLAMP_TO_EDGE,
    });
    dot.init();
    let mut blur_input_tex_regions =
      blur_input_texs
        .as_ref()
        .map(|blur_input_texs: &Vec<GLTexture>| {
          blur_input_texs
            .par_iter()
            .map(|blur_input_tex| {
              RefCell::new(TextureRegion {
                name: blur_input_tex.get_name().unwrap().to_owned() + "_blur",
                atlas_id: blur_input_tex.get_id(),
                atlas_size: blur_input_tex.get_size(),
                prev_position: UVec2::new(()),
                position: UVec2::new(()),
                size: blur_input_tex.get_size(),
                is_rotated: false,
                is_opaque: false,
              })
            })
            .collect()
        });
    if let Some(blur_input) = &blur_input {
      if let Some(rects) = &blur_input.rects {
        blur_input_tex_regions.get_or_insert(vec![]).append(
          &mut rects
            .iter()
            .map(|(name, rect)| {
              RefCell::new(TextureRegion {
                name: name.to_owned() + "_blur",
                atlas_id: dot.get_id(),
                atlas_size: dot.get_size(),
                prev_position: UVec2::new(()),
                position: UVec2::new(()),
                size: UVec2::new((rect.w, rect.h)),
                is_rotated: false,
                is_opaque: false,
              })
            })
            .collect::<Vec<_>>(),
        );
      }
    }
    let mut this = Self {
      env_args: EnvArgs {
        input_dir_path,
        output_dir_path,
      },
      // size,
      tex_regions: texs
        .par_iter()
        .map(|tex| {
          RefCell::new(TextureRegion {
            name: tex.get_name().unwrap().to_owned(),
            atlas_id: tex.get_id(),
            atlas_size: tex.get_size(),
            prev_position: UVec2::new(()),
            position: UVec2::new(()),
            size: tex.get_size(),
            is_rotated: tex.get_size().get_x() < tex.get_size().get_y(),
            is_opaque: tex.is_opaque(),
          })
        })
        .collect(),
      _texs: texs,
      blur_input_scene: blur_input_tex_regions.as_ref().map(|_| {
        RefCell::new(GLBasicScene::new(
          UVec2::new((INITIAL_SIZE >> 1, INITIAL_SIZE >> 1)),
          UVec2::new((INITIAL_SIZE, INITIAL_SIZE)),
          1,
        ))
      }),
      blur_scenes: blur_input_tex_regions.as_ref().map(|_| {
        RefCell::new(vec![
          GLPostFXScene::new(gl_postfx_scene::Arg {
            size: UVec2::new((INITIAL_SIZE >> 1, INITIAL_SIZE >> 1)),
            frag_shader_code: include_bytes!("../../res/blur.frag"),
            from_tex_unit: 1,
            to_tex_unit: Some(2),
            init_uniforms: |program| unsafe {
              gl::ProgramUniform1i(program, 2, 1);
            },
          }),
          GLPostFXScene::new(gl_postfx_scene::Arg {
            size: UVec2::new((INITIAL_SIZE >> 1, INITIAL_SIZE >> 1)),
            frag_shader_code: include_bytes!("../../res/blur.frag"),
            from_tex_unit: 2,
            to_tex_unit: Some(3),
            init_uniforms: |program| unsafe {
              gl::ProgramUniform1i(program, 2, 0);
            },
          }),
        ])
      }),
      blur_input_tex_regions,
      _dot: dot,
      _blur_input_texs: blur_input_texs,
      basic_scene: GLBasicScene::new(
        UVec2::new((INITIAL_SIZE, INITIAL_SIZE)),
        UVec2::new((INITIAL_SIZE, INITIAL_SIZE)),
        4,
      ),
      // identity_scene: GLPostFXScene::new(gl_postfx_scene::Arg {
      //   size,
      //   frag_shader_code: include_bytes!("../../res/identity.frag"),
      //   from_tex_unit: 4,
      //   to_tex_unit: None,
      //   init_uniforms: |_| {},
      // }),
      current_tex_position: Cell::new(UVec2::new((BASIC_ATLAS_GAP >> 1, BASIC_ATLAS_GAP >> 1))),
      max_tex_h: Cell::new(0),
      used_size: Cell::new(UVec2::new(())),
    };
    if let Some(blur_input_tex_regions) = &mut this.blur_input_tex_regions {
      blur_input_tex_regions.par_sort_unstable_by(|a, b| {
        let a = a.borrow();
        let b = b.borrow();
        b.size
          .get_x()
          .min(b.size.get_y())
          .cmp(&a.size.get_x().min(a.size.get_y()))
      });
    }
    this
  }

  fn show_atlas(
    &self,
    basic_scene: &GLBasicScene,
    tex_regions: &[RefCell<TextureRegion>],
    gap: u32,
  ) -> Result<(), AtlasOverflowError> {
    self
      .current_tex_position
      .set(UVec2::new((gap >> 1, gap >> 1)));
    self.max_tex_h.set(0);
    self.used_size.set(UVec2::new(()));
    basic_scene.get_fbo().use_();
    unsafe {
      gl::Viewport(
        0,
        0,
        basic_scene.get_fbo().get_size().get_x() as _,
        basic_scene.get_fbo().get_size().get_y() as _,
      );
    }
    for (i, tex_region) in tex_regions.iter().enumerate() {
      if self.show_tex_region(basic_scene, tex_region, gap).is_err() {
        TextureRegion::reset_positions(&tex_regions.iter().take(i).collect::<Vec<_>>());
        return Err(AtlasOverflowError);
      }
    }
    Ok(())
  }

  fn show_tex_region(
    &self,
    basic_scene: &GLBasicScene,
    tex_region: &RefCell<TextureRegion>,
    gap: u32,
  ) -> Result<(), AtlasOverflowError> {
    let atlas_tex_size: UVec2;
    {
      let tex_region = tex_region.borrow();
      atlas_tex_size = UVec2::new((
        tex_region.size.get_x().max(tex_region.size.get_y()),
        tex_region.size.get_x().min(tex_region.size.get_y()),
      ));
      if self.current_tex_position.get().get_x() + atlas_tex_size.get_x() + (gap >> 1)
        > basic_scene.get_virtual_size().get_x()
      {
        self.current_tex_position.set(UVec2::new((
          gap >> 1,
          self.current_tex_position.get().get_y() + self.max_tex_h.get() + gap,
        )));
        self.max_tex_h.set(0);
        if self.current_tex_position.get().get_x() + atlas_tex_size.get_x() + (gap >> 1)
          > basic_scene.get_virtual_size().get_x()
          || self.current_tex_position.get().get_y() + atlas_tex_size.get_y() + (gap >> 1)
            > basic_scene.get_virtual_size().get_y()
        {
          return Err(AtlasOverflowError);
        }
      }
      unsafe {
        gl::BindTextureUnit(0, tex_region.atlas_id);
      }
      basic_scene.set_atlas_size(tex_region.atlas_size);
      basic_scene.set_rect(Rect {
        position: self.current_tex_position.get().into(),
        size: atlas_tex_size,
        tex_position: tex_region.position,
        tex_size: tex_region.size,
        is_rotated: tex_region.size.get_x() < tex_region.size.get_y(),
      });
      basic_scene.show();
    }
    tex_region.borrow_mut().position =
      self.current_tex_position.get() - UVec2::new((gap >> 1, gap >> 1));
    self
      .current_tex_position
      .set(self.current_tex_position.get() + UVec2::new((atlas_tex_size.get_x() + gap, 0)));
    self
      .max_tex_h
      .set(self.max_tex_h.get().max(atlas_tex_size.get_y()));
    self.used_size.set(UVec2::new((
      self
        .used_size
        .get()
        .get_x()
        .max(self.current_tex_position.get().get_x() - (gap >> 1)),
      self
        .used_size
        .get()
        .get_y()
        .max(self.current_tex_position.get().get_y() + self.max_tex_h.get() + (gap >> 1)),
    )));
    Ok(())
  }

  fn update_blur_scenes_scale(&self) {
    let scale = FVec2::from(self.used_size.get())
      / FVec2::from(
        self
          .blur_input_scene
          .as_ref()
          .unwrap()
          .borrow()
          .get_virtual_size(),
      );
    for blur_scene in self.blur_scenes.as_ref().unwrap().borrow().iter() {
      blur_scene.set_scale(scale)
    }
  }

  fn grow(&self) {
    self.blur_input_scene.as_ref().unwrap().borrow_mut().grow();
    self.grow_blur_scenes();
  }

  fn grow_blur_scenes(&self) {
    for blur_scene in self.blur_scenes.as_ref().unwrap().borrow_mut().iter_mut() {
      blur_scene.grow();
    }
  }

  fn update_blur_input_tex_regions(&self) {
    for blur_input_tex_region in self.blur_input_tex_regions.as_ref().unwrap() {
      self.update_blur_input_tex_region(blur_input_tex_region);
    }
  }

  fn update_blur_input_tex_region(&self, blur_input_tex_region: &RefCell<TextureRegion>) {
    let mut blur_input_tex_region = blur_input_tex_region.borrow_mut();
    blur_input_tex_region.atlas_id = self.blur_scenes.as_ref().unwrap().borrow()[1]
      .get_tex_id()
      .unwrap();
    blur_input_tex_region.atlas_size = self
      .blur_input_scene
      .as_ref()
      .unwrap()
      .borrow()
      .get_virtual_size();
    blur_input_tex_region.prev_position = blur_input_tex_region.position;
    blur_input_tex_region.is_rotated =
      blur_input_tex_region.size.get_x() < blur_input_tex_region.size.get_y();
    blur_input_tex_region.size = UVec2::new((
      blur_input_tex_region
        .size
        .get_x()
        .max(blur_input_tex_region.size.get_y())
        + BLUR_INPUT_ATLAS_GAP,
      blur_input_tex_region
        .size
        .get_x()
        .min(blur_input_tex_region.size.get_y())
        + BLUR_INPUT_ATLAS_GAP,
    ));
  }

  fn blur(&self) {
    let blur_scenes = self.blur_scenes.as_ref().unwrap().borrow();
    const BLUR: usize = 4;
    for i in 0..BLUR {
      for blur_scene in blur_scenes.iter() {
        blur_scene.show();
      }
      if i == 0 {
        blur_scenes[0].set_from_tex_unit(3);
      }
    }
    blur_scenes[0].set_from_tex_unit(1);
  }

  fn update_tex_regions(&self) {
    for tex_region in &self.tex_regions {
      self.update_tex_region(tex_region);
    }
  }

  fn update_tex_region(&self, tex_region: &RefCell<TextureRegion>) {
    let mut tex_region = tex_region.borrow_mut();
    tex_region.atlas_id = self.basic_scene.get_fbo().get_tex_id();
    tex_region.atlas_size = self.basic_scene.get_virtual_size();
    tex_region.prev_position = tex_region.position;
    tex_region.size = UVec2::new((
      tex_region.size.get_x().max(tex_region.size.get_y()) + BASIC_ATLAS_GAP,
      tex_region.size.get_x().min(tex_region.size.get_y()) + BASIC_ATLAS_GAP,
    ));
  }
}

impl GLStage for GLBasicStage {
  fn show(&mut self, _window: &Window) {
    if let Some(blur_input_tex_regions) = &self.blur_input_tex_regions {
      while self
        .show_atlas(
          &self.blur_input_scene.as_ref().unwrap().borrow(),
          blur_input_tex_regions,
          BLUR_INPUT_ATLAS_GAP,
        )
        .is_err()
      {
        self.grow();
      }
      self.update_blur_input_tex_regions();
      self.update_blur_scenes_scale();
      self.blur();
      self
        .tex_regions
        .append(self.blur_input_tex_regions.as_mut().unwrap());
    }
    self.tex_regions.par_sort_unstable_by(|a, b| {
      let a = a.borrow();
      let b = b.borrow();
      b.size
        .get_x()
        .min(b.size.get_y())
        .cmp(&a.size.get_x().min(a.size.get_y()))
    });
    while self
      .show_atlas(&self.basic_scene, &self.tex_regions, BASIC_ATLAS_GAP)
      .is_err()
    {
      self.basic_scene.grow();
    }
    self.update_tex_regions();
    save_atlas_png_file(self.basic_scene.get_physical_size(), &self.env_args);
    save_atlas_toml_file(&mut self.tex_regions, &self.env_args);

    // Show the output atlas onto the screen
    //
    // unsafe {
    //   gl::Viewport(0, 0, self.size.get_x() as _, self.size.get_y() as _);
    //   gl::Enable(gl::BLEND);
    // }
    // self.identity_scene.set_scale(
    //   FVec2::from(self.used_size.get()) / FVec2::from(self.basic_scene.get_virtual_size()),
    // );
    // self.identity_scene.show();
    // unsafe {
    //   gl::Disable(gl::BLEND);
    // }
    // window.gl_swap_window();
  }
}
