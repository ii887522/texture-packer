// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_ANY_COMMANDLINE_H_
#define TEXTURE_PACKER_SRC_MAIN_ANY_COMMANDLINE_H_

#include <viewify/Struct/Size.h>
#include <string>

using ii887522::viewify::Size;
using std::string;

namespace ii887522::texturePacker {

/// <summary>Not Thread Safe</summary>
class CommandLine final {
  // remove copy semantics
  CommandLine(const CommandLine&) = delete;
  CommandLine& operator=(const CommandLine&) = delete;

  // remove move semantics
  CommandLine(CommandLine&&) = delete;
  CommandLine& operator=(CommandLine&&) = delete;

  /// <summary>It ends with either '/' or '\\'</summary>
  string inputDirPath;

  /// <summary>It ends with either '/' or '\\'</summary>
  string outputDirPath;

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also ../Struct/Sprite.h for more details.</para>
  /// </summary>
  Size<int> atlasSize;

 public:
  explicit CommandLine(int argc, char** argv);

  /// <returns>It ends with either '/' or '\\'</returns>
  const string& getInputDirPath() const;

  /// <returns>it ends with either '/' or '\\'</returns>
  const string& getOutputDirPath() const;

  /// <summary>See also TextureAtlas.h for more details</summary>
  constexpr const Size<int>& getAtlasSize() const {
    return atlasSize;
  }

  void validate() const;
};

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_ANY_COMMANDLINE_H_
