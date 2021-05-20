// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_
#define TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_

#include <viewify/Struct/Size.h>
#include <viewify/Struct/Rect.h>
#include <viewify/Struct/Point.h>
#include <string>
#include "../Struct/Sprite.h"

using std::string;
using ii887522::viewify::Size;
using ii887522::viewify::Rect;
using ii887522::viewify::Point;

namespace ii887522::texturePacker {

// Param inputDirPath: it must exists and ends with either '/' or '\\'
string getSpriteNameEnumFileContent(const string& inputDirPath);

// Param inputDirPath: it must exists and ends with either '/' or '\\'
// Param outputDirPath: it must ends with either '/' or '\\'
void writeSpriteNameEnumFile(const string& inputDirPath, const string& outputDirPath);

// Param sprite: it must not be assigned to nullptr or integer
// See also Sprite.h for more details
void rotate(Sprite*const sprite);

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_
