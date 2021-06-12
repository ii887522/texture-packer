// Copyright ii887522

#ifndef TEST

#include "TexturePackerViewGroupFactory.h"
#include <viewify/viewify.h>
#include <nitro/nitro.h>
#include <SDL.h>
#include <SDL_image.h>
#include <filesystem>
#include <string>
#include <vector>
#include <cstring>
#include <functional>
#include <stdexcept>
#include "../Struct/Sprite.h"
#include "../Functions/util.h"
#include "../Struct/SpriteRow.h"
#include "../Any/constants.h"

using ii887522::viewify::ViewGroupFactory;
using ii887522::viewify::ViewGroup;
using ii887522::viewify::Size;
using ii887522::viewify::Point;
using ii887522::viewify::Image;
using ii887522::viewify::Rect;
using ii887522::viewify::Align;
using ii887522::viewify::Rotation;
using ii887522::viewify::Action;
using ii887522::viewify::snapshot;
using ii887522::viewify::View;
using ii887522::nitro::sort;
using ii887522::nitro::write;
using ii887522::nitro::Range;
using ii887522::nitro::isOverlapX;
using ii887522::nitro::emptyDir;
using std::filesystem::directory_iterator;
using std::string;
using std::vector;
using std::to_string;
using std::function;
using std::invalid_argument;

