/**
* @file InterfaceBlock.h
*/
#ifndef INTERFACEBLOCK_H_INCLUDED
#define INTERFACEBLOCK_H_INCLUDED
#include <glm/glm.hpp>

namespace InterfaceBlock {


	//<--- ここにVertexData, LightData, PostEffectData構造体, BindingPoint列挙型を
	//     カット&ペーストしてください --->
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

	/**
	* ライトデータ(点光源).
	*/
	struct PointLight
	{
		glm::vec4 position; ///< 座標(ワールド座標系).
		glm::vec4 color; ///< 明るさ.
	};

	const int maxLightCount = 4; ///< ライトの数.

	///ライティングパラメータをシェーダに転送するための構造体.
	struct LightData
	{
		glm::vec4 ambientColor; ///< 環境光.
		PointLight light[maxLightCount]; ///< ライトのリスト.
	};

	///ポストエフェクトデータをシェーダに転送するための構造体.
		struct PostEffectData
	{
		glm::mat4x4 matColor; ///< 色変換行列.
	};

	/// バインディングポイント.
	enum BindingPoint
	{
		BINDINGPOINT_VERTEXDATA, ///< 頂点シェーダ用パラメータのバインディングポイント.
		BINDINGPOINT_LIGHTDATA, ///< ライティングパラメータ用のバインディングポイント.
		BINDINGPOINT_POSTEFFECTDATA, ///< ポストエフェクトパラメータ用のバインディングポイント.
	};

}

#endif