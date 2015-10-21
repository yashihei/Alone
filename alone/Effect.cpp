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

CircleEffect::CircleEffect(Vec2 pos, double size) :
pos(pos),
size(size)
{}

bool CircleEffect::update(double sec) {
	for (int i : step(10)) {
		Circle(pos, size *  sec + i).drawFrame(1, 0.0, Color(255, 0, 0, 20 * i));
	}
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
		//‚±‚±‚ç‚ª•¨‘«‚è‚È‚¢
		particle.pos += particle.vec * sec;
		RectF(30.0 * (1.0 - sec)).setCenter(particle.pos.asPoint()).rotated(particle.rad).draw(HSV(Palette::White).toColorF(1.0 - sec));
	}
	return sec < 1.0;
}

ScoreEffect::ScoreEffect(Vec2 pos, int score) :
pos(pos), score(score)
{}

bool ScoreEffect::update(double sec) {
	FontAsset(L"middleLog").drawCenter(Format(score), pos, HSV(Palette::Lightgreen).toColorF(1.0 - sec));

	return sec < 1.0;
}