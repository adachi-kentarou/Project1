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

//�g�b�vNode
Node* GameEngine::world;

namespace /* unnamed */ {
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
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		return vao;
	}


} // unnamed namespace

  /**
  * �Q�[���G���W���̃C���X�^���X���擾����.
  *
  * @return �Q�[���G���W���̃C���X�^���X.
  */
GameEngine& GameEngine::Instance()
{
	static GameEngine instance;
	return instance;
}

/**
* �Q�[���G���W��������������.
*
* @param w     �E�B���h�E�̕`��͈͂̕�(�s�N�Z��).
* @param h     �E�B���h�E�̕`��͈͂̍���(�s�N�Z��).
* @param title �E�B���h�E�^�C�g��(UTF-8��0�I�[������).
*
* @retval true  ����������.
* @retval false ���������s.
*
* Run�֐����Ăяo���O�ɁA��x�����Ăяo���Ă����K�v������.
* ��x�������ɐ�������ƁA�Ȍ�̌Ăяo���ł͂Ȃɂ�������true��Ԃ�.
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
		std::cerr << "ERROR: GameEngine�̏������Ɏ��s" << std::endl;
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
		std::cerr << "ERROR: GameEngine�̏������Ɏ��s" << std::endl;
		return false;
	}
	entityBuffer = Entity::Buffer::Create(1024, sizeof(InterfaceBlock::VertexData),
		InterfaceBlock::BINDINGPOINT_VERTEXDATA, "VertexData");
	if (!entityBuffer) {
		std::cerr << "ERROR: GameEngine�̏������Ɏ��s" << std::endl;
		return false;
	}

	shaderMap["Tutorial"]->UniformBlockBinding(*entityBuffer->UniformBuffer());
	shaderMap["Tutorial"]->UniformBlockBinding(*uboLight);
	shaderMap["ColorFilter"]->UniformBlockBinding(*uboPostEffect);

	rand.seed(std::random_device()());

	fontRenderer.Init(1024, glm::vec2(static_cast<float>(w), static_cast<float>(h)), 32);

	isInitialized = true;

	//�g�b�v�m�[�h
	world = new Node();
	return true;
}

/**
* �Q�[�������s����.
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
			//���ԂɒB���Ă��Ȃ���΃X�L�b�v
			continue;
		}
		//�I�[�o�[�������𑫂�
		prevTime = time + (time - prevTime - delta);


		Update(delta);
		Render();
		window.SwapBuffers();
	}
}

/**
* ��ԍX�V�֐���ݒ肷��.
*
* @param func �ݒ肷��X�V�֐�.
*/
void GameEngine::UpdateFunc(const UpdateFuncType& func)
{
	updateFunc = func;


}

/**
* �e�N�X�`����ǂݍ���.
*
* @param filename �e�N�X�`���t�@�C����.
*
* @retval true  �ǂݍ��ݐ���.
* @retval false �ǂݍ��ݎ��s.
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
* �e�N�X�`�����擾����.
*
* @param filename �e�N�X�`���t�@�C����.
*
* @return filename�ɑΉ�����e�N�X�`���ւ̃|�C���^.
*         �Ή�����e�N�X�`����������Ȃ��ꍇ��nullptr��Ԃ�.
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
* �G���e�B�e�B��ǉ�����.
*
* @param groupId  �G���e�B�e�B�̃O���[�vID.
* @param pos      �G���e�B�e�B�̍��W.
* @param meshName �G���e�B�e�B�̕\���Ɏg�p���郁�b�V����.
* @param texName  �G���e�B�e�B�̕\���Ɏg���e�N�X�`���t�@�C����.
* @param func     �G���e�B�e�B�̏�Ԃ��X�V����֐�(�܂��͊֐��I�u�W�F�N�g).
* @param shader   �G���e�B�e�B�̕\���Ɏg���V�F�[�_��.
*
* @return �ǉ������G���e�B�e�B�ւ̃|�C���^.
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
*�@�G���e�B�e�B���폜����.
*
* @param �폜����G���e�B�e�B�̃|�C���^.
*/
void GameEngine::RemoveEntity(Entity::Entity* e)
{
	entityBuffer->RemoveEntity(e);
}


/**
*�v���C���[�L�����N�^�[���쐬����
*/
Char::Charactor* GameEngine::CreatePlayerChara() {
	PlayerCharactor* c = new PlayerCharactor();

	Char::Charactor::Player = c;

	//�L�����N�^�[�Z�b�g�쐬
	c->CreateChara();
	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	c->Hp(20);
	return c;
}

/**
*�G�L�����N�^�[���쐬����
*/
Char::Charactor* GameEngine::CreateEnemyChara() {
	EnemyCharactor* c = new EnemyCharactor();
	//�L�����N�^�[�Z�b�g�쐬
	c->CreateChara();

	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	c->SetColor({1.0f,0.0f,0.0f,0.0f});
	c->Hp(3);

	return c;
}

/**
*���C�t�A�C�e�����쐬����
*/
Char::Item* GameEngine::CreateLifeItemChara() {
	LifeItem* c = new LifeItem();
	//�L�����N�^�[�Z�b�g�쐬
	c->CreateItem("Res/Life.bmp");

	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	return c;
}
/**
*���C�t�A�C�e�����쐬����
*/
Char::Item* GameEngine::CreateTimeItemChara() {
	TimeItem* c = new TimeItem();
	//�L�����N�^�[�Z�b�g�쐬
	c->CreateItem("Res/Time.bmp");

	c->SetFunc(&c->UpdateFnc);
	c->SetRotationOrder(Node::order::YXZ);
	return c;
}

