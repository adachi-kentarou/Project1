/**
* @file Main.cpp
*/
/// �G���g���[�|�C���g.
#include "GameEngine.h"
#include "GameState.h"
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include "../Res/Audio/SampleSound_acf.h"
#include "../Res/Audio/SampleCueSheet.h"
/*
#include "GLFWEW.h"
#include "Shader.h"
#include "OffscreenBuffer.h"
#include "UniformBuffer.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include <random>
#include <iostream>
#include <vector>
#include "Mesh.h"
*/

/// �G���e�B�e�B�̏Փ˃O���[�vID.
enum EntityGroupId
{
	EntityGroupId_Player,
	EntityGroupId_PlayerShot,
	EntityGroupId_Enemy,
	EntityGroupId_EnemyShot,
	EntityGroupId_Others,
};

/// �����v���C���[��ID.
enum AudioPlayerId
{
	AudioPlayerId_Shot, ///< ���@�̃V���b�g��.
	AudioPlayerId_Bomb, ///< ������.
	AudioPlayerId_BGM, ///<�@BGM.
	AudioPlayerId_Max, ///< �Đ�����p�v���C���[�̑���.

	AudioPlayerId_UI = AudioPlayerId_Shot, ///<�@���[�U�[�C���^�[�t�F�C�X���쉹.
};

/// �Փˌ`�󃊃X�g.
/*
static const Entity::CollisionData collisionDataList[] = {
	{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(0.5f, 0.5f, 1.0f) },
	{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
	{ glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f) },
};
*/


/// 3D�x�N�g���^.
struct Vector3
{
	float x, y, z;
};

/// RGBA�J���[�^.
struct Color
{
	float r, g, b, a;
};
/*
/// ���_�f�[�^�^.
struct Vertex
{
glm::vec3 position; ///< ���W.
glm::vec4 color; ///< �F.
glm::vec2 texCoord; ///< �e�N�X�`�����W.
};
*/


/// ���_�f�[�^.
/*
const Vertex vertices[] = {
	{ {-0.5f, -0.3f, 0.5f},{ 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
	{ { 0.3f, -0.3f, 0.5f},{ 0.0f, 1.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 0.3f,  0.5f, 0.5f},{ 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ {-0.5f,  0.5f, 0.5f},{ 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },

	{ {-0.3f,  0.3f, 0.1f},{ 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
	{ {-0.3f, -0.5f, 0.1f},{ 0.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.1f},{ 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.1f},{ 1.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 0.5f,  0.3f, 0.1f},{ 1.0f, 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
	{ {-0.3f,  0.3f, 0.1f},{ 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },
};
*/

/*
const Vertex vertices[] = {
	{ { -0.5f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
	{ { 0.3f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 0.3f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },

	{ { -0.3f,  0.3f, 0.1f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
	{ { -0.3f, -0.5f, 0.1f },{ 0.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.1f },{ 0.0f, 0.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 0.5f,  0.3f, 0.1f },{ 1.0f, 1.0f, 0.0f, 1.0f },{ 1.0f, 1.0f } },
	{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },

	{ {-1.0f,-1.0f, 0.5f},{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
	{ { 1.0f,-1.0f, 0.5f},{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
	{ { 1.0f, 1.0f, 0.5f},{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
	{ {-1.0f, 1.0f, 0.5f},{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
};
/// �C���f�b�N�X�f�[�^.
const GLuint indices[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
	10, 11, 12, 12, 13, 10,
};

*/

/*
/// ���_�V�F�[�_�̃p�����[�^�^.
struct VertexData
{
	glm::mat4 matMVP;
	glm::mat4 matModel;
	glm::mat3x4 matNormal;
	glm::vec4 color;
	//glm::vec4 lightPosition;
	//glm::vec4 lightColor;
	//glm::vec4 ambientColor;
};
*/


const int maxLightCount = 4; ///< ���C�g�̐�.

/**
* ���C�g�f�[�^(�_����).
*/
struct PointLight
{
	glm::vec4 position; ///< ���W(���[���h���W�n).
	glm::vec4 color; ///< ���邳.
};

