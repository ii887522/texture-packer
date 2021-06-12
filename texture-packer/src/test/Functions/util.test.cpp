// Copyright ii887522

#ifdef TEST

#include <nitro/nitro.h>
#include <viewify/viewify.h>
#include <vector>
#include <catch.hpp>
#include "../../main/Functions/util.h"
#include "../../main/Struct/Sprite.h"

using std::vector;
using ii887522::nitro::Range;
using ii887522::viewify::Rect;
using ii887522::viewify::Point;
using ii887522::viewify::Size;

namespace ii887522::texturePacker {

TEST_CASE("test rotate() function") {
  {
    Sprite sprite{ 0u, Rect{ Point{ 0, 0 }, Size{ 1, 1 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 0u);
    REQUIRE(sprite.rect.position.x == 0);
    REQUIRE(sprite.rect.position.y == 0);
    REQUIRE(sprite.rect.size.w == 1);
    REQUIRE(sprite.rect.size.h == 1);
    REQUIRE(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 0 }, Size{ 1, 1 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 0u);
    REQUIRE(sprite.rect.position.x == 1);
    REQUIRE(sprite.rect.position.y == 0);
    REQUIRE(sprite.rect.size.w == 1);
    REQUIRE(sprite.rect.size.h == 1);
    REQUIRE(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 1, 1 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 0u);
    REQUIRE(sprite.rect.position.x == 1);
    REQUIRE(sprite.rect.position.y == 1);
    REQUIRE(sprite.rect.size.w == 1);
    REQUIRE(sprite.rect.size.h == 1);
    REQUIRE(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 2, 1 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 0u);
    REQUIRE(sprite.rect.position.x == 1);
    REQUIRE(sprite.rect.position.y == 1);
    REQUIRE(sprite.rect.size.w == 1);
    REQUIRE(sprite.rect.size.h == 2);
    REQUIRE(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 1, 2 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 0u);
    REQUIRE(sprite.rect.position.x == 1);
    REQUIRE(sprite.rect.position.y == 1);
    REQUIRE(sprite.rect.size.w == 2);
    REQUIRE(sprite.rect.size.h == 1);
    REQUIRE(sprite.isRotated);
  }
  {
    Sprite sprite{ 0u, Rect{ Point{ 1, 1 }, Size{ 2, 2 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 0u);
    REQUIRE(sprite.rect.position.x == 1);
    REQUIRE(sprite.rect.position.y == 1);
    REQUIRE(sprite.rect.size.w == 2);
    REQUIRE(sprite.rect.size.h == 2);
    REQUIRE(sprite.isRotated);
  }
  {
    Sprite sprite{ 1u, Rect{ Point{ 1, 1 }, Size{ 2, 2 } }, false };
    rotate(&sprite);
    REQUIRE(sprite.atlasI == 1u);
    REQUIRE(sprite.rect.position.x == 1);
    REQUIRE(sprite.rect.position.y == 1);
    REQUIRE(sprite.rect.size.w == 2);
    REQUIRE(sprite.rect.size.h == 2);
    REQUIRE(sprite.isRotated);
  }
}

}  // namespace ii887522::texturePacker

#endif
