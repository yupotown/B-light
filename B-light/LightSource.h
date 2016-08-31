#pragma once

#include <Siv3D.hpp>
#include "PColor.h"

struct LightSource {

	LightSource(PColor color, Point pos, float angle, float angleRange, float stlength, float attenuation, int life);

	/// <summary>
	/// •`‰æ ON / OFF
	/// </summary>
	bool on;

	/// <summary>
	/// F
	/// </summary>
	PColor col;

	/// <summary>
	/// ˆÊ’u
	/// </summary>
	Point pos;

	/// <summary>
	/// Œü‚«(’†S)
	/// </summary>
	float angle;

	/// <summary>
	/// Œü‚«‚Ì”ÍˆÍ
	/// </summary>
	float range;

	/// <summary>
	/// ‹­‚³
	/// </summary>
	float st;

	/// <summary>
	/// Œ¸ŠŒW”
	/// </summary>
	float att;

	/// <summary>
	/// õ–½
	/// </summary>
	int life;
};
