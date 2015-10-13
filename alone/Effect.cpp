#include "Effect.h"

#include "Game.h"

ScreenShake::ScreenShake(double maxOffset, double lengthMillSec) :
maxOffset(maxOffset), lengthMillSec(lengthMillSec) {}

bool ScreenShake::update(double timeSec) {
	const double timeMillisec = timeSec * 1000.0;
	const Vec2 offset = RandomVec2(Circle(offset * Max(1.0 - timeMillisec / lengthMillSec, 0.0)));
	Graphics2D::SetTransform(Mat3x2::Translate(offset));
	return timeMillisec < lengthMillSec;
}

Snow::Snow(const Point& pos, int count) : particles(count) {
	for (auto& particle : particles) {
		const Vec2 v = Circular(Random(10.0, 80.0), Random(TwoPi));
		particle.pos = pos + v;
		particle.v0 = v * 2.0;
	}
}

bool Snow::update(double sec) {
	for (const auto& particle : particles) {
		const Vec2 pos = particle.pos + particle.v0 * sec + 0.5 * sec*sec * Vec2(0, 320);
		Circle(pos, 1).draw(HSV(pos.y / 4.0, 0.6, 1.0).toColorF(1.0 - sec));
	}

	return sec < 1.0;
}

CircleEffect::CircleEffect(Vec2 pos, double size) :
pos(pos),
size(size)
{}

bool CircleEffect::update(double sec) {
	Circle(pos, size *  sec).drawFrame();
	return sec < 1.0;
}

CrashEffect::CrashEffect(Vec2 pos) {
	for (int i : step(10)) {
		particles.emplace_back(pos, RandomVec2(5.0), 0.0);
	}
}

bool CrashEffect::update(double sec) {
	for (auto& particle : particles) {
		particle.rad += Radians(3.0);
		particle.pos += particle.vec * sec;
		RectF(30.0 * (1.0 - sec)).setCenter(particle.pos.asPoint()).rotated(particle.rad).draw(HSV(Palette::White).toColorF(1.0 - sec));
	}
	return sec < 1.0;
}