/**
* ���C�e�B���O�p�����[�^���V�F�[�_�ɓ]�����邽�߂̍\����.
struct LightData
{
glm::vec4 ambientColor; ///< ����.
PointLight light[maxLightCount]; ///< ���C�g�̃��X�g.
};
*/


/**
* �|�X�g�G�t�F�N�g�f�[�^���V�F�[�_�ɓ]�����邽�߂̍\����.
struct PostEffectData
{
glm::mat4x4 matColor; ///< �F�ϊ��s��.
};
*/

/*
/// �o�C���f�B���O�|�C���g.
enum BindingPoint
{
BINDINGPOINT_VERTEXDATA, ///< ���_�V�F�[�_�p�p�����[�^�̃o�C���f�B���O�|�C���g.
BINDINGPOINT_LIGHTDATA, ///< ���C�e�B���O�p�����[�^�p�̃o�C���f�B���O�|�C���g.
BINDINGPOINT_POSTEFFECTDATA, ///< �|�X�g�G�t�F�N�g�p�����[�^�p�̃o�C���f�B���O�|�C���g.
};
*/


/**
* �����`��f�[�^.
struct RenderingPart
{
GLvoid* offset; ///< �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g.
GLsizei size; ///< �`�悷��C���f�b�N�X��.
};
*/


/**
* RenderingPart���쐬����.
*
* @param offset �`��J�n�C���f�b�N�X�̃I�t�Z�b�g(�C���f�b�N�X�P��).
* @param size �`�悷��C���f�b�N�X��.
*
* @return �쐬���������`��I�u�W�F�N�g.
*/
/*
constexpr RenderingPart MakeRenderingPart(GLsizei offset, GLsizei size) {
	return { reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)), size };
}
*/


/**
* �����`��f�[�^���X�g.
*/
/*
static const RenderingPart renderingParts[] = {
	MakeRenderingPart(0, 12),
	MakeRenderingPart(12, 6),
};
*/


/**
* Vertex Buffer Object���쐬����.
*
* @param size ���_�f�[�^�̃T�C�Y.
* @param data ���_�f�[�^�ւ̃|�C���^.
*
* @return �쐬����VBO.
*/
/*
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data)
{
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}
*/


/**
* Index Buffer Object���쐬����.
*
* @param size �C���f�b�N�X�f�[�^�̃T�C�Y.
* @param data �C���f�b�N�X�f�[�^�ւ̃|�C���^.
*
* @return �쐬����IBO.
*/
/*
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data)
{
	GLuint ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ibo;
}
*/


/**
* ���_�A�g���r���[�g��ݒ肷��.
*
* @param index ���_�A�g���r���[�g�̃C���f�b�N�X.
* @param cls   ���_�f�[�^�^��.
* @param mbr   ���_�A�g���r���[�g�ɐݒ肷��cls�̃����o�ϐ���.
*/
/*
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
*/


/**
* Vertex Array Object���쐬����.
*
* @param vbo VAO�Ɋ֘A�t������VBO.
* @param ibo VAO�Ɋ֘A�t������IBO.
*
* @return �쐬����VAO.
*/
/*
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
	glBindVertexArray(0);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1,&ibo);
	return vao;
}
*/


/**
* �G�̉~�Ղ̏�Ԃ��X�V����.
*/
/*
struct UpdateToroid
{
	//explicit UpdateToroid(const Entity::BufferPtr& buffer) : entityBuffer(buffer) {}

	void operator()(Entity::Entity& entity, double delta)
	{
		// �͈͊O�ɏo����폜����.
		const glm::vec3 pos = entity.Position();
		if (std::abs(pos.x) > 40.0f || std::abs(pos.z) > 40.0f) {
			//entityBuffer->RemoveEntity(&entity);
			GameEngine::Instance().RemoveEntity(&entity);
			return;
		}

		// �~�Ղ���]������.
		float rot = glm::angle(entity.Rotation());
		//rot += glm::radians(15.0f) * static_cast<float>(delta);
		rot += glm::radians(60.0f) * static_cast<float>(delta);
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity.Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		// ���_�V�F�[�_�[�̃p�����[�^��UBO�ɃR�s�[����.
		//VertexData data;
		
	}

	//Entity::BufferPtr entityBuffer;
};
*/


