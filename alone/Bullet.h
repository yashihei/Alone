#pragma once
#include <Siv3D.hpp>
#include "Actor.h"

class Game;

class Bullet : public Actor {
public:
	Bullet();
	void set(Vec2 pos, Color color, double rad, double speed, double accel);
	void update(Game* game) override;
	void draw(Game* game) override;

	Vec2 getPos() const { return pos; }
	double getSize() const { return size; }
private:
	Vec2 pos;
	Color color;
	double size, rad, speed, accel;
	int frameCount;
};
