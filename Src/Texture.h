/**
* @file Texture.h
*/
#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED
#include <GL/glew.h>
#include <memory>

class Texture;
using TexturePtr = std::shared_ptr<Texture>; ///< テクスチャポインタ.

											 /**
											 * テクスチャクラス.
											 */
class Texture
{
public:
	static TexturePtr Create(
		int width, int height, GLenum iformat, GLenum format, const void* data);

	static TexturePtr LoadFromFile(const char*, const int c = -1, const int v = 0);

	GLuint Id() const { return texId; }
	GLsizei Width() const { return width; }
	GLsizei Height() const { return height; }

private:
	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	GLuint texId = 0;
	int width = 0;
	int height = 0;
};

#endif