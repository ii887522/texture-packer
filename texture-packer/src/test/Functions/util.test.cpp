// Copyright ii887522

#ifdef TEST

#include "util.test.h"
#include <cassert>
#include <vector>
#include <Struct/Range.h>
#include <Struct/Rect.h>
#include <Struct/Point.h>
#include <Struct/Size.h>
#include "../../main/Functions/util.h"
#include "../../main/Struct/Sprite.h"

using std::vector;
using ii887522::nitro::Range;
using ii887522::viewify::Rect;
using ii887522::viewify::Point;
using ii887522::viewify::Size;

namespace ii887522::texturePacker {

void testRotate() {
  {
    Sprite sprite{ 0u, Rect{ Point{ 0, 0 }, Size{ 1, 1 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 0u);
    assert(sprite.rect.position.x == 0);
    assert(sprite.rect.position.y == 0);
    assert(sprite.rect.size.w == 1);
    assert(sprite.rect.size.h == 1);
    assert(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 0 }, Size{ 1, 1 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 0u);
    assert(sprite.rect.position.x == 1);
    assert(sprite.rect.position.y == 0);
    assert(sprite.rect.size.w == 1);
    assert(sprite.rect.size.h == 1);
    assert(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 1, 1 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 0u);
    assert(sprite.rect.position.x == 1);
    assert(sprite.rect.position.y == 1);
    assert(sprite.rect.size.w == 1);
    assert(sprite.rect.size.h == 1);
    assert(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 2, 1 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 0u);
    assert(sprite.rect.position.x == 1);
    assert(sprite.rect.position.y == 1);
    assert(sprite.rect.size.w == 1);
    assert(sprite.rect.size.h == 2);
    assert(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 1, 2 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 0u);
    assert(sprite.rect.position.x == 1);
    assert(sprite.rect.position.y == 1);
    assert(sprite.rect.size.w == 2);
    assert(sprite.rect.size.h == 1);
    assert(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 2, 2 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 0u);
    assert(sprite.rect.position.x == 1);
    assert(sprite.rect.position.y == 1);
    assert(sprite.rect.size.w == 2);
    assert(sprite.rect.size.h == 2);
    assert(sprite.isRotated);
  }
  {
    Sprite sprite{ 1u, Rect{ Point{ 1, 1 }, Size{ 2, 2 } }, false };
    rotate(sprite);
    assert(sprite.atlasIndex == 1u);
    assert(sprite.rect.position.x == 1);
    assert(sprite.rect.position.y == 1);
    assert(sprite.rect.size.w == 2);
    assert(sprite.rect.size.h == 2);
    assert(sprite.isRotated);
  }
}

void testUtil() {
  testRotate();
}

}  // namespace ii887522::texturePacker

#endif
