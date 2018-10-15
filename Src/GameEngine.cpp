/**
* @file GameEngine.cpp
*/
#include "GameEngine.h"
#include "GLFWEW.h"
#include "Audio.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Constants.h"
//using namespace Charactor;

//トップNode
Node* GameEngine::world;

namespace /* unnamed */ {
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
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		return vao;
	}


} // unnamed namespace

  /**
  * ゲームエンジンのインスタンスを取得する.
  *
  * @return ゲームエンジンのインスタンス.
  */
GameEngine& GameEngine::Instance()
{
	static GameEngine instance;
	return instance;
}

/**
* ゲームエンジンを初期化する.
*
* @param w     ウィンドウの描画範囲の幅(ピクセル).
* @param h     ウィンドウの描画範囲の高さ(ピクセル).
* @param title ウィンドウタイトル(UTF-8の0終端文字列).
*
* @retval true  初期化成功.
* @retval false 初期化失敗.
*
* Run関数を呼び出す前に、一度だけ呼び出しておく必要がある.
* 一度初期化に成功すると、以後の呼び出しではなにもせずにtrueを返す.
*/
bool GameEngine::Init(int w, int h, const char* title)
{
	if (isInitialized) {
		return true;
	}
	if (!GLFWEW::Window::Instance().Init(w, h, title)) {
		return false;
	}

	width = w;
	height = h;
	vbo = CreateVBO(sizeof(vertices), vertices);
	ibo = CreateIBO(sizeof(indices), indices);
	vao = CreateVAO(vbo, ibo);
	uboLight = UniformBuffer::Create(sizeof(InterfaceBlock::LightData),
		InterfaceBlock::BINDINGPOINT_LIGHTDATA, "LightData");
	uboPostEffect = UniformBuffer::Create(sizeof(InterfaceBlock::PostEffectData),
		InterfaceBlock::BINDINGPOINT_POSTEFFECTDATA, "PostEffectData");
	

	offscreen = OffscreenBuffer::Create(width, height);
	if (!vbo || !ibo || !vao || !uboLight || !uboPostEffect || !offscreen) {
		std::cerr << "ERROR: GameEngineの初期化に失敗" << std::endl;
		return false;
	}

	static const char* const shaderNameList[][3] = {
		{ "Tutorial", "Res/Tutorial.vert", "Res/Tutorial.frag" },
		{ "ColorFilter", "Res/ColorFilter.vert", "Res/ColorFilter.frag" },
		{ "NonLighting", "Res/NonLighting.vert", "Res/NonLighting.frag" },
		};
	shaderMap.reserve(sizeof(shaderNameList) / sizeof(shaderNameList[0]));
	for (auto& e : shaderNameList) {
		Shader::ProgramPtr program = Shader::Program::Create(e[1], e[2]);
		if (!program) {
			return false;
			
		}
		shaderMap.insert(std::make_pair(std::string(e[0]), program));
		
	}

	meshBuffer = Mesh::Buffer::Create(10 * 1024, 30 * 1024);
	if (!meshBuffer) {
		std::cerr << "ERROR: GameEngineの初期化に失敗" << std::endl;
		return false;
	}
	entityBuffer = Entity::Buffer::Create(1024, sizeof(InterfaceBlock::VertexData),
		InterfaceBlock::BINDINGPOINT_VERTEXDATA, "VertexData");
	if (!entityBuffer) {
		std::cerr << "ERROR: GameEngineの初期化に失敗" << std::endl;
		return false;
	}

	shaderMap["Tutorial"]->UniformBlockBinding(*entityBuffer->UniformBuffer());
	shaderMap["Tutorial"]->UniformBlockBinding(*uboLight);
	shaderMap["ColorFilter"]->UniformBlockBinding(*uboPostEffect);

	rand.seed(std::random_device()());

	fontRenderer.Init(1024, glm::vec2(static_cast<float>(w), static_cast<float>(h)), 32);

	isInitialized = true;

	//トップノード
	world = new Node();
	return true;
}

