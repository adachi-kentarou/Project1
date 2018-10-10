#include"Node.h"

/*
コンストラクタ
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
デストラクタ
*/
Node::~Node() {
	for (auto itr = nodeList.begin(); itr < nodeList.end(); itr++) {
		//ノードが消されるときは子の親情報をリセットする
		Node* n = static_cast<Node*>(*itr);
		n->parentnode = nullptr;
		//マトリクス設定
		n->mymatrix = glmatrixdefault * n->CalcModelMatrix();
		//色設定
		n->mycolor = glm::vec4({1.0f,1.0f, 1.0f, 1.0f});
		//表示設定
		n->combinedvisible = n->visible;
	}
	//リスト削除
	nodeList.clear();
	removeNode();
}

/*
ノード割り当て
*/
void Node::addNode(Node* n) {
	//親ノードがある場合は取り外す
	if (n->parentnode != nullptr) {
		n->removeNode();
	}
	//リストに指定ノードのポインタを追加する
	nodeList.push_back(n);
	//親ノードの設定
	n->parentnode = this;
	//マトリクスの設定
	n->mymatrix = mymatrix * n->CalcModelMatrix();
	//表示フラグを設定しなおす
	n->combinedvisible = combinedvisible && n->visible;
}

/*
ノード取り外し
*/
void Node::removeNode() {
	if (parentnode == nullptr) {
		return;
	}
	int size = parentnode->nodeList.size();
	for (int i = 0; i < size; i++) {
		//親のノードリストの自分のポインタを削除する
		if (parentnode->nodeList[i] == this) {
			parentnode->nodeList.erase(parentnode->nodeList.begin() + i);
			//親ノードを空にする
			parentnode = nullptr;
			//表示フラグを設定しなおす
			combinedvisible = visible;
			//マトリクスを初期化
			mymatrix = glmatrixdefault * CalcModelMatrix();
			//色を初期化
			mycolor = color;
			break;
		}
	}
}

/*
ノード更新
*/
void Node::Update(){
	//回転
	SetRotation(RotateVelocity());
	//移動
	SetPosition(Velocity());
	//拡大縮小
	SetScale(ScaleVelocity());
	//色
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
		//再帰処理で子のノードをすべて更新
		static_cast<Node*>(*itr)->Update();
	}
}

/**
*マトリックス移動計算
*@param x x移動増加量
*@param y y移動増加量
*@param z z移動増加量
@return 移動計算結果マトリクス
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
*マトリックス回転計算
*@param x x軸回転角度増加量
*@param y y軸回転角度増加量
*@param z z軸回転角度増加量
@return 回転計算結果マトリクス
*/
void Node::SetRotation(const glm::vec3& r) {
	rotation += r;

	//x軸
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

	//ｙ軸
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

	//z軸
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

	//回転順番
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
*マトリックス拡大縮小計算
*@param x x拡大縮小増加量
*@param y y拡大縮小増加量
*@param z z拡大縮小増加量
@return 拡大縮小計算結果マトリクス
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
*カラー計算
*@param x x拡大縮小増加量
*@param y y拡大縮小増加量
*@param z z拡大縮小増加量
@return 拡大縮小計算結果マトリクス
*/
void Node::SetColor(const glm::vec4& c) {
	color += c;
}

/**
* 移動・回転・拡縮行列を取得する.
*
* @return TRS行列.
*/
glm::mat4 Node::CalcModelMatrix() const
{
	const glm::mat4 t = glm::translate(glm::mat4(), position);
	const glm::mat4 r = glrotate;
	const glm::mat4 s = glm::scale(glm::mat4(), scale);

	return t * r * s;
}