// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_STRUCT_SPRITE_H_
#define TEXTURE_PACKER_SRC_MAIN_STRUCT_SPRITE_H_

#include <viewify/Struct/Rect.h>

using ii887522::viewify::Rect;

namespace ii887522::texturePacker {

/// <summary>It is a single part of a texture atlas.</summary>
struct Sprite final {
  unsigned int atlasIndex;
  Rect<int> rect;
  bool isRotated;

  explicit constexpr Sprite() { }
  explicit constexpr Sprite(const unsigned int atlasIndex, const Rect<int>& rect, const bool isRotated = false) : atlasIndex{ atlasIndex }, rect { rect }, isRotated{ isRotated } { }
};

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_STRUCT_SPRITE_H_
