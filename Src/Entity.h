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





/// ���_�f�[�^�^.
struct Vertex
{
	glm::vec3 position; ///< ���W.
	glm::vec4 color; ///< �F.
	glm::vec2 texCoord; ///< �e�N�X�`�����W.
	glm::vec3 normal; ///< �@��.
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

//��������L���[�u�f�[�^
//����
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } },
{ { 1.0f, -1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
{ { -1.0f, -1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, -1.0f, 0.0f } },
//���
{ { -1.0f,1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
{ { 1.0f,1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f, 0.0f } },
{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f, 0.0f } },
{ { -1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f } },
//�O��
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
{ { -1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
//���
{ { -1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f } },
{ { 1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f } },
{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, -1.0f } },
{ { -1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, -1.0f } },
//�E��
{ { 1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f, 0.0f } },
{ { 1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f } },
//����
{ { -1.0f,-1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ -1.0f, 0.0f, 0.0f } },
{ { -1.0f,-1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f } },
{ { -1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ -1.0f, 0.0f, 0.0f } },
{ { -1.0f, 1.0f, -1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ -1.0f, 0.0f, 0.0f } },
//�K�i���i
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
//�K�i�Ȑi
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

//�A�C�e��
{ { -1.0f,-1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f,-1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f } },
{ { 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },
{ { -1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f } },

};


/// �C���f�b�N�X�f�[�^.
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
* �����`��f�[�^.
*/
struct RenderingPart
{
	GLvoid* offset; ///< �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g.
	GLsizei size; ///< �`�悷��C���f�b�N�X��.
};

/**
* RenderingPart���쐬����.
*
* @param offset �`��J�n�C���f�b�N�X�̃I�t�Z�b�g(�C���f�b�N�X�P��).
* @param size �`�悷��C���f�b�N�X��.
*
* @return �쐬���������`��I�u�W�F�N�g.
*/
constexpr RenderingPart MakeRenderingPart(GLsizei offset, GLsizei size) {
	return { reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)), size };
}

/**
* �����`��f�[�^���X�g.
*/
static const RenderingPart renderingParts[] = {
	//MakeRenderingPart(0, 12),
	MakeRenderingPart(12, 6),///�f�B�X�v���C
	MakeRenderingPart(18, 36),///�L���[�u
	MakeRenderingPart(18, 6),///�^�C��
	MakeRenderingPart(54, 30),///�K�i���i
	MakeRenderingPart(84, 30),///�K�i�Ȑi
	MakeRenderingPart(114, 6),///�A�C�e��
};







namespace Entity {

	class Buffer;
	class Entity;
	using BufferPtr = std::shared_ptr<Buffer>; ///< �G���e�B�e�B�o�b�t�@�|�C���^�^.

											   /// �Փˉ����n���h���^.
	typedef std::function<void(Entity&, Entity&)> CollisionHandlerType;
	
	static const int maxGroupId = 31; ///< �O���[�vID�̍ő�l.
	
	
	/**
	 * �Փ˔���`��.
	*/
	struct CollisionData
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	/**
	* �G���e�B�e�B.
	*/
	class Entity : public Node
	{
		friend class Buffer;
		//friend class Node;

	public:
		/// ��ԍX�V�֐��^.
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
		UpdateFuncType updateFunc; ///< ��ԍX�V�֐�.
		//Mesh::MeshPtr mesh; ///< �G���e�B�e�B��`�悷��Ƃ��Ɏg���郁�b�V���f�[�^.
		TexturePtr texture; ///< �G���e�B�e�B��`�悷��Ƃ��Ɏg����e�N�X�`��.
		Shader::ProgramPtr program; ///< �G���e�B�e�B��`�悷��Ƃ��Ɏg����V�F�[�_.
		GLintptr uboOffset; ///< UBO�̃G���e�B�e�B�p�̈�ւ̃o�C�g�I�t�Z�b�g.

		int groupId = -1; ///< �O���[�vID.
		Buffer* pBuffer = nullptr; ///< ��������Buffer�N���X�ւ̃|�C���^.
		CollisionData colLocal; ///< ���[�J�����W�n�̏Փˌ`��.
		CollisionData colWorld; ///< ���[���h���W�n�̏Փˌ`��.

		bool isActive = false; ///< �A�N�e�B�u�ȃG���e�B�e�B�Ȃ�true, ��A�N�e�B�u�Ȃ�false.
		
	};

	/**
	* �G���e�B�e�B�o�b�t�@.
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
		/// �G���e�B�e�B�p�����N���X�g.
		struct Link {
			void Insert(Link* e);
			void Remove();
			Link* prev = this;
			Link* next = this;
		};
		/// �����N���G���e�B�e�B.
		struct LinkEntity : public Link, public Entity {};
		/// �����N�t���G���e�B�e�B�z��̍폜�p�֐��I�u�W�F�N�g.
		struct EntityArrayDeleter { void operator()(LinkEntity* p) { delete[] p; } };

		std::unique_ptr<LinkEntity[], EntityArrayDeleter> buffer; ///< �G���e�B�e�B�̔z��.
		size_t bufferSize; ///< �G���e�B�e�B�̑���.
		Link freeList; ///< ���g�p�̃G���e�B�e�B�̃����N���X�g.
		Link activeList[maxGroupId + 1]; ///< �g�p���̃G���e�B�e�B�̃����N���X�g.
		GLsizeiptr ubSizePerEntity; ///< �e�G���e�B�e�B���g����Uniform Buffer�̃o�C�g��.
		UniformBufferPtr ubo; ///< �G���e�B�e�B�pUBO.
		Link* itrUpdate = nullptr; ///< Update��RemoveEntity�̑��ݍ�p�ɑΉ����邽�߂̃C�e���[�^.
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