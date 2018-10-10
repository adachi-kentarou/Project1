/**
* @file Mesh.cpp
*/
#include "Mesh.h"
//#include <fbxsdk.h>
#include <iostream>

/**
* モデルデータ管理のための名前空間.
*/
namespace Mesh {

	/// 頂点データ型.
	struct Vertex
	{
		glm::vec3 position; ///< 座標.
		glm::vec4 color; ///< 色.
		glm::vec2 texCoord; ///< テクスチャ座標.
		glm::vec3 normal; ///< 法線.
	};

	/**
	* Vertex Buffer Objectを作成する.
	*
	* @param size 頂点データのサイズ.
	* @param data 頂点データへのポインタ.
	*
	* @return 作成したVBO.
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
	* Index Buffer Objectを作成する.
	*
	* @param size インデックスデータのサイズ.
	* @param data インデックスデータへのポインタ.
	*
	* @return 作成したIBO.
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
	* 頂点アトリビュートを設定する.
	*
	* @param index 頂点アトリビュートのインデックス.
	* @param cls   頂点データ型名.
	* @param mbr   頂点アトリビュートに設定するclsのメンバ変数名.
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
	* Vertex Array Objectを作成する.
	*
	* @param vbo VAOに関連付けられるVBO.
	* @param ibo VAOに関連付けられるIBO.
	*
	* @return 作成したVAO.
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
	* 4次元FBXベクトルを4次元glmベクトルに変換する.
	*
	* @param fbxVec FBXベクトル.
	*
	* @return glmベクトル.
	*/
	template<typename T>
	glm::vec4 ToVec4(const T& fbxVec)
	{
		return glm::vec4(static_cast<float>(fbxVec[0]), static_cast<float>(fbxVec[1]),
			static_cast<float>(fbxVec[2]), static_cast<float>(fbxVec[3]));
	}

	/**
	* 3次元FBXベクトルを3次元glmベクトルに変換する.
	*
	* @param fbxVec FBXベクトル.
	*
	* @return glmベクトル.
	*/
	template<typename T>
	glm::vec3 ToVec3(const T& fbxVec)
	{
		return glm::vec3(static_cast<float>(fbxVec[0]), static_cast<float>(fbxVec[1]),
			static_cast<float>(fbxVec[2]));
	}

	/**
	* 2次元FBXベクトルを2次元glmベクトルに変換する.
	*
	* @param fbxVec FBXベクトル.
	*
	* @return glmベクトル.
	*/
	template<typename T>
	glm::vec2 ToVec2(const T& fbxVec)
	{
		return glm::vec2(static_cast<float>(fbxVec[0]), static_cast<float>(fbxVec[1]));
	}

	/**
	* FBXオブジェクトを破棄するためのヘルパー構造体.
	*/
	template<typename T>
	struct Deleter
	{
		void operator()(T* p) { if (p) { p->Destroy(); } }
	};

	/// FBXオブジェクトを管理するためのポインタ型.
	template<typename T>
	using FbxPtr = std::unique_ptr<T, Deleter<T>>;

	/**
	* マテリアルの仮データ.
	*/
	struct TemporaryMaterial
	{
		glm::vec4 color = glm::vec4(1);
		std::vector<uint32_t> indexBuffer;
		std::vector<Vertex> vertexBuffer;
		std::vector<std::string> textureName;
	};

	/**
	* メッシュの仮データ.
	*/
	struct TemporaryMesh
	{
		std::string name;
		std::vector<TemporaryMaterial> materialList;
	};


	/**
	* コンストラクタ.
	*
	* @param meshName メッシュデータ名.
	* @param begin    描画するマテリアルの先頭インデックス.
	* @param end      描画するマテリアルの終端インデックス.
	*/
	Mesh::Mesh(const std::string& meshName, size_t begin, size_t end) :
		name(meshName), beginMaterial(begin), endMaterial(end)
	{
	}

	/**
	* メッシュを描画する.
	*
	* @param buffer  描画に使用するメッシュバッファへのポインタ.
	*/
	void Mesh::Draw(const BufferPtr& buffer) const
	{
		if (!buffer) {
			return;
		}
		if (buffer->GetMesh(name.c_str()).get() != this) {
			std::cerr << "WARNING: バッファに存在しないメッシュ'" << name <<
				"'を描画しようとしました" << std::endl;
			return;
		}
		for (size_t i = beginMaterial; i < endMaterial; ++i) {
			const Material& m = buffer->GetMaterial(i);
			glDrawElementsBaseVertex(GL_TRIANGLES, m.size, m.type, m.offset, m.baseVertex);
		}
	}

	/**
	* メッシュバッファを作成する.
	*
	* @param vboSize  バッファに格納可能な総頂点数.
	* @param iboSize  バッファに格納可能な総インデックス数.
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
	* デストラクタ.
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
	* メッシュを取得する.
	*
	* @param name メッシュ名.
	*
	* @return nameに対応するメッシュへのポインタ.
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
	* マテリアルを取得する.
	*
	* @param index マテリアルインデックス.
	*
	* @return indexに対応するマテリアル.
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
	* バッファが保持するVAOをOpenGLの処理対象に設定する.
	*/
	void Buffer::BindVAO() const
	{
		glBindVertexArray(vao);
	}

	/**
	* Uniform Block Objectを作成する.
	*
	* @param size Uniform Blockのサイズ.
	* @param data Uniform Blockに転送するデータへのポインタ.
	*
	* @return 作成したUBO.
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