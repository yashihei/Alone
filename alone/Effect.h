#pragma once
#include <Siv3D.hpp>

//FIXME:offset‚Ì‹¤—L
class ScreenShake : public IEffect
{
private:
	double maxOffset, lengthMillSec;
public:
	ScreenShake(double maxOffset, double lengthMillSec);
	bool update(double sec);
};

class CircleEffect : public IEffect
{
private:
	Vec2 pos;
	double size;
public:
	CircleEffect(Vec2 pos, double size);
	bool update(double sec);
};
