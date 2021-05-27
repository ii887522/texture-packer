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

/// <param name="inputDirPath">It must exists and ends with either '/' or '\\'</param>
string getSpriteNameEnumFileContent(const string& inputDirPath);

/// <param name="inputDirPath">It must exists and ends with either '/' or '\\'</param>
/// <param name="outputDirPath">It must ends with either '/' or '\\'</param>
void writeSpriteNameEnumFile(const string& inputDirPath, const string& outputDirPath);

/// <summary>See also Sprite.h for more details</summary>
/// <param name="sprite">It must not be assigned to nullptr or integer</param>
void rotate(Sprite*const sprite);

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_
