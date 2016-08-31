#pragma once

#include <opencv2/opencv.hpp>
#include <Siv3D.hpp>
#include <vector>
#include "LightSource.h"

struct Light2D {

	Light2D(int screenW, int screenH, int imgRate);

	void update(PColor col);

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