/**
* ゲームを実行する.
*/
void GameEngine::Run()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	const double delta = 1.0 / (double)FPS;
	double prevTime = glfwGetTime();
	double time = 0.0f;
	while (!window.ShouldClose()) {
		
		time = glfwGetTime();
		if (time - prevTime < delta) {
			//時間に達していなければスキップ
			continue;
		}
		//オーバーした分を足す
		prevTime = time + (time - prevTime - delta);


		Update(delta);
		Render();
		window.SwapBuffers();
	}
}

/**
* 状態更新関数を設定する.
*
* @param func 設定する更新関数.
*/
void GameEngine::UpdateFunc(const UpdateFuncType& func)
{
	updateFunc = func;


}

/**
* テクスチャを読み込む.
*
* @param filename テクスチャファイル名.
*
* @retval true  読み込み成功.
* @retval false 読み込み失敗.
*/
bool GameEngine::LoadTextureFromFile(const char* filename,const int c, const int v)
{
	if (GetTexture(filename)) {
		return true;
	}
	TexturePtr texture = Texture::LoadFromFile(filename,c,v);
	
	if (!texture) {
		return false;
	}
	textureBuffer.insert(std::make_pair(std::string(filename), texture));
	return true;
}

/**
* テクスチャを取得する.
*
* @param filename テクスチャファイル名.
*
* @return filenameに対応するテクスチャへのポインタ.
*         対応するテクスチャが見つからない場合はnullptrを返す.
*/
const TexturePtr& GameEngine::GetTexture(const char* filename) const
{
	const auto itr = textureBuffer.find(filename);
	if (itr != textureBuffer.end()) {
		return itr->second;
	}
	static const TexturePtr dummy;
	return dummy;
}

/**
* エンティティを追加する.
*
* @param groupId  エンティティのグループID.
* @param pos      エンティティの座標.
* @param meshName エンティティの表示に使用するメッシュ名.
* @param texName  エンティティの表示に使うテクスチャファイル名.
* @param func     エンティティの状態を更新する関数(または関数オブジェクト).
* @param shader   エンティティの表示に使うシェーダ名.
*
* @return 追加したエンティティへのポインタ.
*/
Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos,
	const char* meshName, const char* texName, Entity::Entity::UpdateFuncType func,
	const char* shader)
{
	decltype(shaderMap)::const_iterator itr = shaderMap.end();
	if (shader) {
		itr = shaderMap.find(shader);
		
	}
	if (itr == shaderMap.end()) {
		itr = shaderMap.find("Tutorial");
		if (itr == shaderMap.end()) {
			return nullptr;
			
		}
		
	}

	const Mesh::MeshPtr& mesh = meshBuffer->GetMesh(meshName);
	const TexturePtr& tex = textureBuffer.find(texName)->second;
	return entityBuffer->AddEntity(groupId, pos, tex, itr->second, func);
}

/**
*　エンティティを削除する.
*
* @param 削除するエンティティのポインタ.
*/
void GameEngine::RemoveEntity(Entity::Entity* e)
{
	entityBuffer->RemoveEntity(e);
}


/**
*プレイヤーキャラクターを作成する
*/
Char::Charactor* GameEngine::CreatePlayerChara() {
	PlayerCharactor* c = new PlayerCharactor();

	Char::Charactor::Player = c;

	//キャラクターセット作成
	c->CreateChara();
	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	c->Hp(20);
	return c;
}

/**
*敵キャラクターを作成する
*/
Char::Charactor* GameEngine::CreateEnemyChara() {
	EnemyCharactor* c = new EnemyCharactor();
	//キャラクターセット作成
	c->CreateChara();

	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	c->SetColor({1.0f,0.0f,0.0f,0.0f});
	c->Hp(3);

	return c;
}

