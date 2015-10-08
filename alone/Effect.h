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

class Particle : public IEffect
{
private:
public:
	bool update(double sec);
};
