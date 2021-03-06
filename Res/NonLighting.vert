#version 410

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexCoord;

layout(location=0) out vec4 outColor;
layout(location=1) out vec2 outTexCoord;

/**
* 頂点シェーダ入力.
*/
layout(std140) uniform VertexData
{
	mat4 matMVP;
	mat4 matModel;
	mat3x4 matNormal;
	vec4 color;
} vertexData;

void main()
{
  outColor = vColor * vertexData.color;
  outTexCoord = vTexCoord;
  gl_Position = vertexData.matMVP * vec4(vPosition, 1.0);
}