/**
*ライフアイテムを作成する
*/
Char::Item* GameEngine::CreateLifeItemChara() {
	LifeItem* c = new LifeItem();
	//キャラクターセット作成
	c->CreateItem("Res/Life.bmp");

	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	return c;
}
/**
*ライフアイテムを作成する
*/
Char::Item* GameEngine::CreateTimeItemChara() {
	TimeItem* c = new TimeItem();
	//キャラクターセット作成
	c->CreateItem("Res/Time.bmp");

	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	return c;
}

/**
* ライトを設定する.
*
* @param indes  設定するライトのインデックス.
* @param light  ライトデータ.
*/
void GameEngine::Light(int index, const InterfaceBlock::PointLight& light)
{
	if (index < 0 || index >= InterfaceBlock::maxLightCount) {
		std::cerr << "WARNING: '" << index << "'は不正なライトインデックスです" << std::endl;
		return;
	}
	lightData.light[index] = light;
}

/**
* ライトを取得する.
*
* @param index 取得するライトのインデックス.
*
* @return ライトデータ.
*/
const InterfaceBlock::PointLight& GameEngine::Light(int index) const
{
	if (index < 0 || index >= InterfaceBlock::maxLightCount) {
		std::cerr << "WARNING: '" << index << "'は不正なライトインデックスです" << std::endl;
		static const InterfaceBlock::PointLight dummy;
		return dummy;
	}
	return lightData.light[index];
}

/**
* 視点の位置と姿勢を設定する.
*
* @param cam 設定するカメラデータ.
*/
void GameEngine::Camera(const GameEngine::CameraData& cam)
{
	camera = cam;
}

/**
* 視点の位置と姿勢を取得する.
*
* @return カメラデータ.
*/
const GameEngine::CameraData& GameEngine::Camera() const
{
	return camera;
}

/**
* 乱数オブジェクトを取得する.
*
* @return 乱数オブジェクト.
*/
std::mt19937& GameEngine::Rand()
{
	return rand;
}

/**
* ゲームパッドの状態を取得する.
*/
const GamePad& GameEngine::GetGamePad() const
{
	return GLFWEW::Window::Instance().GetGamePad();
}

/**
* 衝突解決ハンドラを設定する.
*
* @param gid0    衝突対象のグループID.
* @param gid1    衝突対象のグループID.
* @param handler 衝突解決ハンドラ.
*
* 衝突が発生し衝突解決ハンドラが呼びされるとき、より小さいグループIDを持つエンティティから先に渡される.
* ここで指定したグループIDの順序とは無関係であることに注意すること.
* ex)
*   CollisionHandler(10, 1, Func)
*   というコードでハンドラを登録したとする. 衝突が発生すると、
*   Func(グループID=1のエンティティ、グループID=10のエンティティ)
*   のように呼び出される.
*/
void GameEngine::CollisionHandler(int gid0, int gid1,
	Entity::CollisionHandlerType handler)
{
	entityBuffer->CollisionHandler(gid0, gid1, handler);
}

/**
* 衝突解決ハンドラを取得する.
*
* @param gid0 衝突対象のグループID.
* @param gid1 衝突対象のグループID.
*
* @return 衝突解決ハンドラ.
*/
const Entity::CollisionHandlerType& GameEngine::CollisionHandler(
	int gid0, int gid1) const
{
	return entityBuffer->CollisionHandler(gid0, gid1);
}

/**
* 衝突解決ハンドラのリストをクリアする.
*/
void GameEngine::ClearCollisionHandlerList()
{
	entityBuffer->ClearCollisionHandlerList();
}

/**
* @copydoc Audio::Initialize
*/
bool GameEngine::InitAudio(const char* acfPath, const char* acbPath,
	const char* awbPath, const char* dspBusName, size_t playerCount)
{
	return Audio::Initialize(acfPath, acbPath, awbPath, dspBusName, playerCount);
}

/**
* @copydoc Audio::Play
*/
void GameEngine::PlayAudio(int playerId, int cueId)
{
	Audio::Play(playerId, cueId);
}

