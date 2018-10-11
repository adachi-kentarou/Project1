/**
* @file Texture.cpp
*/
#include "Texture.h"
#include <iostream>
#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

/**
* �o�C�g�񂩂琔�l�𕜌�����.
*
* @param p      �o�C�g��ւ̃|�C���^.
* @param offset ���l�̃I�t�Z�b�g.
* @param size   ���l�̃o�C�g��(1�`4).
*
* @return �����������l.
*/
uint32_t Get(const uint8_t* p, size_t offset, size_t size)
{
	uint32_t n = 0;
	p += offset;
	for (size_t i = 0; i < size; ++i) {
		n += p[i] << (i * 8);
	}
	return n;
}

/**
* �f�X�g���N�^.
*/
Texture::~Texture()
{
	if (texId) {
		glDeleteTextures(1, &texId);
	}
}

/**
* 2D�e�N�X�`�����쐬����.
*
* @param width   �e�N�X�`���̕�(�s�N�Z����).
* @param height  �e�N�X�`���̍���(�s�N�Z����).
* @param iformat �e�N�X�`���̃f�[�^�`��.
* @param format  �A�N�Z�X����v�f.
* @param data    �e�N�X�`���f�[�^�ւ̃|�C���^.
*
* @return �쐬�ɐ��������ꍇ�̓e�N�X�`���|�C���^��Ԃ�.
*         ���s�����ꍇ��nullptr�Ԃ�.
*/
TexturePtr Texture::Create(
	int width, int height, GLenum iformat, GLenum format, const void* data)
{
	struct Impl : Texture {};
	TexturePtr p = std::make_shared<Impl>();

	p->width = width;
	p->height = height;
	glGenTextures(1, &p->texId);
	glBindTexture(GL_TEXTURE_2D, p->texId);
	glTexImage2D(
		+GL_TEXTURE_2D, 0, iformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	const GLenum result = glGetError();
	if (result != GL_NO_ERROR) {
		std::cerr << "ERROR �e�N�X�`���쐬�Ɏ��s: 0x" << std::hex << result << std::endl;
		return {};
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return p;
}

/**
* �t�@�C������2D�e�N�X�`����ǂݍ���.
*
* @param filename �t�@�C����.
*
* @return �쐬�ɐ��������ꍇ�̓e�N�X�`���|�C���^��Ԃ�.
*         ���s�����ꍇ��nullptr�Ԃ�.
*/
TexturePtr Texture::LoadFromFile(const char* filename, const int c, const int v)
{
	//�t�@�C��������Ȃ��e�N�X�`���[��Ԃ�
	if (filename == "") {
		uint32_t textureData[] = { 
			0xffffffff,0xffffffff,
			0xffffffff,0xffffffff
		};
		return Texture::Create(2, 2, GL_RGBA8, GL_RGBA, textureData);
		//struct Impl : Texture {};
		//return std::make_shared<Impl>();
	}

	//�t�@�C���T�C�Y���擾����.
	struct stat st;
	if (stat(filename, &st)) {
		return {};
		
	}

	// �t�@�C���T�C�Y��BMP�t�@�C���̏���ێ��ł���T�C�Y�����Ȃ��̃I�u�W�F�N�g��Ԃ�.
	const size_t bmpFileHeaderSize = 14; // �r�b�g�}�b�v�t�@�C���w�b�_�̃o�C�g��
	const size_t windowsV1HeaderSize = 40; // �r�b�g�}�b�v���w�b�_�̃o�C�g��.
	if (st.st_size < bmpFileHeaderSize + windowsV1HeaderSize) {
		return {};
		
	}

	// �t�@�C�����J��.
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		return {};
		
	}

	// �t�@�C����ǂݍ���.
	std::vector<uint8_t> buf;
	buf.resize(st.st_size);
	const size_t readSize = fread(buf.data(), 1, st.st_size, fp);
	fclose(fp);
	if (readSize != st.st_size) {
		return {};
		
	}

	// BMP�t�@�C���łȂ���΋�̃I�u�W�F�N�g��Ԃ�.
	const uint8_t* pHeader = buf.data();
	if (pHeader[0] != 'B' || pHeader[1] != 'M') {
		return {};
		
	}

	// BMP�t�@�C���̏����擾����.
	const size_t offsetBytes = Get(pHeader, 10, 4);
	const uint32_t infoSize = Get(pHeader, 14, 4);
	const uint32_t width = Get(pHeader, 18, 4);
	const uint32_t height = Get(pHeader, 22, 4);
	const uint32_t bitCount = Get(pHeader, 28, 2);
	const uint32_t compression = Get(pHeader, 30, 4);
	const size_t pixelBytes = bitCount / 8;
	if (infoSize != windowsV1HeaderSize || bitCount != 24 || compression) {
		return {};
		
	}

	const size_t stride = ((width * pixelBytes + 3) / 4) * 4; // ���ۂ̉��o�C�g��.
	const size_t imageSize = stride * height;
	if (buf.size() < offsetBytes + imageSize) {
		return {};
		
	}

	//�w��F��臒l�͈͓��̃A���t�@��0�ɂ���
	if (c != -1) {
		uint32_t* textureData = new uint32_t[width * height];
		
		int cc[] = { 0, 3, 2, 1 };
		int d = cc[width * 3 % 4];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int base = (i * width + j) * 3 + (d * i);
				int b = *(buf.begin() + offsetBytes + base);
				int g = *(buf.begin() + offsetBytes + base + 1);
				int r = *(buf.begin() + offsetBytes + base + 2);

				int color = 0;

				//臒l��RGB
				int cb = c & 0xff;
				int cg = c >> 8 & 0xff;
				int cr = c >> 16 & 0xff;

				if (!(cr - v <= r && cr + v >= r &&
					cg - v <= g && cg + v >= g &&
					cb - v <= b && cb + v >= b
					)) {
					//�摜��臒l�ȓ��ł���΃A���t�@0�ɕύX
					color = 0xff << 24;
				}

				color += b << 16;
				color += g << 8;
				color += r;
				textureData[i * width + j] = color;
				
			}
		}
		
		return Texture::Create(width, height, GL_RGBA8, GL_RGBA, textureData);
		
	}
	
	
	return Create(width, height, GL_RGB8, GL_BGR, buf.data() + offsetBytes);
}