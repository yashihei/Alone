#include "Effect.h"

ScreenShake::ScreenShake(double maxOffset, double lengthMillSec) :
maxOffset(maxOffset), lengthMillSec(lengthMillSec) {}

bool ScreenShake::update(double timeSec) {
	const double timeMillisec = timeSec * 1000.0;
	const Vec2 offset = RandomVec2(Circle(offset * Max(1.0 - timeMillisec / lengthMillSec, 0.0)));
	Graphics2D::SetTransform(Mat3x2::Translate(offset));
	return timeMillisec < lengthMillSec;
}
