#version 460 core

in vec2 fTexCoord;

out vec4 oColor;

layout(location = 7) uniform sampler2D tex;

void main() {
  oColor = texture(tex, fTexCoord);
}
