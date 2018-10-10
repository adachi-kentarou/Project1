/**
* @file Mesh.cpp
*/
#include "Mesh.h"
//#include <fbxsdk.h>
#include <iostream>

/**
* ���f���f�[�^�Ǘ��̂��߂̖��O���.
*/
namespace Mesh {

	/// ���_�f�[�^�^.
	struct Vertex
	{
		glm::vec3 position; ///< ���W.
		glm::vec4 color; ///< �F.
		glm::vec2 texCoord; ///< �e�N�X�`�����W.
		glm::vec3 normal; ///< �@��.
	};

	/**
	* Vertex Buffer Object���쐬����.
	*
	* @param size ���_�f�[�^�̃T�C�Y.
	* @param data ���_�f�[�^�ւ̃|�C���^.
	*
	* @return �쐬����VBO.
	*/
	GLuint CreateVBO(GLsizeiptr size, const GLvoid* data)
	{
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo;
	}

	/**
	* Index Buffer Object���쐬����.
	*
	* @param size �C���f�b�N�X�f�[�^�̃T�C�Y.
	* @param data �C���f�b�N�X�f�[�^�ւ̃|�C���^.
	*
	* @return �쐬����IBO.
	*/
	GLuint CreateIBO(GLsizeiptr size, const GLvoid* data)
	{
		GLuint ibo = 0;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return ibo;
	}

