#version 410

layout(location=0) in vec2 inTexCoord;
layout(location=1) in vec4 inColor;

out vec4 fragColor;

uniform sampler2D colorSampler;

const float edge = 0.4; ///< �t�H���g�̗֊s�Ƃ݂Ȃ�����.
const float smoothing = 1.0 / 16.0; ///< �֊s�����炩�ɂ��邽�߂̌W��.

void main()
{
  float distance = texture(colorSampler, inTexCoord).r;
  float alpha = smoothstep(edge - smoothing, edge + smoothing, 1 - distance);
  fragColor = vec4(inColor.rgb, inColor.a * alpha);
}