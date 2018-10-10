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
* �Q�[����Ԃ̍X�V.
*/
class Update
{
public:

	void operator()(double delta)
	{
		
	}

private:
	bool isInitialized = false;
	double interval = 0;

	Entity::Entity* pPlayer = nullptr;
};

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

	//��e�N�X�`���[
	game.LoadTextureFromFile("");

	game.LoadTextureFromFile("Res/Toroid.bmp");
	game.LoadTextureFromFile("Res/Player.bmp");

	game.LoadTextureFromFile("Res/SpaceSphere.bmp");

	game.LoadTextureFromFile("Res/Wall.bmp");
	
	game.LoadTextureFromFile("Res/Life.bmp",0xffffff,10);

	game.LoadTextureFromFile("Res/Star.bmp");
	
	game.LoadTextureFromFile("Res/Cloud.bmp");

	game.LoadFontFromFile("Res/Font.fnt");
	
	game.UpdateFunc(GameState::Title());
	game.Run();
	return 0;
}

