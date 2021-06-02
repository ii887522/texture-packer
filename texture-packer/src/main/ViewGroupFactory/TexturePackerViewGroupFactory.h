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

constexpr auto GAP{ 0 };

/// <summary>
///   <para>Texture Packer is a CLI tool used to generate texture atlases for graphics applications such as GUI, video games and so on to improve performance of these applications.</para>
///   <para>Atlas is an image that contains multiple smaller sprites.</para>
///   <para>Not Thread Safe: it must only be used in main thread</para>
///   <para>See also viewify/View/ViewGroup.h for more details</para>
///   <para>See also ../Struct/Sprite.h for more details</para>
/// </summary>
class TexturePackerViewGroupFactory final : public ViewGroupFactory {
  // remove copy semantics
  TexturePackerViewGroupFactory(const TexturePackerViewGroupFactory&) = delete;
  TexturePackerViewGroupFactory& operator=(const TexturePackerViewGroupFactory&) = delete;

  // remove move semantics
  TexturePackerViewGroupFactory(TexturePackerViewGroupFactory&&) = delete;
  TexturePackerViewGroupFactory& operator=(TexturePackerViewGroupFactory&&) = delete;

  SDL_Texture* atlas;

  /// <summary>It ends with either '/' or '\\'</summary>
  const string outputDirPath;

  /// <summary>See also ../Struct/Sprite.h for more details</summary>
  vector<Sprite> sprites;

  vector<SDL_Surface*> surfaces;
  vector<unsigned int> indices;

  /// <summary>See also ../Struct/Sprite.h for more details</summary>
  vector<SpriteRow> spriteRows;

  /// <summary>See also ../Struct/Sprite.h for more details</summary>
  QuadTree spriteRects;

  vector<unsigned int> lPendingIndices;
  vector<unsigned int> rPendingIndices;
  vector<unsigned int>* currentPendingIndices;
  vector<unsigned int>* nextPendingIndices;
  unsigned int indicesI;

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  unsigned int atlasI;

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites. It must only be called 1 time.</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  /// <param name="inputDirPath">It must exists and ends with either '/' or '\\'</param>
  void addImages(const string& inputDirPath, const Size<int>& atlasSize);

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  void addImage(const string& filePath, const unsigned int i, const Size<int>& atlasSize);

  /// <summary>It must only be called 1 time.</summary>
  void rotateImagesToMakeThemLonger();

  /// <param name="compare">It returns true if the image needs to be rotated</param>
  void rotateSomeImages(const vector<unsigned int>& pendingIndices, const function<bool(const unsigned int w, const unsigned int h)>& compare);

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  void linearlyLayOutSprites(const Size<int>& atlasSize);

  /// <summary>See also ../Struct/Sprite.h for more details</summary>
  void pushUpSprites();

  /// <summary>
  ///   <para>See also viewify/View/ViewGroup.h for more details</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  /// <param name="self">It must not be assigned to nullptr or integer</param>
  /// <param name="renderer">It must not be assigned to nullptr or integer</param>
  void addImageViewsFromSpriteRows(ViewGroup*const self, SDL_Renderer*const renderer);

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  unsigned int getNearestSpriteRowToAtlasBottomRightCornerI(const Size<int>& atlasSize) const;

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also viewify/Any/View.h for more details</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  /// <param name="self">It must not be assigned to nullptr or integer</param>
  /// <param name="renderer">It must not be assigned to nullptr or integer</param>
  /// <param name="spriteRowsI">The atlas bottom region that below the sprite row referenced by this</param>
  Action linearlyFillAtlasBottom(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize, const unsigned int spriteRowsI);

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also viewify/Any/View.h for more details</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  /// <param name="self">It must not be assigned to nullptr or integer</param>
  /// <param name="renderer">It must not be assigned to nullptr or integer</param>
  /// <param name="spriteRowsI">The atlas right region that above the bottom border of sprite row referenced by this</param>
  Action linearlyFillAtlasRight(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize, const unsigned int spriteRowsI);

  vector<unsigned int> getIndicesReferencedByPendingIndices() const;

  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  void prepareForNextAtlas();

  /// <summary>
  ///   <para>L Shape represents the atlas bottom region and right region where they are not allocated to any sprites.</para>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also viewify/Any/View.h for more details</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  /// <param name="self">It must not be assigned to nullptr or integer</param>
  /// <param name="renderer">It must not be assigned to nullptr or integer</param>
  Action fillLShape(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize);

 public:
  /// <summary>
  ///   <para>Atlas is an image that contains multiple smaller sprites.</para>
  ///   <para>See also viewify/View/ViewGroup.h for more details</para>
  ///   <para>See also ../Struct/Sprite.h for more details</para>
  /// </summary>
  /// <param name="inputDirPath">It must exists and ends with either '/' or '\\'</param>
  /// <param name="outputDirPath">It must ends with either '/' or '\\'</param>
  explicit TexturePackerViewGroupFactory(const string& inputDirPath, const string& outputDirPath, const Size<int>& atlasSize);

  /// <summary>See also viewify/View/ViewGroup.h for more details</summary>
  /// <param name="renderer">It must not be assigned to nullptr or integer</param>
  ViewGroup make(SDL_Renderer*const renderer, const Size<int>&) override;

  /// <summary>See also viewify/View/ViewGroup.h for more details</summary>
  ~TexturePackerViewGroupFactory();
};

}  // namespace ii887522::texturePacker

#endif
#endif  // TEXTURE_PACKER_SRC_MAIN_VIEWGROUPFACTORY_TEXTUREPACKERVIEWGROUPFACTORY_H_
