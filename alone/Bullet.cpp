#include "Bullet.h"

#include "Game.h"
#include "Player.h"

Bullet::Bullet(Vec2 pos, Color color, double rad, double speed, double accel) :
pos(pos),
color(color),
size(5.0), rad(rad), speed(speed), accel(accel),
frameCount(0)
{}

void Bullet::update(Game* game) {
	frameCount++;
	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}

	speed += accel;
	speed = Clamp(speed, 2.0, 15.0);
	Vec2 vec = Vec2(Cos(rad), Sin(rad)) * speed;
	pos += vec;
}

void Bullet::draw(Game* game) {
	Circle(pos, size).draw(color.setAlpha(200));
}