/**
* ���@�̒e�̍X�V.
*/
/*
struct UpdatePlayerShot
{
	void operator()(Entity::Entity& entity, double delta)
	{
		// �͈͊O�ɏo����폜����.
		const glm::vec3 pos = entity.Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
			entity.Destroy();
			return;
		}
	}
};
*/


/**
* �����̍X�V.
*/
/*
struct UpdateBlast
{
	void operator()(Entity::Entity& entity, double delta) {
		timer += delta;
		if (timer >= 0.5) {
			entity.Destroy();
			return;
		}
		const float variation = static_cast<float>(timer * 4); // �ω���.
		entity.Scale(glm::vec3(static_cast<float>(1 + variation))); // ���X�Ɋg�傷��.
																	// ���Ԍo�߂ŐF�Ɠ����x��ω�������.
		static const glm::vec4 color[] = {
			glm::vec4(1.0f, 1.0f, 0.75f, 1),
			glm::vec4(1.0f, 0.5f, 0.1f, 1),
			glm::vec4(0.25f, 0.1f, 0.1f, 0),
		};
		const glm::vec4 col0 = color[static_cast<int>(variation)];
		const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
		const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
		entity.Color(newColor);
		// Y����]������.
		glm::vec3 euler = glm::eulerAngles(entity.Rotation());
		euler.y += glm::radians(60.0f) * static_cast<float>(delta);
		entity.Rotation(glm::quat(euler));
	}

	double timer = 0;
};
*/


/**
* ���@�̍X�V.
*/
/*
struct UpdatePlayer
{
	void operator()(Entity::Entity& entity, double delta)
	{
		GameEngine& game = GameEngine::Instance();
		const GamePad gamepad = game.GetGamePad();
		glm::vec3 vec;
		float rotZ = 0;
		if (gamepad.buttons & GamePad::DPAD_LEFT) {
			vec.x = 1;
			rotZ = -glm::radians(30.0f);
		}
		else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
			vec.x = -1;
			rotZ = glm::radians(30.0f);
		}
		if (gamepad.buttons & GamePad::DPAD_UP) {
			vec.z = 1;
		}
		else if (gamepad.buttons & GamePad::DPAD_DOWN) {
			vec.z = -1;
		}
		if (vec.x || vec.z) {
			vec = glm::normalize(vec) * 15.0f;
		}
		entity.Velocity(vec);
		entity.Rotation(glm::quat(glm::vec3(0, 0, rotZ)));
		glm::vec3 pos = entity.Position();
		pos = glm::min(glm::vec3(11, 100, 20), glm::max(pos, glm::vec3(-11, -100, 1)));
		entity.Position(pos);

		if (gamepad.buttons & GamePad::A) {
			shotInterval -= delta;
			if (shotInterval <= 0) {
				glm::vec3 pos = entity.Position();
				pos.x -= 0.3f; // ���@�̒��S���獶��0.3���炵���ʒu��1�߂̔��˓_.
				for (int i = 0; i < 2; ++i) {
					if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, pos,
						"NormalShot", "Res/Player.bmp", UpdatePlayerShot())) {
						p->Velocity(glm::vec3(0, 0, 80));
						p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
						
					}
					pos.x += 0.6f; // ���S����ɉE��0.3���炵���ʒu��2�߂̔��˓_.
					
				}
				shotInterval += 0.25; // �b��4�A��.
				game.PlayAudio(AudioPlayerId_Shot, CRI_SAMPLECUESHEET_PLAYERSHOT);
			}
			
		}
		else {
			shotInterval = 0;
			
		}
		
	}

	private:
		double shotInterval = 0;
};
*/


