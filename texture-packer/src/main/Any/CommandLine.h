// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_ANY_COMMANDLINE_H_
#define TEXTURE_PACKER_SRC_MAIN_ANY_COMMANDLINE_H_

#include <Struct/Size.h>
#include <string>

using ii887522::viewify::Size;
using std::string;

namespace ii887522::texturePacker {

// Not Thread Safe
class CommandLine final {
  // remove copy semantics
  CommandLine(const CommandLine&) = delete;
  CommandLine& operator=(const CommandLine&) = delete;

  // remove move semantics
  CommandLine(CommandLine&&) = delete;
  CommandLine& operator=(CommandLine&&) = delete;

  string inputDirPath;  // It ends with either '/' or '\\'
  string outputDirPath;  // It ends with either '/' or '\\'
  Size<int> atlasSize;  // See also TextureAtlas.h for more details

 public:
  explicit CommandLine(int argc, char** argv);

  // Return: it ends with either '/' or '\\'
  const string& getInputDirPath() const;

  // Return: it ends with either '/' or '\\'
  const string& getOutputDirPath() const;

  // See also TextureAtlas.h for more details
  constexpr const Size<int>& getAtlasSize() const {
    return atlasSize;
  }

  void validate() const;
};

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_ANY_COMMANDLINE_H_
