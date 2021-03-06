// Copyright ii887522

#include "util.h"  // NOLINT(build/include_subdir)
#include <nitro/nitro.h>
#include <string>
#include <filesystem>
#include <vector>
#include "../Struct/Sprite.h"
#include "../Any/constants.h"

using std::string;
using std::filesystem::directory_iterator;
using std::vector;
using ii887522::nitro::getFileName;
using ii887522::nitro::toUpperCase;
using ii887522::nitro::write;
using ii887522::nitro::swap;

namespace ii887522::texturePacker {

string getSpriteNameEnumFileContent(const string& inputDirPath) {
  string content{
R"(// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_ANY_SPRITE_NAME_H_
#define TEXTURE_PACKER_SRC_MAIN_ANY_SPRITE_NAME_H_

namespace ii887522::texturePacker {

enum class SpriteName : unsigned int {
)"
  };
  for (const auto& entry : directory_iterator{ inputDirPath }) {
    if (!(entry.path().string().ends_with(LOWER_CASE_IMAGE_EXTENSION_NAME) || entry.path().string().ends_with(UPPER_CASE_IMAGE_EXTENSION_NAME))) continue;
    auto spriteName{ getFileName(entry.path().string()) };
    toUpperCase(&spriteName);
    content += "\t_" + spriteName + ",\n";
  }
  return content +
R"(};

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_ANY_SPRITE_NAME_H_
)";
}

void writeSpriteNameEnumFile(const string& inputDirPath, const string& outputDirPath) {
  const auto content{ getSpriteNameEnumFileContent(inputDirPath) };
  write<char, vector>(outputDirPath + "SpriteName.h", vector(content.begin(), content.end()));
}

void rotate(Sprite*const sprite) {
  swap(sprite->rect.size.w, sprite->rect.size.h);  // NOLINT(build/include_what_you_use)
  sprite->isRotated = !sprite->isRotated;
}

}  // namespace ii887522::texturePacker
