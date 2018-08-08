/**
* @file Main.cpp
*/
/// エントリーポイント.
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

/// エンティティの衝突グループID.
enum EntityGroupId
{
	EntityGroupId_Player,
	EntityGroupId_PlayerShot,
	EntityGroupId_Enemy,
	EntityGroupId_EnemyShot,
	EntityGroupId_Others,
};

/// 音声プレイヤーのID.
enum AudioPlayerId
{
	AudioPlayerId_Shot, ///< 自機のショット音.
	AudioPlayerId_Bomb, ///< 爆発音.
	AudioPlayerId_BGM, ///<　BGM.
	AudioPlayerId_Max, ///< 再生制御用プレイヤーの総数.

	AudioPlayerId_UI = AudioPlayerId_Shot, ///<　ユーザーインターフェイス操作音.
};

/// 衝突形状リスト.
/*
static const Entity::CollisionData collisionDataList[] = {
	{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
	{ glm::vec3(-0.5f, -0.5f, -1.0f), glm::vec3(0.5f, 0.5f, 1.0f) },
	{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f) },
	{ glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f) },
};
*/


/// 3Dベクトル型.
struct Vector3
{
	float x, y, z;
};

/// RGBAカラー型.
struct Color
{
	float r, g, b, a;
};
/*
/// 頂点データ型.
struct Vertex
{
glm::vec3 position; ///< 座標.
glm::vec4 color; ///< 色.
glm::vec2 texCoord; ///< テクスチャ座標.
};
*/


/// 頂点データ.
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
/// インデックスデータ.
const GLuint indices[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
	10, 11, 12, 12, 13, 10,
};

*/

/*
/// 頂点シェーダのパラメータ型.
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


const int maxLightCount = 4; ///< ライトの数.

/**
* ライトデータ(点光源).
*/
struct PointLight
{
	glm::vec4 position; ///< 座標(ワールド座標系).
	glm::vec4 color; ///< 明るさ.
};

/**
* ライティングパラメータをシェーダに転送するための構造体.
struct LightData
{
glm::vec4 ambientColor; ///< 環境光.
PointLight light[maxLightCount]; ///< ライトのリスト.
};
*/


/**
* ポストエフェクトデータをシェーダに転送するための構造体.
struct PostEffectData
{
glm::mat4x4 matColor; ///< 色変換行列.
};
*/

/*
/// バインディングポイント.
enum BindingPoint
{
BINDINGPOINT_VERTEXDATA, ///< 頂点シェーダ用パラメータのバインディングポイント.
BINDINGPOINT_LIGHTDATA, ///< ライティングパラメータ用のバインディングポイント.
BINDINGPOINT_POSTEFFECTDATA, ///< ポストエフェクトパラメータ用のバインディングポイント.
};
*/


/**
* 部分描画データ.
struct RenderingPart
{
GLvoid* offset; ///< 描画開始インデックスのバイトオフセット.
GLsizei size; ///< 描画するインデックス数.
};
*/


/**
* RenderingPartを作成する.
*
* @param offset 描画開始インデックスのオフセット(インデックス単位).
* @param size 描画するインデックス数.
*
* @return 作成した部分描画オブジェクト.
*/
/*
constexpr RenderingPart MakeRenderingPart(GLsizei offset, GLsizei size) {
	return { reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)), size };
}
*/


/**
* 部分描画データリスト.
*/
/*
static const RenderingPart renderingParts[] = {
	MakeRenderingPart(0, 12),
	MakeRenderingPart(12, 6),
};
*/


/**
* Vertex Buffer Objectを作成する.
*
* @param size 頂点データのサイズ.
* @param data 頂点データへのポインタ.
*
* @return 作成したVBO.
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
* Index Buffer Objectを作成する.
*
* @param size インデックスデータのサイズ.
* @param data インデックスデータへのポインタ.
*
* @return 作成したIBO.
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
* 頂点アトリビュートを設定する.
*
* @param index 頂点アトリビュートのインデックス.
* @param cls   頂点データ型名.
* @param mbr   頂点アトリビュートに設定するclsのメンバ変数名.
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
* Vertex Array Objectを作成する.
*
* @param vbo VAOに関連付けられるVBO.
* @param ibo VAOに関連付けられるIBO.
*
* @return 作成したVAO.
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
* 敵の円盤の状態を更新する.
*/
/*
struct UpdateToroid
{
	//explicit UpdateToroid(const Entity::BufferPtr& buffer) : entityBuffer(buffer) {}

	void operator()(Entity::Entity& entity, double delta)
	{
		// 範囲外に出たら削除する.
		const glm::vec3 pos = entity.Position();
		if (std::abs(pos.x) > 40.0f || std::abs(pos.z) > 40.0f) {
			//entityBuffer->RemoveEntity(&entity);
			GameEngine::Instance().RemoveEntity(&entity);
			return;
		}

		// 円盤を回転させる.
		float rot = glm::angle(entity.Rotation());
		//rot += glm::radians(15.0f) * static_cast<float>(delta);
		rot += glm::radians(60.0f) * static_cast<float>(delta);
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity.Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		// 頂点シェーダーのパラメータをUBOにコピーする.
		//VertexData data;
		
	}

	//Entity::BufferPtr entityBuffer;
};
*/


