#include "LightSource.h"

LightSource::LightSource(PColor color, Point pos, float angle, float angleRange, float stlength, float attenuation, int life)
	: on(true),
	col(color),
	pos(pos),
	angle(angle),
	range(angleRange),
	st(stlength),
	att(attenuation),
	life(life)
{
}
