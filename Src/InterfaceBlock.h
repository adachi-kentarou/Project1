/**
* @file InterfaceBlock.h
*/
#ifndef INTERFACEBLOCK_H_INCLUDED
#define INTERFACEBLOCK_H_INCLUDED
#include <glm/glm.hpp>

namespace InterfaceBlock {


	//<--- ������VertexData, LightData, PostEffectData�\����, BindingPoint�񋓌^��
	//     �J�b�g&�y�[�X�g���Ă������� --->
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

	/**
	* ���C�g�f�[�^(�_����).
	*/
	struct PointLight
	{
		glm::vec4 position; ///< ���W(���[���h���W�n).
		glm::vec4 color; ///< ���邳.
	};

	const int maxLightCount = 4; ///< ���C�g�̐�.

	///���C�e�B���O�p�����[�^���V�F�[�_�ɓ]�����邽�߂̍\����.
	struct LightData
	{
		glm::vec4 ambientColor; ///< ����.
		PointLight light[maxLightCount]; ///< ���C�g�̃��X�g.
	};

	///�|�X�g�G�t�F�N�g�f�[�^���V�F�[�_�ɓ]�����邽�߂̍\����.
		struct PostEffectData
	{
		glm::mat4x4 matColor; ///< �F�ϊ��s��.
	};

	/// �o�C���f�B���O�|�C���g.
	enum BindingPoint
	{
		BINDINGPOINT_VERTEXDATA, ///< ���_�V�F�[�_�p�p�����[�^�̃o�C���f�B���O�|�C���g.
		BINDINGPOINT_LIGHTDATA, ///< ���C�e�B���O�p�����[�^�p�̃o�C���f�B���O�|�C���g.
		BINDINGPOINT_POSTEFFECTDATA, ///< �|�X�g�G�t�F�N�g�p�����[�^�p�̃o�C���f�B���O�|�C���g.
	};

}

#endif