	/**
	* ���_�A�g���r���[�g��ݒ肷��.
	*
	* @param index ���_�A�g���r���[�g�̃C���f�b�N�X.
	* @param cls   ���_�f�[�^�^��.
	* @param mbr   ���_�A�g���r���[�g�ɐݒ肷��cls�̃����o�ϐ���.
	*/
#define SetVertexAttribPointer(index, cls, mbr) SetVertexAttribPointerI( \
  index, \
  sizeof(cls::mbr) / sizeof(float), \
  sizeof(cls), \
  reinterpret_cast<GLvoid*>(offsetof(cls, mbr)))

	void SetVertexAttribPointerI(
		GLuint index, GLint size, GLsizei stride, const GLvoid* pointer)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
	}

	/**
	* Vertex Array Object���쐬����.
	*
	* @param vbo VAO�Ɋ֘A�t������VBO.
	* @param ibo VAO�Ɋ֘A�t������IBO.
	*
	* @return �쐬����VAO.
	*/
	GLuint CreateVAO(GLuint vbo, GLuint ibo)
	{
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		SetVertexAttribPointer(0, Vertex, position);
		SetVertexAttribPointer(1, Vertex, color);
		SetVertexAttribPointer(2, Vertex, texCoord);
		SetVertexAttribPointer(3, Vertex, normal);
		glBindVertexArray(0);
		//glDeleteBuffers(1, &vbo);
		//glDeleteBuffers(1, &ibo);
		return vao;
	}

	/**
	* 4����FBX�x�N�g����4����glm�x�N�g���ɕϊ�����.
	*
	* @param fbxVec FBX�x�N�g��.
	*
	* @return glm�x�N�g��.
	*/
	template<typename T>
	glm::vec4 ToVec4(const T& fbxVec)
	{
		return glm::vec4(static_cast<float>(fbxVec[0]), static_cast<float>(fbxVec[1]),
			static_cast<float>(fbxVec[2]), static_cast<float>(fbxVec[3]));
	}

	/**
	* 3����FBX�x�N�g����3����glm�x�N�g���ɕϊ�����.
	*
	* @param fbxVec FBX�x�N�g��.
	*
	* @return glm�x�N�g��.
	*/
	template<typename T>
	glm::vec3 ToVec3(const T& fbxVec)
	{
		return glm::vec3(static_cast<float>(fbxVec[0]), static_cast<float>(fbxVec[1]),
			static_cast<float>(fbxVec[2]));
	}

	/**
	* 2����FBX�x�N�g����2����glm�x�N�g���ɕϊ�����.
	*
	* @param fbxVec FBX�x�N�g��.
	*
	* @return glm�x�N�g��.
	*/
	template<typename T>
	glm::vec2 ToVec2(const T& fbxVec)
	{
		return glm::vec2(static_cast<float>(fbxVec[0]), static_cast<float>(fbxVec[1]));
	}

	/**
	* FBX�I�u�W�F�N�g��j�����邽�߂̃w���p�[�\����.
	*/
	template<typename T>
	struct Deleter
	{
		void operator()(T* p) { if (p) { p->Destroy(); } }
	};

	/// FBX�I�u�W�F�N�g���Ǘ����邽�߂̃|�C���^�^.
	template<typename T>
	using FbxPtr = std::unique_ptr<T, Deleter<T>>;

	/**
	* �}�e���A���̉��f�[�^.
	*/
	struct TemporaryMaterial
	{
		glm::vec4 color = glm::vec4(1);
		std::vector<uint32_t> indexBuffer;
		std::vector<Vertex> vertexBuffer;
		std::vector<std::string> textureName;
	};

	/**
	* ���b�V���̉��f�[�^.
	*/
	struct TemporaryMesh
	{
		std::string name;
		std::vector<TemporaryMaterial> materialList;
	};


	/**
	* �R���X�g���N�^.
	*
	* @param meshName ���b�V���f�[�^��.
	* @param begin    �`�悷��}�e���A���̐擪�C���f�b�N�X.
	* @param end      �`�悷��}�e���A���̏I�[�C���f�b�N�X.
	*/
	Mesh::Mesh(const std::string& meshName, size_t begin, size_t end) :
		name(meshName), beginMaterial(begin), endMaterial(end)
	{
	}

	/**
	* ���b�V����`�悷��.
	*
	* @param buffer  �`��Ɏg�p���郁�b�V���o�b�t�@�ւ̃|�C���^.
	*/
	void Mesh::Draw(const BufferPtr& buffer) const
	{
		if (!buffer) {
			return;
		}
		if (buffer->GetMesh(name.c_str()).get() != this) {
			std::cerr << "WARNING: �o�b�t�@�ɑ��݂��Ȃ����b�V��'" << name <<
				"'��`�悵�悤�Ƃ��܂���" << std::endl;
			return;
		}
		for (size_t i = beginMaterial; i < endMaterial; ++i) {
			const Material& m = buffer->GetMaterial(i);
			glDrawElementsBaseVertex(GL_TRIANGLES, m.size, m.type, m.offset, m.baseVertex);
		}
	}

	/**
	* ���b�V���o�b�t�@���쐬����.
	*
	* @param vboSize  �o�b�t�@�Ɋi�[�\�ȑ����_��.
	* @param iboSize  �o�b�t�@�Ɋi�[�\�ȑ��C���f�b�N�X��.
	*/
	BufferPtr Buffer::Create(int vboSize, int iboSize)
	{
		struct Impl : Buffer { Impl() {} ~Impl() {} };
		BufferPtr p = std::make_shared<Impl>();
		p->vbo = CreateVBO(vboSize * sizeof(Vertex), nullptr);
		if (!p->vbo) {
			return {};
		}
		p->ibo = CreateIBO(iboSize * sizeof(uint32_t), nullptr);
		if (!p->ibo) {
			return {};
		}
		p->vao = CreateVAO(p->vbo, p->ibo);
		if (!p->vao) {
			return {};
		}
		return p;
	}

	/**
	* �f�X�g���N�^.
	*/
	Buffer::~Buffer()
	{
		if (vao) {
			glDeleteVertexArrays(1, &vao);
		}
		if (ibo) {
			glDeleteBuffers(1, &ibo);
		}
		if (vbo) {
			glDeleteBuffers(1, &vbo);
		}
	}

	/**
	* ���b�V�����擾����.
	*
	* @param name ���b�V����.
	*
	* @return name�ɑΉ����郁�b�V���ւ̃|�C���^.
	*/
	const MeshPtr& Buffer::GetMesh(const char* name) const
	{
		auto itr = meshList.find(name);
		if (itr == meshList.end()) {
			static const MeshPtr dummy;
			return dummy;
		}
		return itr->second;
	}

	/**
	* �}�e���A�����擾����.
	*
	* @param index �}�e���A���C���f�b�N�X.
	*
	* @return index�ɑΉ�����}�e���A��.
	*/
	const Material& Buffer::GetMaterial(size_t index) const
	{
		if (index >= materialList.size()) {
			static const Material dummy{ GL_UNSIGNED_BYTE, 0, 0, 0, glm::vec4(1) };
			return dummy;
		}
		return materialList[index];
	}

	/**
	* �o�b�t�@���ێ�����VAO��OpenGL�̏����Ώۂɐݒ肷��.
	*/
	void Buffer::BindVAO() const
	{
		glBindVertexArray(vao);
	}

	/**
	* Uniform Block Object���쐬����.
	*
	* @param size Uniform Block�̃T�C�Y.
	* @param data Uniform Block�ɓ]������f�[�^�ւ̃|�C���^.
	*
	* @return �쐬����UBO.
	*/
	GLuint CreateUBO(GLsizeiptr size, const GLvoid* data = nullptr)
	{
		GLuint ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return ubo;
	}
}