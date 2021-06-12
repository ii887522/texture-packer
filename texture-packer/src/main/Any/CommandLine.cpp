// Copyright ii887522

#include "CommandLine.h"
#include <nitro/nitro.h>
#include <string>
#include <stdexcept>
#include <filesystem>
#include "constants.h"  // NOLINT(build/include_subdir)

using std::string;
using std::invalid_argument;
using std::filesystem::exists;
using ii887522::nitro::isUint;
using ii887522::nitro::parseUint;
using ii887522::nitro::isPowerOfTwo;
using ii887522::nitro::hasFileWithExtension;

namespace ii887522::texturePacker {

CommandLine::CommandLine(int argc, char** argv) {
  if (argc != COMMAND_LINE_ARGS_REQUIRED + 1u) throw invalid_argument{ "There must be only " + string { COMMAND_LINE_ARGS_REQUIRED } + " command line arguments!" };
  inputDirPath = argv[INPUT_DIRECTORY_PATH_I];
  outputDirPath = argv[OUTPUT_DIRECTORY_PATH_I];
  if (!(isUint(string{ argv[ATLAS_WIDTH_I] }) && isUint(string{ argv[ATLAS_HEIGHT_I] }))) throw invalid_argument{ "Atlas width and atlas height must be unsigned integers!" };
  atlasSize = Size{ static_cast<int>(parseUint(string{ argv[ATLAS_WIDTH_I] })), static_cast<int>(parseUint(string{ argv[ATLAS_HEIGHT_I] })) };
}

const string& CommandLine::getInputDirPath() const {
  return inputDirPath;
}

const string& CommandLine::getOutputDirPath() const {
  return outputDirPath;
}

void CommandLine::validate() const {
  if (!((inputDirPath.ends_with('/') || inputDirPath.ends_with('\\')) && exists(inputDirPath) && (hasFileWithExtension(inputDirPath, LOWER_CASE_IMAGE_EXTENSION_NAME) ||
    hasFileWithExtension(inputDirPath, UPPER_CASE_IMAGE_EXTENSION_NAME))))
    throw invalid_argument{ "Input directory path must exists, has at least 1 png file and ends with either '/' or '\\'!" };
  if (!(outputDirPath.ends_with('/') || outputDirPath.ends_with('\\'))) throw invalid_argument{ "Output directory path must ends with either '/' or '\\'!" };
  if (!(isPowerOfTwo(atlasSize.w) && isPowerOfTwo(atlasSize.h))) throw invalid_argument{ "Atlas width and atlas height must be power of two!" };
}

}  // namespace ii887522::texturePacker
