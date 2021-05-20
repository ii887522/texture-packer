// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_STRUCT_SPRITEROW_H_
#define TEXTURE_PACKER_SRC_MAIN_STRUCT_SPRITEROW_H_

#include <nitro/Struct/Range.h>

using ii887522::nitro::Range;

namespace ii887522::texturePacker {

struct SpriteRow final {
  Range<unsigned int> indices;
  int width;

  explicit constexpr SpriteRow() { }
  explicit constexpr SpriteRow(const Range<unsigned int>& indices, const int width) : indices{ indices }, width{ width } { }
};

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_STRUCT_SPRITEROW_H_
