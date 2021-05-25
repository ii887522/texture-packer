// Copyright ii887522

#ifndef TEXTURE_PACKER_SRC_MAIN_VIEWGROUPFACTORY_TEXTUREPACKERVIEWGROUPFACTORY_H_
#define TEXTURE_PACKER_SRC_MAIN_VIEWGROUPFACTORY_TEXTUREPACKERVIEWGROUPFACTORY_H_

#ifndef TEST

#include <viewify/Factory/ViewGroupFactory.h>
#include <viewify/View/ViewGroup.h>
#include <viewify/Struct/Size.h>
#include <viewify/Any/QuadTree.h>
#include <viewify/Any/Enums.h>
#include <SDL.h>
#include <vector>
#include <string>
#include <functional>
#include "../Struct/Sprite.h"
#include "../Struct/SpriteRow.h"

using ii887522::viewify::ViewGroupFactory;
using ii887522::viewify::ViewGroup;
using ii887522::viewify::Size;
using ii887522::viewify::QuadTree;
using ii887522::nitro::Range;
using ii887522::viewify::Action;
using std::vector;
using std::string;
using std::function;

namespace ii887522::texturePacker {

// Texture Packer is a CLI tool used to generate texture atlases for graphics applications such as GUI, video games and so on to improve performance of these applications.
// Atlas is an image that contains multiple smaller sprites.
//
// Not Thread Safe: it must only be used in main thread
// See also viewify/View/ViewGroup.h for more details
// See also ../Struct/Sprite.h for more details
class TexturePackerViewGroupFactory final : public ViewGroupFactory {
  // remove copy semantics
  TexturePackerViewGroupFactory(const TexturePackerViewGroupFactory&) = delete;
  TexturePackerViewGroupFactory& operator=(const TexturePackerViewGroupFactory&) = delete;

  // remove move semantics
  TexturePackerViewGroupFactory(TexturePackerViewGroupFactory&&) = delete;
  TexturePackerViewGroupFactory& operator=(TexturePackerViewGroupFactory&&) = delete;

  SDL_Texture* atlas;
  const string outputDirPath;  // It ends with either '/' or '\\'
  vector<Sprite> sprites;  // See also ../Struct/Sprite.h for more details
  vector<SDL_Surface*> surfaces;
  vector<unsigned int> indices;
  vector<SpriteRow> spriteRows;  // See also ../Struct/Sprite.h for more details
  QuadTree spriteRects;  // See also ../Struct/Sprite.h for more details
  vector<unsigned int> lPendingIndices;
  vector<unsigned int> rPendingIndices;
  vector<unsigned int>* currentPendingIndices;
  vector<unsigned int>* nextPendingIndices;
  const int gap;
  unsigned int indicesI;

  // Atlas is an image that contains multiple smaller sprites.
  // See also ../Struct/Sprite.h for more details
  unsigned int atlasIndex;

  // Atlas is an image that contains multiple smaller sprites. It must only be called 1 time.
  // Param inputDirPath: it must exists and ends with either '/' or '\\'
  // See also ../Struct/Sprite.h for more details
  void addImages(const string& inputDirPath, const Size<int>& atlasSize);

  // Atlas is an image that contains multiple smaller sprites.
  // See also ../Struct/Sprite.h for more details
  void addImage(const string& filePath, const unsigned int index, const Size<int>& atlasSize);

  // It must only be called 1 time.
  void rotateImagesToMakeThemLonger();

  // Param compare: it returns true if the image needs to be rotated
  void rotateSomeImages(const vector<unsigned int>& pendingIndices, const function<bool(const unsigned int, const unsigned int)>& compare);

  // Atlas is an image that contains multiple smaller sprites.
  // See also ../Struct/Sprite.h for more details
  void linearlyLayOutSprites(const Size<int>& atlasSize);

  // See also ../Struct/Sprite.h for more details
  void pushUpSprites();

  // Param self: it must not be assigned to nullptr or integer
  // Param renderer: it must not be assigned to nullptr or integer
  // See also viewify/View/ViewGroup.h for more details
  // See also ../Struct/Sprite.h for more details
  void addImageViewsFromSpriteRows(ViewGroup*const self, SDL_Renderer*const renderer);

  // Atlas is an image that contains multiple smaller sprites.
  // See also ../Struct/Sprite.h for more details
  unsigned int getNearestSpriteRowToAtlasBottomRightCornerI(const Size<int>& atlasSize) const;

  // Atlas is an image that contains multiple smaller sprites.
  //
  // Param self: it must not be assigned to nullptr or integer
  // Param renderer: it must not be assigned to nullptr or integer
  // Param spriteRowsI: the atlas bottom region that below the sprite row referenced by this
  // See also viewify/Any/View.h for more details
  // See also ../Struct/Sprite.h for more details
  Action linearlyFillAtlasBottom(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize, const unsigned int spriteRowsI);

  // Atlas is an image that contains multiple smaller sprites.
  //
  // Param self: it must not be assigned to nullptr or integer
  // Param renderer: it must not be assigned to nullptr or integer
  // Param spriteRowsI: the atlas right region that above the bottom border of sprite row referenced by this
  // See also viewify/Any/View.h for more details
  // See also ../Struct/Sprite.h for more details
  Action linearlyFillAtlasRight(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize, const unsigned int spriteRowsI);

  vector<unsigned int> getIndicesReferencedByPendingIndices() const;

  // Atlas is an image that contains multiple smaller sprites.
  // See also ../Struct/Sprite.h for more details
  void prepareForNextAtlas();

  // L Shape represents the atlas bottom region and right region where they are not allocated to any sprites.
  // Atlas is an image that contains multiple smaller sprites.
  //
  // Param self: it must not be assigned to nullptr or integer
  // Param renderer: it must not be assigned to nullptr or integer
  // See also viewify/Any/View.h for more details
  // See also ../Struct/Sprite.h for more details
  Action fillLShape(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize);

 public:
  // Atlas is an image that contains multiple smaller sprites.
  //
  // Param inputDirPath: it must exists and ends with either '/' or '\\'
  // Param outputDirPath: it must ends with either '/' or '\\'
  // See also viewify/View/ViewGroup.h for more details
  // See also ../Struct/Sprite.h for more details
  explicit TexturePackerViewGroupFactory(const string& inputDirPath, const string& outputDirPath, const Size<int>& atlasSize);

  // Param renderer: it must not be assigned to nullptr or integer
  // See also viewify/View/ViewGroup.h for more details
  ViewGroup make(SDL_Renderer*const renderer, const Size<int>&) override;

  // See also viewify/View/ViewGroup.h for more details
  ~TexturePackerViewGroupFactory();
};

}  // namespace ii887522::texturePacker

#endif
#endif  // TEXTURE_PACKER_SRC_MAIN_VIEWGROUPFACTORY_TEXTUREPACKERVIEWGROUPFACTORY_H_
