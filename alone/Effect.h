#pragma once
#include <Siv3D.hpp>
#include <vector>

//FIXME:offset‚Ì‹¤—L
//from:http://play-siv3d.hateblo.jp/entry/sample/screenshake
class ScreenShake : public IEffect {
private:
	double maxOffset, lengthMillSec;
public:
	ScreenShake(double maxOffset, double lengthMillSec);
	bool update(double sec) override;
};

class CircleEffect : public IEffect {
private:
	Vec2 pos;
	double size;
public:
	CircleEffect(Vec2 pos, double size);
	bool update(double sec) override;
};

class CrashEffect : public IEffect {
private:
	struct Particle {
		Particle(Vec2 pos, Vec2 vec, double rad) : pos(pos), vec(vec), rad(rad) {}
		Vec2 pos, vec;
		double rad;
	};
	std::vector<Particle> particles;
public:
	CrashEffect(Vec2 pos);
	bool update(double sec) override;
};
