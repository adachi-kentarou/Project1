/**
* @file Entity.h
*/
#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED
#include <GL/glew.h>
//#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <functional>
#include <vector>
#include "Node.h"





/// 頂点データ型.
struct Vertex
{
	glm::vec3 position; ///< 座標.
	glm::vec4 color; ///< 色.
	glm::vec2 texCoord; ///< テクスチャ座標.
	glm::vec3 normal; ///< 法線.
};

const Vertex vertices[] = {
	{ { -0.5f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 0.3f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 0.3f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f } },
{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f } },

{ { -0.3f,  0.3f, 0.1f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
{ { -0.3f, -0.5f, 0.1f },{ 0.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.5f,  0.3f, 0.1f },{ 1.0f, 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },

{ { -1.0f,-1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 1.0f,-1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 1.0f, 1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -1.0f, 1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },

//ここからキューブデータ
//下面
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, -1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, -1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
//上面
{ { -1.0f,1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
{ { 1.0f,1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f } },
{ { -1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f } },
//前面
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
{ { -1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
//後面
{ { -1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f } },
{ { 1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f } },
{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, -1.0f } },
{ { -1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, -1.0f } },
//右面
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f } },
//左面
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f } },
{ { -1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f } },
{ { -1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ -1.0f, 0.0f, 0.0f } },
{ { -1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ -1.0f, 0.0f, 0.0f } },
//階段直進
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, -1.0f, -0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, -1.0f, -0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -1.0f,-0.6f, -0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,-0.6f, -0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, -0.6f, -0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, -0.6f, -0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -1.0f,-0.2f, -0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,-0.2f, -0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, -0.2f, 0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, -0.2f, 0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -1.0f,0.2f, 0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,0.2f, 0.2f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, 0.2f, 0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, 0.2f, 0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -1.0f,0.6f, 0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,0.6f, 0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, 0.6f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, 0.6f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
//階段曲進
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, -1.0f, -0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, -1.0f, -0.6f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -0.908239f,-0.6f, -1.061313f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 0.939520f,-0.6f, -0.295946f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 0.786446f, -0.6f, 0.073606f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.061313f, -0.6f, -0.691761f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -0.800000f,-0.2f, -1.082843f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 0.614213f,-0.2f, 0.331371f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 0.331371f, -0.2f, 0.614213f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.082843f, -0.2f, -0.800000f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -0.691761f,0.2f, -1.061313f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 0.073606f,0.2f, 0.786446f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -0.295946f, 0.2f, 0.939520f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.061313f, 0.2f, -0.908239f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

{ { -0.6f,0.6f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -0.6f,0.6f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, 0.6f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, 0.6f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },

//アイテム
{ { -1.0f,-1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f,-1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
{ { -1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },

};


/// インデックスデータ.
const GLuint indices[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
	10, 11, 12, 12, 13, 10,
	14, 15, 16, 16, 17, 14,
	18, 19, 20, 20, 21, 18,
	22, 23, 24, 24, 25, 22,
	26, 27, 28, 28, 29, 26,
	30, 31, 32, 32, 33, 30,
	34, 35, 36, 36, 37, 34,
	38, 39, 40, 40, 41, 38,

	42, 43, 44, 44, 45, 42,
	46, 47, 48, 48, 49, 46,
	50, 51, 52, 52, 53, 50,
	54, 55, 56, 56, 57, 54,
	58, 59, 60, 60, 61, 58,

	62, 63, 64, 64, 65, 62,
	66, 67, 68, 68, 69, 66,
	70, 71, 72, 72, 73, 70,
	74, 75, 76, 76, 77, 74,
	78, 79, 80, 80, 81, 78,

	82, 83, 84, 84, 85, 82,
};

/**
* 部分描画データ.
*/
struct RenderingPart
{
	GLvoid* offset; ///< 描画開始インデックスのバイトオフセット.
	GLsizei size; ///< 描画するインデックス数.
};

/**
* RenderingPartを作成する.
*
* @param offset 描画開始インデックスのオフセット(インデックス単位).
* @param size 描画するインデックス数.
*
* @return 作成した部分描画オブジェクト.
*/
constexpr RenderingPart MakeRenderingPart(GLsizei offset, GLsizei size) {
	return { reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)), size };
}

/**
* 部分描画データリスト.
*/
static const RenderingPart renderingParts[] = {
	//MakeRenderingPart(0, 12),
	MakeRenderingPart(12, 6),///ディスプレイ
	MakeRenderingPart(18, 36),///キューブ
	MakeRenderingPart(18, 6),///タイル
	MakeRenderingPart(54, 30),///階段直進
	MakeRenderingPart(84, 30),///階段曲進
	MakeRenderingPart(114, 6),///アイテム
};







namespace Entity {

	class Buffer;
	class Entity;
	using BufferPtr = std::shared_ptr<Buffer>; ///< エンティティバッファポインタ型.

											   /// 衝突解決ハンドラ型.
	typedef std::function<void(Entity&, Entity&)> CollisionHandlerType;
	
	static const int maxGroupId = 31; ///< グループIDの最大値.
	
	
	/**
	 * 衝突判定形状.
	*/
	struct CollisionData
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	/**
	* エンティティ.
	*/
	class Entity : public Node
	{
		friend class Buffer;
		//friend class Node;

	public:
		/// 状態更新関数型.
		using UpdateFuncType = std::function<void(Entity&, double)>;
		
		void UpdateFunc(const UpdateFuncType& func) { updateFunc = func; }
		const UpdateFuncType& UpdateFunc() const { return updateFunc; }

		void Collision(const CollisionData& c) { colLocal = c; }
		const CollisionData& Collision() const { return colLocal; }
		int GroupId() const { return groupId; }
		void Destroy();

	private:
		Entity() = default;
		~Entity() = default;
		Entity(const Entity&) = default;
		Entity& operator=(const Entity&) = default;
		
	private:
		UpdateFuncType updateFunc; ///< 状態更新関数.
		//Mesh::MeshPtr mesh; ///< エンティティを描画するときに使われるメッシュデータ.
		TexturePtr texture; ///< エンティティを描画するときに使われるテクスチャ.
		Shader::ProgramPtr program; ///< エンティティを描画するときに使われるシェーダ.
		GLintptr uboOffset; ///< UBOのエンティティ用領域へのバイトオフセット.

		int groupId = -1; ///< グループID.
		Buffer* pBuffer = nullptr; ///< 生成元のBufferクラスへのポインタ.
		CollisionData colLocal; ///< ローカル座標系の衝突形状.
		CollisionData colWorld; ///< ワールド座標系の衝突形状.

		bool isActive = false; ///< アクティブなエンティティならtrue, 非アクティブならfalse.
		
	};

	/**
	* エンティティバッファ.
	*/
	class Buffer
	{
	public:
		static BufferPtr Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity,
			int bindingPoint, const char* name);

		Entity* AddEntity(int groupId, const glm::vec3& pos,
			const TexturePtr& t, const Shader::ProgramPtr& p, Entity::UpdateFuncType func);
		void RemoveEntity(Entity* entity);
		void Update(double delta, const glm::mat4& matView, const glm::mat4& matProj);
		void Draw() const;

		void CollisionHandler(int gid0, int gid1, CollisionHandlerType handler);
		const CollisionHandlerType& CollisionHandler(int gid0, int gid1) const;
		void ClearCollisionHandlerList();
		
		const UniformBufferPtr& UniformBuffer()const { return ubo; }

		void DestoroyAllEntity();
	private:
		Buffer() = default;
		~Buffer() = default;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

	private:
		/// エンティティ用リンクリスト.
		struct Link {
			void Insert(Link* e);
			void Remove();
			Link* prev = this;
			Link* next = this;
		};
		/// リンクつきエンティティ.
		struct LinkEntity : public Link, public Entity {};
		/// リンク付きエンティティ配列の削除用関数オブジェクト.
		struct EntityArrayDeleter { void operator()(LinkEntity* p) { delete[] p; } };

		std::unique_ptr<LinkEntity[], EntityArrayDeleter> buffer; ///< エンティティの配列.
		size_t bufferSize; ///< エンティティの総数.
		Link freeList; ///< 未使用のエンティティのリンクリスト.
		Link activeList[maxGroupId + 1]; ///< 使用中のエンティティのリンクリスト.
		GLsizeiptr ubSizePerEntity; ///< 各エンティティが使えるUniform Bufferのバイト数.
		UniformBufferPtr ubo; ///< エンティティ用UBO.
		Link* itrUpdate = nullptr; ///< UpdateとRemoveEntityの相互作用に対応するためのイテレータ.
		Link* itrUpdateRhs = nullptr;

		struct CollisionHandlerInfo
		{
			int groupId[2];
			CollisionHandlerType handler;
		};
		std::vector<CollisionHandlerInfo> collisionHandlerList;
	};
}

#endif