#include"Node.h"

/*
�R���X�g���N�^
*/
Node::Node() {
	glm::mat4 glmatrixdefault = {
		{ 1		,0		,0		,0 },
		{ 0		,1		,0		,0 },
		{ 0		,0		,1		,0 },
		{ 0		,0		,0		,1 }
	};
	MyMatrix = &glmatrixdefault;
}
Node::Node(glm::mat4* m) {
	MyMatrix = m;
}

/*
�f�X�g���N�^
*/
Node::~Node() {
	for (auto itr = nodeList.begin(); itr < nodeList.end(); itr++) {
		//�m�[�h���������Ƃ��͎q�̐e�������Z�b�g����
		Node* n = static_cast<Node*>(*itr);
		n->parentnode = nullptr;
		//�}�g���N�X�ݒ�
		n->mymatrix = glmatrixdefault * n->CalcModelMatrix();
		//�F�ݒ�
		n->mycolor = glm::vec4({1.0f,1.0f, 1.0f, 1.0f});
		//�\���ݒ�
		n->combinedvisible = n->visible;
	}
	//���X�g�폜
	nodeList.clear();
	removeNode();
}

/*
�m�[�h���蓖��
*/
void Node::addNode(Node* n) {
	//�e�m�[�h������ꍇ�͎��O��
	if (n->parentnode != nullptr) {
		n->removeNode();
	}
	//���X�g�Ɏw��m�[�h�̃|�C���^��ǉ�����
	nodeList.push_back(n);
	//�e�m�[�h�̐ݒ�
	n->parentnode = this;
	//�}�g���N�X�̐ݒ�
	n->mymatrix = mymatrix * n->CalcModelMatrix();
	//�\���t���O��ݒ肵�Ȃ���
	n->combinedvisible = combinedvisible && n->visible;
}

/*
�m�[�h���O��
*/
void Node::removeNode() {
	if (parentnode == nullptr) {
		return;
	}
	int size = parentnode->nodeList.size();
	for (int i = 0; i < size; i++) {
		//�e�̃m�[�h���X�g�̎����̃|�C���^���폜����
		if (parentnode->nodeList[i] == this) {
			parentnode->nodeList.erase(parentnode->nodeList.begin() + i);
			//�e�m�[�h����ɂ���
			parentnode = nullptr;
			//�\���t���O��ݒ肵�Ȃ���
			combinedvisible = visible;
			//�}�g���N�X��������
			mymatrix = glmatrixdefault * CalcModelMatrix();
			//�F��������
			mycolor = color;
			break;
		}
	}
}

/*
�m�[�h�X�V
*/
void Node::Update(){
	//��]
	SetRotation(RotateVelocity());
	//�ړ�
	SetPosition(Velocity());
	//�g��k��
	SetScale(ScaleVelocity());
	//�F
	SetColor(ColorVelocity());

	const glm::mat4 t = glm::translate(glm::mat4(), position);
	const glm::mat4 r = glrotate;
	const glm::mat4 s = glm::scale(glm::mat4(), scale);
	if (parentnode == nullptr) {
		combinedvisible = visible;
		mymatrix = glmatrixdefault * CalcModelMatrix();
		mycolor = glm::vec4({1.0f,1.0f, 1.0f, 1.0f});
	}
	else {
		combinedvisible = parentnode->combinedvisible && visible;
		mymatrix = parentnode->mymatrix * CalcModelMatrix();
		mycolor = parentnode->mycolor * color;
	}

	for (auto itr = nodeList.begin(); itr < nodeList.end();itr++) {
		//�ċA�����Ŏq�̃m�[�h�����ׂčX�V
		static_cast<Node*>(*itr)->Update();
	}
}

