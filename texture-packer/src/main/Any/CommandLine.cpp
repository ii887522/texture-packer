// Copyright ii887522

#include "CommandLine.h"
#include <nitro/Functions/math_ext.h>
#include <string>
#include <stdexcept>
#include <filesystem>
#include "constants.h"  // NOLINT(build/include_subdir)
#include <nitro/Functions/fs_ext.h>

using std::string;
using std::invalid_argument;
using std::filesystem::exists;
using ii887522::nitro::isUint;
using ii887522::nitro::parseUint;
using ii887522::nitro::isPowerOfTwo;
using ii887522::nitro::hasFileWithExtension;

namespace ii887522::texturePacker {

CommandLine::CommandLine(int argc, char** argv) {
  if (argc != 5u) throw invalid_argument{ "There must be only 4 command line arguments!" };
  inputDirPath = argv[INPUT_DIRECTORY_PATH_INDEX];
  outputDirPath = argv[OUTPUT_DIRECTORY_PATH_INDEX];
  if (!(isUint(string{ argv[ATLAS_WIDTH_INDEX] }) && isUint(string{ argv[ATLAS_HEIGHT_INDEX] }))) throw invalid_argument{ "Atlas width and atlas height must be unsigned integers!" };
  atlasSize = Size{ static_cast<int>(parseUint(string{ argv[ATLAS_WIDTH_INDEX] })), static_cast<int>(parseUint(string{ argv[ATLAS_HEIGHT_INDEX] })) };
}

const string& CommandLine::getInputDirPath() const {
  return inputDirPath;
}

const string& CommandLine::getOutputDirPath() const {
  return outputDirPath;
}

void CommandLine::validate() const {
  if (!((inputDirPath.ends_with('/') || inputDirPath.ends_with('\\')) && exists(inputDirPath) && (hasFileWithExtension(inputDirPath, ".png") || hasFileWithExtension(inputDirPath, ".PNG"))))
    throw invalid_argument{ "Input directory path must exists, has at least 1 png file and ends with either '/' or '\\'!" };
  if (!(outputDirPath.ends_with('/') || outputDirPath.ends_with('\\'))) throw invalid_argument{ "Output directory path must ends with either '/' or '\\'!" };
  if (!(isPowerOfTwo(atlasSize.w) && isPowerOfTwo(atlasSize.h))) throw invalid_argument{ "Atlas width and atlas height must be power of two!" };
}

}  // namespace ii887522::texturePacker
