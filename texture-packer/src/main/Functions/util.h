// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_
#define TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_

#include <string>
#include "../Struct/Sprite.h"

using std::string;

namespace ii887522::texturePacker {

// Param inputDirPath: it must exists and ends with either '/' or '\\'
string getSpriteNameEnumFileContent(const string& inputDirPath);

// Param inputDirPath: it must exists and ends with either '/' or '\\'
// Param outputDirPath: it must ends with either '/' or '\\'
void writeSpriteNameEnumFile(const string& inputDirPath, const string& outputDirPath);

// Param sprite: it must not be assigned to nullptr or integer
void rotate(Sprite*const sprite);

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_FUNCTIONS_UTIL_H_
