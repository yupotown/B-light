#pragma once

#include <Siv3D.hpp>
#include "PColor.h"

struct LightSource {

	LightSource(PColor color, Point pos, float angle, float angleRange, float stlength, float attenuation, int life);

	/// <summary>
	/// �`�� ON / OFF
	/// </summary>
	bool on;

	/// <summary>
	/// �F
	/// </summary>
	PColor col;

	/// <summary>
	/// �ʒu
	/// </summary>
	Point pos;

	/// <summary>
	/// ����(���S)
	/// </summary>
	float angle;

	/// <summary>
	/// �����͈̔�
	/// </summary>
	float range;

	/// <summary>
	/// ����
	/// </summary>
	float st;

	/// <summary>
	/// �����W��
	/// </summary>
	float att;

	/// <summary>
	/// ����
	/// </summary>
	int life;
};