/**
* ���C�g��ݒ肷��.
*
* @param indes  �ݒ肷�郉�C�g�̃C���f�b�N�X.
* @param light  ���C�g�f�[�^.
*/
void GameEngine::Light(int index, const InterfaceBlock::PointLight& light)
{
	if (index < 0 || index >= InterfaceBlock::maxLightCount) {
		std::cerr << "WARNING: '" << index << "'�͕s���ȃ��C�g�C���f�b�N�X�ł�" << std::endl;
		return;
	}
	lightData.light[index] = light;
}

/**
* ���C�g���擾����.
*
* @param index �擾���郉�C�g�̃C���f�b�N�X.
*
* @return ���C�g�f�[�^.
*/
const InterfaceBlock::PointLight& GameEngine::Light(int index) const
{
	if (index < 0 || index >= InterfaceBlock::maxLightCount) {
		std::cerr << "WARNING: '" << index << "'�͕s���ȃ��C�g�C���f�b�N�X�ł�" << std::endl;
		static const InterfaceBlock::PointLight dummy;
		return dummy;
	}
	return lightData.light[index];
}

/**
* ���_�̈ʒu�Ǝp����ݒ肷��.
*
* @param cam �ݒ肷��J�����f�[�^.
*/
void GameEngine::Camera(const GameEngine::CameraData& cam)
{
	camera = cam;
}

/**
* ���_�̈ʒu�Ǝp�����擾����.
*
* @return �J�����f�[�^.
*/
const GameEngine::CameraData& GameEngine::Camera() const
{
	return camera;
}

/**
* �����I�u�W�F�N�g���擾����.
*
* @return �����I�u�W�F�N�g.
*/
std::mt19937& GameEngine::Rand()
{
	return rand;
}

/**
* �Q�[���p�b�h�̏�Ԃ��擾����.
*/
const GamePad& GameEngine::GetGamePad() const
{
	return GLFWEW::Window::Instance().GetGamePad();
}

/**
* �Փˉ����n���h����ݒ肷��.
*
* @param gid0    �ՓˑΏۂ̃O���[�vID.
* @param gid1    �ՓˑΏۂ̃O���[�vID.
* @param handler �Փˉ����n���h��.
*
* �Փ˂��������Փˉ����n���h�����Ăт����Ƃ��A��菬�����O���[�vID�����G���e�B�e�B�����ɓn�����.
* �����Ŏw�肵���O���[�vID�̏����Ƃ͖��֌W�ł��邱�Ƃɒ��ӂ��邱��.
* ex)
*   CollisionHandler(10, 1, Func)
*   �Ƃ����R�[�h�Ńn���h����o�^�����Ƃ���. �Փ˂���������ƁA
*   Func(�O���[�vID=1�̃G���e�B�e�B�A�O���[�vID=10�̃G���e�B�e�B)
*   �̂悤�ɌĂяo�����.
*/
void GameEngine::CollisionHandler(int gid0, int gid1,
	Entity::CollisionHandlerType handler)
{
	entityBuffer->CollisionHandler(gid0, gid1, handler);
}

/**
* �Փˉ����n���h�����擾����.
*
* @param gid0 �ՓˑΏۂ̃O���[�vID.
* @param gid1 �ՓˑΏۂ̃O���[�vID.
*
* @return �Փˉ����n���h��.
*/
const Entity::CollisionHandlerType& GameEngine::CollisionHandler(
	int gid0, int gid1) const
{
	return entityBuffer->CollisionHandler(gid0, gid1);
}

/**
* �Փˉ����n���h���̃��X�g���N���A����.
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
* ������ݒ肷��.
*
* @param color �����̖��邳.
*/
void GameEngine::AmbientLight(const glm::vec4& color)
{
	lightData.ambientColor = color;
}

/**
* �������擾����.
*
* @return �����̖��邳.
*/
const glm::vec4& GameEngine::AmbientLight() const
{
	return lightData.ambientColor;
}

/**
* ��ԍX�V�֐����擾����.
*
* @return �ݒ肳��Ă���X�V�֐�.
*/
const GameEngine::UpdateFuncType& GameEngine::UpdateFunc() const
{
	return updateFunc;
}

/**
* �f�X�g���N�^.
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
* �Q�[���̏�Ԃ��X�V����.
*
* @param delta �O��̍X�V����̌o�ߎ���(�b).
*/
void GameEngine::Update(double delta)
{
	//���Z�b�g�t���O������Ȃ�폜
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
* Node��Entity��S�č폜.
*/
void GameEngine::DestroyAllEntity() {
	
	//�L�����N�^�[�S�폜
	for (int itr = 0; Char::Charactor::CharList.begin() + itr < Char::Charactor::CharList.end(); itr++) {
		auto itrr = Char::Charactor::CharList.begin() + itr;
		
		static_cast<Char::Charactor*>(*itrr)->removeNode();

	}
	Char::Charactor::CharList.clear();

	//�A�C�e���S�폜
	for (int itr = 0; Char::Item::ItemList.begin() + itr < Char::Item::ItemList.end(); itr++) {
		auto itrr = Char::Item::ItemList.begin() + itr;

		static_cast<Char::Item*>(*itrr)->removeNode();

	}
	Char::Item::ItemList.clear();
	
	//�S�G���e�B�e�B�폜
	entityBuffer->DestoroyAllEntity();
	//world��NodeList����ɂ���
	world->nodeList.clear();	
	meshBuffer.reset();
	meshBuffer = Mesh::Buffer::Create(10 * 1024, 30 * 1024);
	entityBuffer.reset();
	entityBuffer = Entity::Buffer::Create(1024, sizeof(InterfaceBlock::VertexData),
		InterfaceBlock::BINDINGPOINT_VERTEXDATA, "VertexData");
}

/**
* �Q�[���̏�Ԃ�`�悷��.
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