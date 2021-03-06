#version 410

layout(location=0) in vec2 inTexCoord;
layout(location=1) in vec4 inColor;

out vec4 fragColor;

uniform sampler2D colorSampler;

const float edge = 0.4; ///< フォントの輪郭とみなす距離.
const float smoothing = 1.0 / 16.0; ///< 輪郭を滑らかにするための係数.

void main()
{
  float distance = texture(colorSampler, inTexCoord).r;
  float alpha = smoothstep(edge - smoothing, edge + smoothing, 1 - distance);
  fragColor = vec4(inColor.rgb, inColor.a * alpha);
}