namespace ii887522::texturePacker {

TexturePackerViewGroupFactory::TexturePackerViewGroupFactory(const string& inputDirPath, const string& outputDirPath, const Size<int>& atlasSize) : ViewGroupFactory{ }, atlas{ nullptr },
  outputDirPath{ outputDirPath }, spriteRects{ Rect{ Point{ 0.f, 0.f }, static_cast<Size<float>>(atlasSize) } }, currentPendingIndices{ &lPendingIndices },
  nextPendingIndices{ &rPendingIndices }, indicesI{ 0u }, atlasI{ 0u } {
  emptyDir(outputDirPath);
  addImages(inputDirPath, atlasSize);
  writeSpriteNameEnumFile(inputDirPath, outputDirPath);
  rotateImagesToMakeThemLonger();
  sort<unsigned int, vector>(&indices, [this](const unsigned int& l, const unsigned int& r) {  // NOLINT(build/include_what_you_use)
    return sprites[l].rect.size.h < sprites[r].rect.size.h;
  });
}

void TexturePackerViewGroupFactory::addImages(const string& inputDirPath, const Size<int>& atlasSize) {
  auto i{ 0u };
  for (const auto& entry : directory_iterator{ inputDirPath }) {
    if (!(entry.path().string().ends_with(LOWER_CASE_IMAGE_EXTENSION_NAME) || entry.path().string().ends_with(UPPER_CASE_IMAGE_EXTENSION_NAME))) continue;
    addImage(entry.path().string(), i, atlasSize);
    ++i;
  }
}

void TexturePackerViewGroupFactory::addImage(const string& filePath, const unsigned int i, const Size<int>& atlasSize) {
  surfaces.push_back(IMG_Load(filePath.c_str()));
  if (
    atlasSize.w < surfaces.back()->w + (GAP << 1u)  /* which means atlasSize.w < surfaces.back()->w + GAP * 2 */ ||
    atlasSize.h < surfaces.back()->h + (GAP << 1u)  /* which means atlasSize.h < surfaces.back()->h + GAP * 2 */)
    throw invalid_argument{"Atlas size must be big enough to fill a sprite!"};
  sprites.push_back(Sprite{ 0u, Rect{ Point{ 0, 0 }, Size{ surfaces.back()->w, surfaces.back()->h } } });
  indices.push_back(i);
}

void TexturePackerViewGroupFactory::rotateImagesToMakeThemLonger() {
  for (auto& sprite : sprites) {
    if (sprite.rect.size.w < sprite.rect.size.h) rotate(&sprite);
  }
}

void TexturePackerViewGroupFactory::rotateSomeImages(const vector<unsigned int>& pendingIndices, const function<bool(const unsigned int w, const unsigned int h)>& compare) {
  for (const auto i : pendingIndices) {
    if (compare(sprites[indices[i]].rect.size.w, sprites[indices[i]].rect.size.h)) rotate(&sprites[indices[i]]);
  }
}

void TexturePackerViewGroupFactory::linearlyLayOutSprites(const Size<int>& atlasSize) {
  Point<int> position{ GAP, GAP };
  auto rowH{ 0 };
  auto canReadRowH{ true };
  auto rowFirstSpriteI{ indicesI };
  for (; indicesI != indices.size(); ++indicesI) {
    if (position.x + sprites[indices[indicesI]].rect.size.w + GAP > atlasSize.w) {
      position.x = GAP;
      position.y += rowH + GAP;
      if (position.y + sprites[indices[indicesI]].rect.size.h + GAP > atlasSize.h) break;
      spriteRows.push_back(SpriteRow{ Range{ rowFirstSpriteI, indicesI - 1u }, sprites[indices[indicesI - 1u]].rect.position.x + sprites[indices[indicesI - 1u]].rect.size.w });
      rowFirstSpriteI = indicesI;
      canReadRowH = true;
    }
    if (canReadRowH) {
      rowH = sprites[indices[indicesI]].rect.size.h;
      canReadRowH = false;
    }
    sprites[indices[indicesI]].atlasI = atlasI;
    sprites[indices[indicesI]].rect.position.x = position.x;
    position.x += sprites[indices[indicesI]].rect.size.w + GAP;
  }
  spriteRows.push_back(SpriteRow{ Range{ rowFirstSpriteI, indicesI - 1u }, sprites[indices[indicesI - 1u]].rect.position.x + sprites[indices[indicesI - 1u]].rect.size.w });
}

void TexturePackerViewGroupFactory::pushUpSprites() {
  auto y{ GAP };
  for (auto i{ spriteRows.front().indices.min }; i <= spriteRows.front().indices.max; ++i) {
    sprites[indices[i]].rect.position.y = GAP;
    spriteRects.add(static_cast<Rect<float>>(sprites[indices[i]].rect));
  }
  y += sprites[indices[spriteRows.front().indices.min]].rect.size.h + GAP;
  for (auto i{ 1u }; i != spriteRows.size(); ++i) {
    auto aboveSpriteI{ spriteRows[i - 1u].indices.min };
    for (auto j{ spriteRows[i].indices.min }; j <= spriteRows[i].indices.max; ++j) {
      while (
        !isOverlapX(
          sprites[indices[j]].rect.position.x,
          Range{ sprites[indices[aboveSpriteI]].rect.position.x, sprites[indices[aboveSpriteI]].rect.position.x + sprites[indices[aboveSpriteI]].rect.size.w + GAP }))
        ++aboveSpriteI;
      sprites[indices[j]].rect.position.y = sprites[indices[aboveSpriteI]].rect.position.y + sprites[indices[aboveSpriteI]].rect.size.h + GAP;
      spriteRects.add(static_cast<Rect<float>>(sprites[indices[j]].rect));
    }
    y += sprites[indices[spriteRows[i].indices.min]].rect.size.h + GAP;
  }
}

void TexturePackerViewGroupFactory::addImageViewsFromSpriteRows(ViewGroup*const self, SDL_Renderer*const renderer) {
  for (auto i{ 0u }; i != spriteRows.size(); ++i) {
    for (auto j{ spriteRows[i].indices.min }; j <= spriteRows[i].indices.max; ++j) {
      self->add(
        Image::Builder{ renderer, surfaces[indices[j]], sprites[indices[j]].rect.position, Align::LEFT, sprites[indices[j]].isRotated ? Rotation::QUARTER_CLOCKWISE : Rotation::NONE }
          .setA(255u)
          .setDuration(1u)  // See also ii887522::viewify::Image::Builder::setDuration(const unsigned int) for more details
          .build());
    }
  }
}

unsigned int TexturePackerViewGroupFactory::getNearestSpriteRowToAtlasBottomRightCornerI(const Size<int>& atlasSize) const {
  auto lastDistanceSqr{ INT_MAX };
  auto spriteRowsI{ static_cast<int>(spriteRows.size() - 1u) };
  for (; spriteRowsI >= 0; --spriteRowsI) {
    const auto nowDistanceSqr{
      distanceSqr(
        Point{
          sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.position.x + sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.size.w - 1,
          sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.position.y + sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.size.h - 1
        }, Point{ atlasSize.w, atlasSize.h }
      )
    };
    if (nowDistanceSqr < lastDistanceSqr) lastDistanceSqr = nowDistanceSqr;
    else
      break;
  }
  return spriteRowsI + 1u;
}

Action TexturePackerViewGroupFactory::linearlyFillAtlasBottom(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize, const unsigned int spriteRowsI) {
  Point<int> position{ atlasSize.w - 1 - GAP, atlasSize.h - 1 - GAP };
  auto rowH{ 0 };
  auto canSetRowH{ true };
  bool canGoToNextRow;
  currentPendingIndices->resize(indices.size() - indicesI);
  for (auto i{ 0u }; i != currentPendingIndices->size(); ++i) (*currentPendingIndices)[i] = indicesI + i;
  do {
    canGoToNextRow = false;
    for (const auto i : *currentPendingIndices) {
      if (
        position.x - sprites[indices[i]].rect.size.w - GAP < -1 || spriteRects.isAnyRectHit(
          Rect{
            Point{ position.x - sprites[indices[i]].rect.size.w + 1.f - GAP, position.y - sprites[indices[i]].rect.size.h + 1.f - GAP },
            Size{ static_cast<float>(sprites[indices[i]].rect.size.w + GAP), static_cast<float>(sprites[indices[i]].rect.size.h + GAP) }
          }) || position.y - sprites[indices[i]].rect.size.h - GAP <
        sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.position.y + sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.size.h - 1
      ) {
        nextPendingIndices->push_back(i);
      } else {
        if (canSetRowH) {
          rowH = sprites[indices[i]].rect.size.h;
          canSetRowH = false;
        }
        sprites[indices[i]].atlasI = atlasI;
        sprites[indices[i]].rect.position = Point{ position.x - sprites[indices[i]].rect.size.w + 1, position.y - sprites[indices[i]].rect.size.h + 1 };
        self->add(
          Image::Builder{ renderer, surfaces[indices[i]], sprites[indices[i]].rect.position, Align::LEFT, sprites[indices[i]].isRotated ? Rotation::QUARTER_CLOCKWISE : Rotation::NONE }
            .setA(255u)
            .setDuration(1u)  // See also ii887522::viewify::Image::Builder::setDuration(const unsigned int) for more details
            .build());
        position.x -= sprites[indices[i]].rect.size.w + GAP;
        canGoToNextRow = true;
      }
    }
    position.x = atlasSize.w - 1 - GAP;
    position.y -= rowH + GAP;
    canSetRowH = true;
    currentPendingIndices->clear();
    ii887522::nitro::swap(currentPendingIndices, nextPendingIndices);
    if (currentPendingIndices->empty()) {
      indicesI = static_cast<unsigned int>(indices.size());
      return Action::RETURN_FROM_CALLER;
    }
  } while (canGoToNextRow);
  return Action::NONE;
}

Action TexturePackerViewGroupFactory::linearlyFillAtlasRight(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize, const unsigned int spriteRowsI) {
  Point<int> position{
    atlasSize.w - 1 - GAP, sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.position.y + sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.size.h - 1
  };
  auto colW{ 0 };
  auto canSetColW{ true };
  bool canGoToNextCol;
  do {
    canGoToNextCol = false;
    for (const auto i : *currentPendingIndices) {
      if (
        position.y - sprites[indices[i]].rect.size.h - GAP < -1 || spriteRects.isAnyRectHit(
          Rect{
            Point{ position.x - sprites[indices[i]].rect.size.w + 1.f - GAP, position.y - sprites[indices[i]].rect.size.h + 1.f - GAP },
            Size{ static_cast<float>(sprites[indices[i]].rect.size.w + GAP), static_cast<float>(sprites[indices[i]].rect.size.h + GAP) }
          })) {
        nextPendingIndices->push_back(i);
      } else {
        if (canSetColW) {
          colW = sprites[indices[i]].rect.size.w;
          canSetColW = false;
        }
        sprites[indices[i]].atlasI = atlasI;
        sprites[indices[i]].rect.position = Point{ position.x - sprites[indices[i]].rect.size.w + 1, position.y - sprites[indices[i]].rect.size.h + 1 };
        self->add(
          Image::Builder{ renderer, surfaces[indices[i]], sprites[indices[i]].rect.position, Align::LEFT, sprites[indices[i]].isRotated ? Rotation::QUARTER_CLOCKWISE : Rotation::NONE }
            .setA(255u)
            .setDuration(1u)  // See also ii887522::viewify::Image::Builder::setDuration(const unsigned int) for more details
            .build());
        position.y -= sprites[indices[i]].rect.size.h + GAP;
        canGoToNextCol = true;
      }
    }
    position.x -= colW + GAP;
    position.y = sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.position.y + sprites[indices[spriteRows[spriteRowsI].indices.max]].rect.size.h - 1;
    canSetColW = true;
    currentPendingIndices->clear();
    ii887522::nitro::swap(currentPendingIndices, nextPendingIndices);  // NOLINT(build/include_what_you_use)
    if (currentPendingIndices->empty()) {
      indicesI = static_cast<unsigned int>(indices.size());
      return Action::RETURN_FROM_CALLER;
    }
  } while (canGoToNextCol);
  return Action::NONE;
}

vector<unsigned int> TexturePackerViewGroupFactory::getIndicesReferencedByPendingIndices() const {
  vector<unsigned int> result(currentPendingIndices->size());
  for (auto i{ 0u }; i != currentPendingIndices->size(); ++i) result[i] = indices[(*currentPendingIndices)[i]];
  return result;
}

void TexturePackerViewGroupFactory::prepareForNextAtlas() {
  auto aux{ getIndicesReferencedByPendingIndices() };
  sort<unsigned int, vector>(&aux, [this](const unsigned int& l, const unsigned int& r) {
    return sprites[l].rect.size.h < sprites[r].rect.size.h;
  });
  indices.resize(aux.size());
  memcpy(indices.data(), aux.data(), aux.size() * sizeof(unsigned int));
  indicesI = 0u;
}

Action TexturePackerViewGroupFactory::fillLShape(ViewGroup*const self, SDL_Renderer*const renderer, const Size<int>& atlasSize) {
  const auto spriteRowsI{ getNearestSpriteRowToAtlasBottomRightCornerI(atlasSize) };
  if (linearlyFillAtlasBottom(self, renderer, atlasSize, spriteRowsI) == Action::RETURN_FROM_CALLER) return Action::RETURN_FROM_CALLER;
  rotateSomeImages(*currentPendingIndices, [](const unsigned int w, const unsigned int h) {
    return w > h;
  });
  sort<unsigned int, vector>(currentPendingIndices, [this](const unsigned int& l, const unsigned int& r) {
    return sprites[indices[l]].rect.size.w < sprites[indices[r]].rect.size.w;
  });
  if (linearlyFillAtlasRight(self, renderer, atlasSize, spriteRowsI) == Action::RETURN_FROM_CALLER) return Action::RETURN_FROM_CALLER;
  rotateSomeImages(*currentPendingIndices, [](const unsigned int w, const unsigned int h) {
    return w < h;
  });
  return Action::NONE;
}

ViewGroup TexturePackerViewGroupFactory::make(SDL_Renderer*const renderer, const Size<int>& size) {
  atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.w, size.h);
  SDL_SetRenderTarget(renderer, atlas);
  return ViewGroup{ renderer, Point{ 0, 0 }, [](ViewGroup*const, SDL_Renderer*const) {
    return vector<View*>{ };
  }, [this, renderer, size](ViewGroup*const self) {
    linearlyLayOutSprites(size);
    sort<SpriteRow, vector>(&spriteRows, [](const SpriteRow& l, const SpriteRow& r) {  // NOLINT(build/include_what_you_use)
      return l.width < r.width;
    });
    pushUpSprites();
    addImageViewsFromSpriteRows(self, renderer);
    if (indicesI == indices.size()) return Action::NONE;
    if (fillLShape(self, renderer, size) == Action::RETURN_FROM_CALLER) return Action::NONE;
    prepareForNextAtlas();
    return Action::NONE;
  }, [this, renderer, size](ViewGroup*const self) {
    snapshot(renderer, Rect{ Point{ 0, 0 }, size }, outputDirPath + "atlas_" + to_string(atlasI) + LOWER_CASE_IMAGE_EXTENSION_NAME);
    self->clear();
    spriteRows.clear();
    spriteRects.clear();
    ++atlasI;
    return indicesI == indices.size() ? Action::QUIT : Action::NONE;
  } };
}

TexturePackerViewGroupFactory::~TexturePackerViewGroupFactory() {
  write<Sprite, vector>(outputDirPath + "atlas.dat", sprites);
  SDL_DestroyTexture(atlas);
}

}  // namespace ii887522::texturePacker

#endif
