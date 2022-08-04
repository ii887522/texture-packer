#version 460 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

layout(location = 0) uniform vec2 size;
layout(location = 1) uniform vec2 atlasSize;
layout(location = 2) uniform vec2 translation;
layout(location = 3) uniform vec2 scale;
layout(location = 4) uniform vec2 texPosition;
layout(location = 5) uniform vec2 texSize;
layout(location = 6) uniform bool isRotated;

vec2 map(const vec2 value, const vec2 fromPosition, const vec2 fromSize, const vec2 toPosition, const vec2 toSize) {
  return (value - fromPosition) / fromSize * toSize + toPosition;
}

void main() {
  fTexCoord = map(
    (isRotated ? vec2(-vTexCoord.y + 1, vTexCoord.x) : vTexCoord) * texSize + texPosition,
    vec2(0),
    atlasSize,
    vec2(0, 1),
    vec2(1, -1)
  );
  gl_Position = vec4(map(vPosition * scale + translation, vec2(0), size, vec2(-1, 1), vec2(2, -2)), 0, 1);
}