/**
* 自機の弾の更新.
*/
/*
struct UpdatePlayerShot
{
	void operator()(Entity::Entity& entity, double delta)
	{
		// 範囲外に出たら削除する.
		const glm::vec3 pos = entity.Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
			entity.Destroy();
			return;
		}
	}
};
*/


/**
* 爆発の更新.
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
		const float variation = static_cast<float>(timer * 4); // 変化量.
		entity.Scale(glm::vec3(static_cast<float>(1 + variation))); // 徐々に拡大する.
																	// 時間経過で色と透明度を変化させる.
		static const glm::vec4 color[] = {
			glm::vec4(1.0f, 1.0f, 0.75f, 1),
			glm::vec4(1.0f, 0.5f, 0.1f, 1),
			glm::vec4(0.25f, 0.1f, 0.1f, 0),
		};
		const glm::vec4 col0 = color[static_cast<int>(variation)];
		const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
		const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
		entity.Color(newColor);
		// Y軸回転させる.
		glm::vec3 euler = glm::eulerAngles(entity.Rotation());
		euler.y += glm::radians(60.0f) * static_cast<float>(delta);
		entity.Rotation(glm::quat(euler));
	}

	double timer = 0;
};
*/


/**
* 自機の更新.
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
				pos.x -= 0.3f; // 自機の中心から左に0.3ずらした位置が1つめの発射点.
				for (int i = 0; i < 2; ++i) {
					if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, pos,
						"NormalShot", "Res/Player.bmp", UpdatePlayerShot())) {
						p->Velocity(glm::vec3(0, 0, 80));
						p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
						
					}
					pos.x += 0.6f; // 中心からに右に0.3ずらした位置が2つめの発射点.
					
				}
				shotInterval += 0.25; // 秒間4連射.
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
* ゲームの状態を更新する.
*
* @param entityBuffer 敵エンティティ追加先のエンティティバッファ.
* @param meshBuffer   敵エンティティのメッシュを管理しているメッシュバッファ.
* @param tex          敵エンティティ用のテクスチャ.
* @param prog         敵エンティティ用のシェーダープログラム.
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
* ゲーム状態の更新.
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
* 自機の弾と敵の衝突処理.
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

/// 頂点シェーダ.
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

/// フラグメントシェーダ.
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
* シェーダコードをコンパイルする.
*
* @param type シェーダの種類.
* @param string シェーダコードへのポインタ.
*
* @return 作成したシェーダオブジェクト.
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
				std::cerr << "ERROR: シェーダのコンパイルに失敗" << buf.data() << std::endl;
			}
			glDeleteShader(shader);
		}
		return 0;
	}
	return shader;
}

/**
* シェーダプログラムを作成する.
*
* @param vsCode 頂点シェーダコードへのポインタ.
* @param fsCode フラグメントシェーダコードへのポインタ.
*
* @return 作成したプログラムオブジェクト.
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
				std::cerr << "ERROR: シェーダのリンクに失敗" << buf.data() << std::endl;
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


	// GLFWの初期化.
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

	/// テクスチャデータ.
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


	//オフスクリーンバッファを作成
	const OffscreenBufferPtr offscreen = OffscreenBuffer::Create(800, 600);



	glEnable(GL_DEPTH_TEST);

	// メインループ.
	while (!window.ShouldClose()) {
		Update(entityBuffer, meshBuffer, texToroid, progTutorial);
		//window.viewPort();
#define USE_OFFSCREEN_BUFFER 1
#ifdef USE_OFFSCREEN_BUFFER
		//オフスクリーンバッファに設定
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());
#endif
		glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 視点を回転移動させる.
		
		//static float degree = 0.0f;
		//degree += 0.1f;
		//if (degree >= 360.0f) { degree -= 360.0f; }
		//const glm::vec3 viewPos = glm::rotate(
		//	glm::mat4(), glm::radians(degree), glm::vec3(0, 1, 0)) * glm::vec4(2, 3, 3, 1);
		
		const glm::vec3 viewPos = glm::vec4(0, 20, -20, 1);

		//glUseProgram(shaderProgram);
		progTutorial->UseProgram();



		// 座標変換行列を作成してシェーダに転送する.
			//const GLint matMVPLoc = glGetUniformLocation(shaderProgram, "matMVP");
		//if (matMVPLoc >= 0) {
		const glm::mat4x4 matProj =
			glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		const glm::mat4x4 matView =
			glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		const glm::mat4x4 matMVP = matProj * matView;
		//glUniformMatrix4fv(matMVPLoc, 1, GL_FALSE, &matMVP[0][0]);

	//}

		//光設定
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

		//オフスクリーンバッファから通常バッファに戻す
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//ライトシェーダー　一時コメントアウト
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
		
		
		
		//モノトーンシェーダー
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
