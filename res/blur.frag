#version 460 core

in vec2 fTexCoord;

out vec4 oColor;

layout(location = 1) uniform sampler2D tex;
layout(location = 2) uniform bool isHorizontal;

const float intensity = 2;

const float weights[] = float[](
  0.2023600146101466,
  0.1790438646174162,
  0.1240093299792275,
  0.06723453549491201,
  0.02853226260337099
);

vec4 blurHorizontally() {
  const float texOffset = 1 / float(textureSize(tex, 0).x);
  const vec4 texColor = texture(tex, fTexCoord);
  vec4 result = vec4(vec3(0), texColor.a * weights[0]);
  float maxAlpha = 0;
  float totalWeight = 0;
  if (texColor.a != 0) {
    result.rgb += texColor.rgb * weights[0];
    maxAlpha += texColor.a * weights[0];
    totalWeight += weights[0];
  }
  for (uint i = 1; i != weights.length(); ++i) {
    const vec4 leftTexColor = texture(tex, fTexCoord - vec2(texOffset * i, 0));
    if (leftTexColor.a != 0) {
      result.rgb += leftTexColor.rgb * weights[i];
      maxAlpha += leftTexColor.a * weights[i];
      totalWeight += weights[i];
    }
    result.a += leftTexColor.a * weights[i];
    const vec4 rightTexColor = texture(tex, fTexCoord + vec2(texOffset * i, 0));
    if (rightTexColor.a != 0) {
      result.rgb += rightTexColor.rgb * weights[i];
      maxAlpha += rightTexColor.a * weights[i];
      totalWeight += weights[i];
    }
    result.a += rightTexColor.a * weights[i];
  }
  return vec4(result.rgb / totalWeight, min(result.a * intensity, maxAlpha / totalWeight));
}

vec4 blurVertically() {
  const float texOffset = 1 / float(textureSize(tex, 0).y);
  const vec4 texColor = texture(tex, fTexCoord);
  vec4 result = vec4(vec3(0), texColor.a * weights[0]);
  float maxAlpha = 0;
  float totalWeight = 0;
  if (texColor.a != 0) {
    result.rgb += texColor.rgb * weights[0];
    maxAlpha += texColor.a * weights[0];
    totalWeight += weights[0];
  }
  for (uint i = 1; i != weights.length(); ++i) {
    const vec4 downTexColor = texture(tex, fTexCoord - vec2(0, texOffset * i));
    if (downTexColor.a != 0) {
      result.rgb += downTexColor.rgb * weights[i];
      maxAlpha += downTexColor.a * weights[i];
      totalWeight += weights[i];
    }
    result.a += downTexColor.a * weights[i];
    const vec4 upTexColor = texture(tex, fTexCoord + vec2(0, texOffset * i));
    if (upTexColor.a != 0) {
      result.rgb += upTexColor.rgb * weights[i];
      maxAlpha += upTexColor.a * weights[i];
      totalWeight += weights[i];
    }
    result.a += upTexColor.a * weights[i];
  }
  return vec4(result.rgb / totalWeight, min(result.a * intensity, maxAlpha / totalWeight));
}

void main() {
  if (isHorizontal) {
    oColor = blurHorizontally();
  } else {
    oColor = blurVertically();
  }
}