/**
*�}�g���b�N�X�ړ��v�Z
*@param x x�ړ�������
*@param y y�ړ�������
*@param z z�ړ�������
@return �ړ��v�Z���ʃ}�g���N�X
*/
void Node::SetPosition(const glm::vec3& p) {
	position += p;
	glposition = {
		{ 1		,0		,0		,p.x },
	{ 0		,1		,0		,p.y },
	{ 0		,0		,1		,p.z },
	{ 0		,0		,0		,1 }
	};
}

/**
*�}�g���b�N�X��]�v�Z
*@param x x����]�p�x������
*@param y y����]�p�x������
*@param z z����]�p�x������
@return ��]�v�Z���ʃ}�g���N�X
*/
void Node::SetRotation(const glm::vec3& r) {
	rotation += r;

	//x��
	if (rotation.x >= 360) {
		rotation.x -= 360;
	}
	else if (rotation.x < 0) {
		rotation.x += 360;
	}
	float radian = glm::radians(rotation.x);
	float sin = std::sin(radian);
	float cos = std::cos(radian);

	glm::mat4 rotatex = {
		{ 1		,0		,0		,0 },
		{ 0		,cos	,sin	,0 },
		{ 0		,-sin	,cos	,0 },
		{ 0		,0		,0		,1 }
	};

	//����
	if (rotation.y >= 360) {
		rotation.y -= 360;
	}
	else if (rotation.y < 0) {
		rotation.y += 360;
	}
	radian = glm::radians(rotation.y);
	sin = std::sin(radian);
	cos = std::cos(radian);

	glm::mat4 rotatey = {
		{ cos	,0		,-sin	,0 },
		{ 0		,1		,0		,0 },
		{ sin	,0		,cos	,0 },
		{ 0		,0		,0		,1 }
	};

	//z��
	if (rotation.z >= 360) {
		rotation.z -= 360;
	}
	else if (rotation.z < 0) {
		rotation.z += 360;
	}
	radian = glm::radians(rotation.z);
	sin = std::sin(radian);
	cos = std::cos(radian);

	glm::mat4 rotatez = {
		{ cos	,sin	,0		,0 },
		{ -sin	,cos	,0		,0 },
		{ 0		,0		,1		,0 },
		{ 0		,0		,0		,1 }
	};

	//��]����
	switch (rotationorder) {
	case XYZ:
		glrotate = rotatex * rotatey *rotatez;
		break;
	case XZY:
		glrotate = rotatex * rotatez *rotatey;
		break;
	case YXZ:
		glrotate = rotatey * rotatex *rotatez;
		break;
	case YZX:
		glrotate = rotatey * rotatez *rotatex;
		break;
	case ZXY:
		glrotate = rotatez * rotatex *rotatey;
		break;
	case ZYX:
		glrotate = rotatez * rotatey *rotatex;
		break;
	default:
		glrotate = rotatex * rotatey *rotatez;
		break;
	}
}

/**
*�}�g���b�N�X�g��k���v�Z
*@param x x�g��k��������
*@param y y�g��k��������
*@param z z�g��k��������
@return �g��k���v�Z���ʃ}�g���N�X
*/
void Node::SetScale(const glm::vec3& s) {
	scale += s;
	glscale = {
		{ s.x	,0		,0		,0 },
	{ 0		,s.y	,0		,0 },
	{ 0		,0		,s.z	,0 },
	{ 0		,0		,0		,1 }
	};
}

/**
*�J���[�v�Z
*@param x x�g��k��������
*@param y y�g��k��������
*@param z z�g��k��������
@return �g��k���v�Z���ʃ}�g���N�X
*/
void Node::SetColor(const glm::vec4& c) {
	color += c;
}

/**
* �ړ��E��]�E�g�k�s����擾����.
*
* @return TRS�s��.
*/
glm::mat4 Node::CalcModelMatrix() const
{
	const glm::mat4 t = glm::translate(glm::mat4(), position);
	const glm::mat4 r = glrotate;
	const glm::mat4 s = glm::scale(glm::mat4(), scale);

	return t * r * s;
}