/**
* �Q�[���̏�Ԃ��X�V����.
*
* @param entityBuffer �G�G���e�B�e�B�ǉ���̃G���e�B�e�B�o�b�t�@.
* @param meshBuffer   �G�G���e�B�e�B�̃��b�V�����Ǘ����Ă��郁�b�V���o�b�t�@.
* @param tex          �G�G���e�B�e�B�p�̃e�N�X�`��.
* @param prog         �G�G���e�B�e�B�p�̃V�F�[�_�[�v���O����.
*/
/*
void Update(Entity::BufferPtr entityBuffer, Mesh::BufferPtr meshBuffer,
	TexturePtr tex, Shader::ProgramPtr prog)
{
	static std::mt19937 rand(std::random_device{}());
	static double interval = 0;

	interval -= 1.0 / 60.0;
	if (interval <= 0) {
		const std::uniform_real_distribution<float> posXRange(-15, 15);
		const glm::vec3 pos(posXRange(rand), 0, 40);
		const Mesh::MeshPtr& mesh = meshBuffer->GetMesh("Toroid");
		if (Entity::Entity* p = entityBuffer->AddEntity(pos, mesh, tex, prog,
			UpdateToroid(entityBuffer))) {
			p->Velocity(glm::vec3(pos.x < 0 ? 0.1f : -0.1f, 0, -1.0f));
		}
		const std::uniform_real_distribution<double> intervalRange(3.0, 6.0);
		interval = intervalRange(rand);
	}
}
*/


/**
* �Q�[����Ԃ̍X�V.
*/
class Update
{
public:

	void operator()(double delta)
	{
		/*
		GameEngine& game = GameEngine::Instance();

		if (!isInitialized) {
			isInitialized = true;
			game.Camera({ { 0, 20, -8},{ 0, 0, 12 },{ 0, 0, 1 } });
			game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
			game.Light(0,{ { 40, 100, 10, 1 },{ 12000, 12000, 12000, 1 } });
			pPlayer = game.AddEntity(EntityGroupId_Player, glm::vec3(0, 0, 2),
				"Aircraft", "Res/Player.bmp", UpdatePlayer());
			pPlayer->Collision(collisionDataList[EntityGroupId_Player]);
		}

		std::uniform_int_distribution<> posXRange(-15, 15);
		std::uniform_int_distribution<> posZRange(38, 40);
		interval -= delta;
		if (interval <= 0) {
			std::uniform_int_distribution<> rndAddingCount(1, 5);
			for (int i = rndAddingCount(game.Rand()); i > 0; --i) {
				const glm::vec3 pos(posXRange(game.Rand()), 0, posZRange(game.Rand()));
				if (Entity::Entity* p = game.AddEntity(
					EntityGroupId_Enemy, pos, "Toroid", "Res/Toroid.bmp", UpdateToroid())) {
					p->Velocity({ pos.x < 0 ? 3.0f : -3.0f, 0, -12.0f });
					p->Collision(collisionDataList[EntityGroupId_Enemy]);
				}
			}
			std::normal_distribution<> intervalRange(2.0, 0.5);
			interval += glm::clamp(intervalRange(game.Rand()), 0.5, 3.0);
		}
		*/
		
		/*
		char str[16];
		snprintf(str, 16, "%08.0f", game.Variable("score"));
		game.FontScale(glm::vec2(1.0f, 1.0f));
		game.FontColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		game.AddString(glm::vec2(320.0f, 8.0f), str);
		*/
		
	}

private:
	bool isInitialized = false;
	double interval = 0;

	Entity::Entity* pPlayer = nullptr;
};

/**
* ���@�̒e�ƓG�̏Փˏ���.
*/
/*
void PlayerShotAndEnemyCollisionHandler(Entity::Entity& lhs, Entity::Entity& rhs)
{
	GameEngine& game = GameEngine::Instance();
	if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, rhs.Position(),
		"Blast", "Res/Toroid.bmp", UpdateBlast())) {
		const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
		p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));

		game.Variable("score") += 100;
	}

	game.PlayAudio(AudioPlayerId_Bomb, CRI_SAMPLECUESHEET_BOMB);

	lhs.Destroy();
	rhs.Destroy();
}
*/



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

/// ���_�V�F�[�_.
static const char* vsCode =
"#version 410 \n"
"layout(location=0) in vec3 vPosition;"
"layout(location=1) in vec4 vColor;"
"layout(location=2) in vec2 vTexCoord;"
"layout(location=0) out vec4 outColor;"
"layout(location=1) out vec2 outTexCoord;"
"uniform mat4x4 matMVP;"
"void main() {"
"  outColor = vColor;"
"outTexCoord = vTexCoord;"
"  gl_Position = matMVP * vec4(vPosition, 1.0);"
"}";

