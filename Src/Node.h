#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

/*
�m�[�h�N���X
*/
class Node {
public:
	//��]����
	enum order {
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZXY = 4,
		ZYX = 5
	};

	//�ړ�
	void Position(const glm::vec3& v) { position = v; }
	const glm::vec3& Position() const { return position; }
	//��]
	void Rotation(const glm::vec3& q) { rotation = q; }
	const glm::vec3& Rotation() const { return rotation; }
	//�g��k��
	void Scale(const glm::vec3& v) { scale = v; }
	const glm::vec3& Scale() const { return scale; }

	void Color(const glm::vec4& v) { color = v; }
	const glm::vec4& Color() const { return color; }

	//�ړ�
	void Velocity(const glm::vec3& v) { velocity = v; }
	const glm::vec3& Velocity() const { return velocity; }
	void Node::SetPosition(const glm::vec3& p);
	glm::mat4 Node::GetPosition() { return glposition; };

	//��]
	void RotateVelocity(const glm::vec3& v) { rotatevelocity = v; }
	const glm::vec3& RotateVelocity() const { return rotatevelocity; }
	void Node::SetRotation(const glm::vec3& r);
	glm::mat4 Node::GetRotation() { return glrotate; };

	//�g��k��
	void ScaleVelocity(const glm::vec3& v) { scalevelocity = v; }
	const glm::vec3& ScaleVelocity() const { return scalevelocity; }
	void Node::SetScale(const glm::vec3& s);
	glm::mat4 Node::GetScale() { return glscale; };

	//�F
	void ColorVelocity(const glm::vec4& c) { colorvelocity = c; }
	const glm::vec4& ColorVelocity() const { return colorvelocity; }
	void Node::SetColor(const glm::vec4& c);
	glm::vec4& Node::GetColor() { return mycolor; };

	//��]����
	void SetRotationOrder(order o) { rotationorder = o; };

	//�}�g���N�X�v�Z
	glm::mat4 CalcModelMatrix() const;

	//�}�g���N�X�擾
	glm::mat4 GetMatrix() { return mymatrix; };


	//visible�擾
	bool GetVisible() { return visible; };
	//visible�ݒ�
	void SetVisible(bool b) { visible = b; };
	//combinedvisible�擾
	bool GetCombinedVisible() { return combinedvisible; };

	Node* GetNode() { return this; };

	//�R���X�g���N�^
	Node();
	Node(glm::mat4* m);
	//�f�X�g���N�^
	virtual ~Node();

	//�m�[�h���蓖��
	void addNode(Node* n);
	//�m�[�h���O��
	void removeNode();

	//�m�[�h�X�V
	void Update();

	//�m�[�h���X�g
	std::vector<Node*> nodeList;
	//���M�̃}�g���N�X
	glm::mat4* MyMatrix;
	
	

protected:
	//�e�m�[�h
	Node * parentnode = nullptr;

	glm::mat4 mymatrix;///�}�g���N�X

	glm::vec4 mycolor = glm::vec4({ 1.0f,1.0f, 1.0f, 1.0f });///�F

	glm::vec3 position = glm::vec3(0, 0, 0); ///< ���W.
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); ///< �g�嗦.
	glm::vec3 rotation = glm::vec3(0, 0, 0); ///< ��].
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); ///< �F.

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); ///< �ړ����x.
	glm::vec3 rotatevelocity = glm::vec3(0.0f, 0.0f, 0.0f); ///< ��]���x.
	glm::vec3 scalevelocity = glm::vec3(0.0f, 0.0f, 0.0f); ///< �g��k�����x.
	glm::vec4 colorvelocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); ///< �F.

	bool visible = true;///visible
	bool combinedvisible = true;///�e�m�[�h�Ƃ̍���visible

	//��]����XYZ�ɐݒ�
	order rotationorder = order::XYZ;
	
	glm::mat4 glposition;
	glm::mat4 glrotate;
	glm::mat4 glscale;
	const glm::mat4 glmatrixdefault = {
		{ 1		,0		,0		,0 },
		{ 0		,1		,0		,0 },
		{ 0		,0		,1		,0 },
		{ 0		,0		,0		,1 }
	};
};

