// Copyright ii887522

#ifndef TEST

#ifndef TEXTURE_PACKER_SRC_MAIN_VIEW_GROUP_FACTORY_TEXTURE_PACKER_VIEW_GROUP_FACTORY_H_
#define TEXTURE_PACKER_SRC_MAIN_VIEW_GROUP_FACTORY_TEXTURE_PACKER_VIEW_GROUP_FACTORY_H_

#include <Factory/ViewGroupFactory.h>
#include <View/ViewGroup.h>
#include <SDL.h>
#include <Struct/Size.h>
#include "../Struct/Sprite.h"
#include <vector>
#include <string>

using ii887522::viewify::ViewGroupFactory;
using ii887522::viewify::ViewGroup;
using ii887522::viewify::Size;
using std::vector;
using std::string;

namespace ii887522::texturePacker {

// Texture Packer is a CLI tool used to generate texture atlases for graphics applications such as GUI, video games and so on to improve performance of these applications.
// 
// Not Thread Safe: it must only be used in main thread
// See also View/ViewGroup.h for more details
// See also TextureAtlas.h for more details
class TexturePackerViewGroupFactory final : public ViewGroupFactory {
  // remove copy semantics
  TexturePackerViewGroupFactory(const TexturePackerViewGroupFactory&) = delete;
  TexturePackerViewGroupFactory& operator=(const TexturePackerViewGroupFactory&) = delete;

  // remove move semantics
  TexturePackerViewGroupFactory(TexturePackerViewGroupFactory&&) = delete;
  TexturePackerViewGroupFactory& operator=(TexturePackerViewGroupFactory&&) = delete;

  const string outputDirPath;  // It ends with either '/' or '\\'
  vector<Sprite> sprites;  // See also Sprite.h for more details
  vector<SDL_Surface*> surfaces;
  vector<unsigned int> indices;
  unsigned int indicesI;
  unsigned int atlasIndex;

  // Param inputDirPath: it must exists and ends with either '/' or '\\'
  void addImages(const string& inputDirPath);

  void addImage(const string& filePath, const unsigned int index);
  void rotateSomeImages();

 public:
  // Param inputDirPath: it must exists and ends with either '/' or '\\'
  // Param outputDirPath: it must ends with either '/' or '\\'
  // See also View/ViewGroup.h for more details
  explicit TexturePackerViewGroupFactory(const string& inputDirPath, const string& outputDirPath);

  // Param renderer: it must not be assigned to nullptr or integer
  // See also View/ViewGroup.h for more details
  ViewGroup make(SDL_Renderer*const renderer, const Size<int>&) override;
};

}  // namespace ii887522::texturePacker

#endif  // TEXTURE_PACKER_SRC_MAIN_VIEW_GROUP_FACTORY_TEXTURE_PACKER_VIEW_GROUP_FACTORY_H_
#endif
