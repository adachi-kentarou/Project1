#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler;

// �|�X�g�G�t�F�N�g�f�[�^.
layout(std140) uniform PostEffectData
{
  mat4x4 matColor; // �F�ϊ��s��.
} postEffect;

void main()
{
  fragColor = texture(colorSampler, inTexCoord);
  //fragColor.rgb = vec3(dot(vec3(0.299, 0.587, 0.114), fragColor.rgb));
  //fragColor.rgb = vec3(dot(vec3(0.5, 0.5, 0.5), fragColor.rgb));

}