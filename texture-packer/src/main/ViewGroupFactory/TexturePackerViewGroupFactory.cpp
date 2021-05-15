// Copyright ii887522

#ifndef TEST

#include "TexturePackerViewGroupFactory.h"
#include <Factory/ViewGroupFactory.h>
#include <View/ViewGroup.h>
#include <SDL.h>
#include <Struct/Size.h>
#include <Struct/Point.h>
#include <View/Image.h>
#include <SDL_image.h>
#include <filesystem>
#include <string>
#include <Struct/Rect.h>
#include <Any/Enums.h>
#include <Functions/sdl_ext.h>
#include <vector>
#include "../Struct/Sprite.h"
#include "../Functions/util.h"
#include <Functions/fs_ext.h>
#include <Functions/string_ext.h>
#include <Functions/util.h>

using ii887522::viewify::ViewGroupFactory;
using ii887522::viewify::ViewGroup;
using ii887522::viewify::Size;
using ii887522::viewify::Point;
using ii887522::viewify::Image;
using ii887522::viewify::Rect;
using ii887522::viewify::Align;
using ii887522::viewify::Rotation;
using ii887522::viewify::Action;
using ii887522::viewify::snapshot;
using ii887522::nitro::sort;
using ii887522::nitro::write;
using std::filesystem::directory_iterator;
using std::string;
using std::vector;
using std::to_string;

namespace ii887522::texturePacker {

TexturePackerViewGroupFactory::TexturePackerViewGroupFactory(const string& inputDirPath, const string& outputDirPath) : ViewGroupFactory{ }, outputDirPath{ outputDirPath }, indicesI{ 0u },
  atlasIndex{ 0u } {
  addImages(inputDirPath);
  writeSpriteNameEnumFile(inputDirPath, outputDirPath);
  rotateSomeImages();
  sort<unsigned int, vector>(&indices, [this](const unsigned int& l, const unsigned int& r) {
    return sprites[l].rect.size.h < sprites[r].rect.size.h;
  });
}

void TexturePackerViewGroupFactory::addImages(const string& inputDirPath) {
  auto i{ 0u };
  for (const auto& entry : directory_iterator{ inputDirPath }) {
    addImage(entry.path().string(), i);
    ++i;
  }
}

void TexturePackerViewGroupFactory::addImage(const string& filePath, const unsigned int index) {
  surfaces.push_back(IMG_Load(filePath.c_str()));
  sprites.push_back(Sprite{ 0u, Rect{ Point{ 0, 0 }, Size{ surfaces.back()->w, surfaces.back()->h } } });
  indices.push_back(index);
}

void TexturePackerViewGroupFactory::rotateSomeImages() {
  for (auto& sprite : sprites) {
    if (sprite.rect.size.w < sprite.rect.size.h) rotate(sprite);
  }
}

ViewGroup TexturePackerViewGroupFactory::make(SDL_Renderer*const renderer, const Size<int>& size) {
  return ViewGroup{ renderer, Point{ 0, 0 }, { }, [this, renderer, size](ViewGroup& self) {
    constexpr auto gap{ 1 };
    Point<int> position{ gap, gap };
    auto rowH{ 0 };
    bool canReadRowH{ true };
    for (; indicesI != indices.size(); ++indicesI) {
      if (position.x + sprites[indices[indicesI]].rect.size.w > size.w) {
        position.x = gap;
        position.y += rowH + gap;
        if (position.y + sprites[indices[indicesI]].rect.size.h > size.h) break;
        sprites[indices[indicesI]].atlasIndex = atlasIndex;
        canReadRowH = true;
      }
      if (canReadRowH) {
        rowH = sprites[indices[indicesI]].rect.size.h;
        canReadRowH = false;
      }
      sprites[indices[indicesI]].rect.position = position;
      self.add(
        Image::Builder{ renderer, surfaces[indices[indicesI]], position, Align::LEFT, sprites[indices[indicesI]].isRotated ? Rotation::QUARTER_CLOCKWISE : Rotation::NONE }
          .setA(255u)
          .setDuration(1u)
          .build()
      );
      position.x += sprites[indices[indicesI]].rect.size.w + gap;
    }
    return Action::NONE;
  }, [this, renderer, size](ViewGroup& self) {
    snapshot(renderer, Rect{ Point{ 0, 0 }, size }, outputDirPath + "atlas_" + to_string(atlasIndex) + ".png");
    write<Sprite, vector>(outputDirPath + "atlas.dat", sprites);
    self.clear();
    ++atlasIndex;
    return indicesI == indices.size() ? Action::QUIT : Action::NONE;
  } };
}

}  // namespace ii887522::texturePacker

#endif
