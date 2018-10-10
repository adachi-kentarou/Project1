#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

/*
ノードクラス
*/
class Node {
public:
	//回転順番
	enum order {
		XYZ = 0,
		XZY = 1,
		YXZ = 2,
		YZX = 3,
		ZXY = 4,
		ZYX = 5
	};

	//移動
	void Position(const glm::vec3& v) { position = v; }
	const glm::vec3& Position() const { return position; }
	//回転
	void Rotation(const glm::vec3& q) { rotation = q; }
	const glm::vec3& Rotation() const { return rotation; }
	//拡大縮小
	void Scale(const glm::vec3& v) { scale = v; }
	const glm::vec3& Scale() const { return scale; }

	void Color(const glm::vec4& v) { color = v; }
	const glm::vec4& Color() const { return color; }

	//移動
	void Velocity(const glm::vec3& v) { velocity = v; }
	const glm::vec3& Velocity() const { return velocity; }
	void Node::SetPosition(const glm::vec3& p);
	glm::mat4 Node::GetPosition() { return glposition; };

	//回転
	void RotateVelocity(const glm::vec3& v) { rotatevelocity = v; }
	const glm::vec3& RotateVelocity() const { return rotatevelocity; }
	void Node::SetRotation(const glm::vec3& r);
	glm::mat4 Node::GetRotation() { return glrotate; };

	//拡大縮小
	void ScaleVelocity(const glm::vec3& v) { scalevelocity = v; }
	const glm::vec3& ScaleVelocity() const { return scalevelocity; }
	void Node::SetScale(const glm::vec3& s);
	glm::mat4 Node::GetScale() { return glscale; };

	//色
	void ColorVelocity(const glm::vec4& c) { colorvelocity = c; }
	const glm::vec4& ColorVelocity() const { return colorvelocity; }
	void Node::SetColor(const glm::vec4& c);
	glm::vec4& Node::GetColor() { return mycolor; };

	//回転順番
	void SetRotationOrder(order o) { rotationorder = o; };

	//マトリクス計算
	glm::mat4 CalcModelMatrix() const;

	//マトリクス取得
	glm::mat4 GetMatrix() { return mymatrix; };


	//visible取得
	bool GetVisible() { return visible; };
	//visible設定
	void SetVisible(bool b) { visible = b; };
	//combinedvisible取得
	bool GetCombinedVisible() { return combinedvisible; };

	Node* GetNode() { return this; };

	//コンストラクタ
	Node();
	Node(glm::mat4* m);
	//デストラクタ
	virtual ~Node();

	//ノード割り当て
	void addNode(Node* n);
	//ノード取り外し
	void removeNode();

	//ノード更新
	void Update();

	//ノードリスト
	std::vector<Node*> nodeList;
	//自信のマトリクス
	glm::mat4* MyMatrix;
	
	

protected:
	//親ノード
	Node * parentnode = nullptr;

	glm::mat4 mymatrix;///マトリクス

	glm::vec4 mycolor = glm::vec4({ 1.0f,1.0f, 1.0f, 1.0f });///色

	glm::vec3 position = glm::vec3(0, 0, 0); ///< 座標.
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f); ///< 拡大率.
	glm::vec3 rotation = glm::vec3(0, 0, 0); ///< 回転.
	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); ///< 色.

	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f); ///< 移動速度.
	glm::vec3 rotatevelocity = glm::vec3(0.0f, 0.0f, 0.0f); ///< 回転速度.
	glm::vec3 scalevelocity = glm::vec3(0.0f, 0.0f, 0.0f); ///< 拡大縮小速度.
	glm::vec4 colorvelocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f); ///< 色.

	bool visible = true;///visible
	bool combinedvisible = true;///親ノードとの合成visible

	//回転順番XYZに設定
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