/// �t���O�����g�V�F�[�_.
static const char* fsCode =
"#version 410 \n"
"layout(location=0) in vec4 inColor;"
"layout(location=1) in vec2 inTexCoord;"
"uniform sampler2D colorSampler;"
"out vec4 fragColor;"
"void main() {"
"  fragColor = inColor * texture(colorSampler, inTexCoord);"
"}";

/**
* �V�F�[�_�R�[�h���R���p�C������.
*
* @param type �V�F�[�_�̎��.
* @param string �V�F�[�_�R�[�h�ւ̃|�C���^.
*
* @return �쐬�����V�F�[�_�I�u�W�F�N�g.
*/
GLuint CompileShader(GLenum type, const GLchar* string)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &string, nullptr);
	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen) {
			std::vector<char> buf;
			buf.resize(infoLen);
			if (static_cast<int>(buf.size()) >= infoLen) {
				glGetShaderInfoLog(shader, infoLen, NULL, buf.data());
				std::cerr << "ERROR: �V�F�[�_�̃R���p�C���Ɏ��s" << buf.data() << std::endl;
			}
			glDeleteShader(shader);
		}
		return 0;
	}
	return shader;
}

/**
* �V�F�[�_�v���O�������쐬����.
*
* @param vsCode ���_�V�F�[�_�R�[�h�ւ̃|�C���^.
* @param fsCode �t���O�����g�V�F�[�_�R�[�h�ւ̃|�C���^.
*
* @return �쐬�����v���O�����I�u�W�F�N�g.
*/
GLuint CreateProgram(const GLchar* vsCode, const GLchar* fsCode)
{
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vsCode);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fsCode);
	if (!vs || !fs) {
		return 0;
	}
	GLuint program = glCreateProgram();
	glAttachShader(program, fs);
	glDeleteShader(fs);
	glAttachShader(program, vs);
	glDeleteShader(vs);
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		GLint infoLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen) {
			std::vector<char> buf;
			buf.resize(infoLen);
			if (static_cast<int>(buf.size()) >= infoLen) {
				glGetProgramInfoLog(program, infoLen, NULL, buf.data());
				std::cerr << "ERROR: �V�F�[�_�̃����N�Ɏ��s" << buf.data() << std::endl;
			}
		}
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

int main()
{
	GameEngine& game = GameEngine::Instance();

	GameEngine::Instance().Variable("score") = 0;

	if (!game.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}

	if (!game.InitAudio("Res/Audio/SampleSound.acf", "Res/Audio/SampleCueSheet.acb",
		"Res/Audio/SampleCueSheet.awb", CRI_SAMPLESOUND_ACF_DSPSETTING_DSPBUSSETTING_0,
		AudioPlayerId_Max)) {
		return 1;
		
	}

	game.LoadTextureFromFile("Res/Toroid.bmp");
	game.LoadTextureFromFile("Res/Player.bmp");

	game.LoadTextureFromFile("Res/SpaceSphere.bmp");

	game.LoadMeshFromFile("Res/Toroid.fbx");
	game.LoadMeshFromFile("Res/Player.fbx");

	game.LoadMeshFromFile("Res/Blast.fbx");

	game.LoadMeshFromFile("Res/SpaceSphere.fbx");

	game.LoadFontFromFile("Res/Font.fnt");
	
	/*
	game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy,
		&PlayerShotAndEnemyCollisionHandler);
	*/
	
	//game.UpdateFunc(Update());
	game.UpdateFunc(GameState::Title());
	game.Run();
	return 0;
}


