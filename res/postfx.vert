#version 460 core

layout(location = 0) in vec2 vPosition;
layout(location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

layout(location = 0) uniform vec2 scale;

void main() {
  fTexCoord = vTexCoord * scale + vec2(0, 1);
  gl_Position = vec4(vPosition * scale + vec2(-1, 1), 0, 1);
}
