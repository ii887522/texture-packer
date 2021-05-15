// Copyright ii887522

#include "CommandLine.h"
#include <Functions/math_ext.h>
#include <string>
#include <stdexcept>
#include <filesystem>

using std::string;
using std::invalid_argument;
using std::filesystem::exists;
using ii887522::nitro::isUint;
using ii887522::nitro::parseUint;
using ii887522::nitro::isPowerOfTwo;

namespace ii887522::texturePacker {

CommandLine::CommandLine(int argc, char** argv) {
  if (argc != 5u) throw invalid_argument{ "There must be only 4 command line arguments!" };
  inputDirPath = argv[1u];
  outputDirPath = argv[2u];
  if (!(isUint(string{ argv[3u] }) && isUint(string{ argv[4u] }))) throw invalid_argument{ "Atlas width and atlas height must be unsigned integers!" };
  atlasSize = Size{ static_cast<int>(parseUint(string{ argv[3u] })), static_cast<int>(parseUint(string{ argv[4u] })) };
}

const string& CommandLine::getInputDirPath() const {
  return inputDirPath;
}

const string& CommandLine::getOutputDirPath() const {
  return outputDirPath;
}

void CommandLine::validate() const {
  if (!((inputDirPath.ends_with('/') || inputDirPath.ends_with('\\')) && exists(inputDirPath)))
    throw invalid_argument{ "Input directory path must exists and ends with either '/' or '\\'!" };
  if (!(outputDirPath.ends_with('/') || outputDirPath.ends_with('\\'))) throw invalid_argument{ "Output directory path must ends with either '/' or '\\'!" };
  if (!(isPowerOfTwo(atlasSize.w) && isPowerOfTwo(atlasSize.h))) throw invalid_argument{ "Atlas width and atlas height must be power of two!" };
}

}  // namespace ii887522::texturePacker