/*
int main()
{


	// GLFW�̏�����.
	if (glfwInit() != GL_TRUE) {
		return 1;
	}
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (!window.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}


	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::cout << "Renderer: " << renderer << std::endl;
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Version: " << version << std::endl;

	const GLuint vbo = CreateVBO(sizeof(vertices), vertices);
	const GLuint ibo = CreateIBO(sizeof(indices), indices);
	const GLuint vao = CreateVAO(vbo, ibo);

	//const GLuint ubo = CreateUBO(sizeof(VertexData));
	const UniformBufferPtr uboVertex = UniformBuffer::Create(
		sizeof(VertexData), BINDINGPOINT_VERTEXDATA, "VertexData");
		//sizeof(VertexData), 0, "VertexData");

	
	//std::vector<char> vsBuf;
	//Shader::ReadFile("Res/Tutorial.vert", vsBuf);
	//std::vector<char> fsBuf;
	//Shader::ReadFile("Res/Tutorial.frag", fsBuf);

	//std::cout << vsBuf.data() << std::endl;
	//std::cout << fsBuf.data() << std::endl;
	




	//const GLuint shaderProgram = Shader::CreateProgram(vsBuf.data(), fsBuf.data());
	//const GLuint shaderProgram = Shader::CreateProgram(vsCode, fsCode);


	//const GLuint shaderProgram = Shader::CreateProgramFromFile("Res/Tutorial.vert", "Res/Tutorial.frag");

	const UniformBufferPtr uboLight = UniformBuffer::Create(
		sizeof(LightData), BINDINGPOINT_LIGHTDATA, "LightData");

	const Shader::ProgramPtr progTutorial =
		Shader::Program::Create("Res/Tutorial.vert", "Res/Tutorial.frag");
	
	const Shader::ProgramPtr progColorFilter =
		Shader::Program::Create("Res/ColorFilter.vert", "Res/ColorFilter.frag");
	
	//const Shader::ProgramPtr progColorFilter =
	//	Shader::Program::Create("Res/Posterization.vert", "Res/Posterization.frag");
	
	

	const UniformBufferPtr uboPostEffect = UniformBuffer::Create(
		sizeof(PostEffectData), BINDINGPOINT_POSTEFFECTDATA, "PostEffectData");

	if (!vbo || !ibo || !vao || !uboVertex || !uboLight || !uboPostEffect || !progTutorial || !progColorFilter) {
		return 1;
	}

	
	//const GLuint uboIndex = glGetUniformBlockIndex(shaderProgram, "VertexData");
	//if (uboIndex == GL_INVALID_INDEX) {
	//	return 1;

	//}
	
	//glUniformBlockBinding(shaderProgram, uboIndex, 0);

	//progTutorial->UniformBlockBinding("VertexData", 0);
	progTutorial->UniformBlockBinding(*uboVertex);
	progTutorial->UniformBlockBinding(*uboLight);
	progColorFilter->UniformBlockBinding(*uboPostEffect);

	/// �e�N�X�`���f�[�^.
	TexturePtr tex = Texture::LoadFromFile("Res/Sample.bmp");
	TexturePtr texToroid = Texture::LoadFromFile("Res/Toroid.bmp");

	if (!tex || !texToroid) {
		std::cerr << "ERROR:1" << std::endl;
		return 1;

	}
	
	Mesh::BufferPtr meshBuffer = Mesh::Buffer::Create(50000, 50000);
	meshBuffer->LoadMeshFromFile("Res/Toroid.fbx");

	Entity::BufferPtr entityBuffer = Entity::Buffer::Create(1024,
		sizeof(VertexData), 0, "VertexData");
	if (!entityBuffer) {
		return 1;
		
	}


	//�I�t�X�N���[���o�b�t�@���쐬
	const OffscreenBufferPtr offscreen = OffscreenBuffer::Create(800, 600);



	glEnable(GL_DEPTH_TEST);

	// ���C�����[�v.
	while (!window.ShouldClose()) {
		Update(entityBuffer, meshBuffer, texToroid, progTutorial);
		//window.viewPort();
#define USE_OFFSCREEN_BUFFER 1
#ifdef USE_OFFSCREEN_BUFFER
		//�I�t�X�N���[���o�b�t�@�ɐݒ�
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());
#endif
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ���_����]�ړ�������.
		
		//static float degree = 0.0f;
		//degree += 0.1f;
		//if (degree >= 360.0f) { degree -= 360.0f; }
		//const glm::vec3 viewPos = glm::rotate(
		//	glm::mat4(), glm::radians(degree), glm::vec3(0, 1, 0)) * glm::vec4(2, 3, 3, 1);
		
		const glm::vec3 viewPos = glm::vec4(0, 20, -20, 1);

		//glUseProgram(shaderProgram);
		progTutorial->UseProgram();



		// ���W�ϊ��s����쐬���ăV�F�[�_�ɓ]������.
			//const GLint matMVPLoc = glGetUniformLocation(shaderProgram, "matMVP");
		//if (matMVPLoc >= 0) {
		const glm::mat4x4 matProj =
			glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		const glm::mat4x4 matView =
			glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		const glm::mat4x4 matMVP = matProj * matView;
		//glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);

	//}

		//���ݒ�
		VertexData vertexData;
		vertexData.matMVP = matProj * matView;
		
		//vertexData.lightPosition = glm::vec4(1, 1, 1, 1);
		//vertexData.lightColor = glm::vec4(2, 2, 2, 1);
		//vertexData.ambientColor = glm::vec4(0.05f, 0.1f, 0.2f, 1);
		
		
		//vertexData.ambientColor = glm::vec4(1, 1, 1, 1);
		//glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(VertexData), &vertexData);
		uboVertex->BufferSubData(&vertexData);

		LightData lightData;
		lightData.ambientColor = glm::vec4(0.05f, 0.1f, 0.2f, 1);
		
		//lightData.light[0].position = glm::vec4(1, 1, 1, 1);
		//lightData.light[0].color = glm::vec4(2, 2, 2, 1);
		//lightData.light[1].position = glm::vec4(-0.2f, 0, 0.6f, 1);
		//lightData.light[1].color = glm::vec4(0.125f, 0.125f, 0.05f, 1);
		
		lightData.light[0].color = glm::vec4(12000, 12000, 12000, 1);
		lightData.light[0].position = glm::vec4(40, 100, 20, 1);
		
		uboLight->BufferSubData(&lightData);

		
		//const GLint colorSamplerLoc = glGetUniformLocation(shaderProgram, "colorSampler");

		//if (colorSamplerLoc >= 0) {
		//	glUniform1i(colorSamplerLoc, 0);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, tex->Id());
		//}
		
		
		progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, tex->Id());

		glBindVertexArray(vao);
		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / sizeof(vertices[0]));
		
		//glDrawElements(
		//	GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]),
		//	GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));
		
		glDrawElements(
			GL_TRIANGLES, renderingParts[0].size, GL_UNSIGNED_INT, renderingParts[0].offset);

		progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, texToroid->Id());
		meshBuffer->BindVAO();
		meshBuffer->GetMesh("Toroid")->Draw(meshBuffer);

		entityBuffer->Update(1.0 / 60.0, matView, matProj);
		entityBuffer->Draw(meshBuffer);

#ifdef USE_OFFSCREEN_BUFFER
		glBindVertexArray(vao);

		//�I�t�X�N���[���o�b�t�@����ʏ�o�b�t�@�ɖ߂�
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//���C�g�V�F�[�_�[�@�ꎞ�R�����g�A�E�g
#if 0
		progTutorial->UseProgram();
		progTutorial->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexutre());
#endif	
		vertexData = {};

		uboVertex->BufferSubData(&vertexData);

		lightData = {};
		lightData.ambientColor = glm::vec4(1);
		uboLight->BufferSubData(&lightData);
		//
		
		
		
		//���m�g�[���V�F�[�_�[
		progColorFilter->UseProgram();
		progColorFilter->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexutre());

		PostEffectData postEffect;
		postEffect.matColor[0] = glm::vec4(0.393f, 0.349f, 0.272f, 0);
		postEffect.matColor[1] = glm::vec4(0.769f, 0.686f, 0.534f, 0);
		postEffect.matColor[2] = glm::vec4(0.189f, 0.168f, 0.131f, 0);
		postEffect.matColor[3] = glm::vec4(0, 0, 0, 1);
		uboPostEffect->BufferSubData(&postEffect);
		
		


		glDrawElements(
			GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

#endif
		window.SwapBuffers();
	}

	//glDeleteBuffers(1, &ubo);

	//glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);

	return 0;
}
*/