/**
* @copydoc Audio::Stop
*/
void GameEngine::StopAudio(int playerId)
{
	Audio::Stop(playerId);
}

/**
* 環境光を設定する.
*
* @param color 環境光の明るさ.
*/
void GameEngine::AmbientLight(const glm::vec4& color)
{
	lightData.ambientColor = color;
}

/**
* 環境光を取得する.
*
* @return 環境光の明るさ.
*/
const glm::vec4& GameEngine::AmbientLight() const
{
	return lightData.ambientColor;
}

/**
* 状態更新関数を取得する.
*
* @return 設定されている更新関数.
*/
const GameEngine::UpdateFuncType& GameEngine::UpdateFunc() const
{
	return updateFunc;
}

/**
* デストラクタ.
*/
GameEngine::~GameEngine()
{
	updateFunc = nullptr;

	Audio::Destroy();

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
* ゲームの状態を更新する.
*
* @param delta 前回の更新からの経過時間(秒).
*/
void GameEngine::Update(double delta)
{
	//リセットフラグがあるなら削除
	if (reset) {
		reset = false;
		DestroyAllEntity();
	}
	GLFWEW::Window::Instance().UpdateGamePad();

	fontRenderer.MapBuffer();

	if (updateFunc) {
		updateFunc(delta);
	}

	const glm::mat4x4 matProj = glm::perspective(glm::radians(zoom),
		static_cast<float>(width) / static_cast<float> (height), 1.0f, 200.0f);
	const glm::mat4x4 matView = glm::lookAt(camera.position, camera.target, camera.up);
	entityBuffer->Update(delta, matView, matProj);
	fontRenderer.UnmapBuffer();
}

/**
* NodeとEntityを全て削除.
*/
void GameEngine::DestroyAllEntity() {
	
	//キャラクター全削除
	for (int itr = 0; Char::Charactor::CharList.begin() + itr < Char::Charactor::CharList.end(); itr++) {
		auto itrr = Char::Charactor::CharList.begin() + itr;
		
		static_cast<Char::Charactor*>(*itrr)->removeNode();

	}
	Char::Charactor::CharList.clear();

	//アイテム全削除
	for (int itr = 0; Char::Item::ItemList.begin() + itr < Char::Item::ItemList.end(); itr++) {
		auto itrr = Char::Item::ItemList.begin() + itr;

		static_cast<Char::Item*>(*itrr)->removeNode();

	}
	Char::Item::ItemList.clear();
	
	//全エンティティ削除
	entityBuffer->DestoroyAllEntity();
	//worldのNodeListを空にする
	world->nodeList.clear();	
	meshBuffer.reset();
	meshBuffer = Mesh::Buffer::Create(10 * 1024, 30 * 1024);
	entityBuffer.reset();
	entityBuffer = Entity::Buffer::Create(1024, sizeof(InterfaceBlock::VertexData),
		InterfaceBlock::BINDINGPOINT_VERTEXDATA, "VertexData");
}

/**
* ゲームの状態を描画する.
*/
void GameEngine::Render() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	uboLight->BufferSubData(&lightData);
	
	//glBindVertexArray(vao);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_BLEND);
	glBindVertexArray(vao);

	//entityBuffer->Draw(meshBuffer);
	entityBuffer->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_BLEND);
	//glBindVertexArray(vao);
	//entityBuffer->Draw(meshBuffer);

	const Shader::ProgramPtr& progColorFilter = shaderMap.find("ColorFilter")->second;
	progColorFilter->UseProgram();
	InterfaceBlock::PostEffectData postEffect;
	uboPostEffect->BufferSubData(&postEffect);
	progColorFilter->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexutre());
	glDrawElements(GL_TRIANGLES, renderingParts[GameState::EntityGroupId::EntityGroupId_Display].size,
		GL_UNSIGNED_INT, renderingParts[GameState::EntityGroupId::EntityGroupId_Display].offset);

	fontRenderer.Draw();
}