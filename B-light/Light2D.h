#pragma once

#include <opencv2/opencv.hpp>
#include <Siv3D.hpp>
#include <vector>
#include "LightSource.h"

struct Light2D {

	Light2D(int screenW, int screenH, int imgRate);

	/// <summary>
	/// 1�F�̃e�N�X�`�����X�V����B
	/// </summary>
	void update(PColor col);

	/// <summary>
	/// ��ʂ̍��W���v�Z�p��Ԃ̍��W�ɕϊ�����B
	/// </summary>
	int toImgX(int x) const;

	/// <summary>
	/// ��ʂ̍��W���v�Z�p��Ԃ̍��W�ɕϊ�����B
	/// </summary>
	int toImgY(int y) const;

	/// <summary>
	/// ��ʂ̍��W���v�Z�p��Ԃ̍��W�ɕϊ�����B
	/// </summary>
	void toImgPos(int x, int y, int &outX, int &outY) const;

	/// <summary>
	/// �v�Z�p��Ԃ̍��W����Ή������ʂ͈̔͂̍���̍��W���v�Z����B
	/// </summary>
	int toScreenX(int x) const;

	/// <summary>
	/// �v�Z�p��Ԃ̍��W����Ή������ʂ͈̔͂̍���̍��W���v�Z����B
	/// </summary>
	int toScreenY(int y) const;

	/// <summary>
	/// �v�Z�p��Ԃ̍��W����Ή������ʂ͈̔͂̍���̍��W���v�Z����B
	/// </summary>
	void toScreenPos(int x, int y, int &outX, int &outY) const;

	/// <summary>
	/// ��ʂ̕�
	/// </summary>
	int scw;

	/// <summary>
	/// ��ʂ̍���
	/// </summary>
	int sch;

	/// <summary>
	/// �v�Z�p�摜�̕�
	/// </summary>
	int imgw;

	/// <summary>
	/// �v�Z�p�摜�̍���
	/// </summary>
	int imgh;

	/// <summary>
	/// ��ʂ̉𑜓x�ɑ΂�����̌v�Z�p�摜�̉𑜓x�̔䗦�̋t��
	/// </summary>
	int rate;

	/// <summary>
	/// �ŏI�I�ȕ`��p�e�N�X�`��
	/// </summary>
	DynamicTexture tex[3];

	/// <summary>
	/// �v�Z�p�摜
	/// </summary>
	cv::Mat matLt[3];

	/// <summary>
	/// �v�Z�p�摜����e�N�X�`���ɕϊ����邽�߂� Siv3D �摜
	/// </summary>
	Image img[3];

	/// <summary>
	/// ��Ԃ̌����ߗ�
	/// </summary>
	cv::Mat air;

	/// <summary>
	/// ��
	/// </summary>
	cv::Mat maskMir;

	/// <summary>
	/// ���̖@���x�N�g��
	/// </summary>
	cv::Mat vecMir;

	/// <summary>
	/// ����
	/// </summary>
	std::vector<LightSource> srcs;

};
