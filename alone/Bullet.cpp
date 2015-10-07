#include "Bullet.h"

#include "Game.h"
#include "Player.h"

Bullet::Bullet() :
pos(0.0, 0.0),
color(Palette::White),
size(5.0), rad(0.0), speed(0.0), accel(0.0),
frameCount(0)
{}

void Bullet::set(Vec2 pos, Color color, double rad, double speed, double accel) {
	this->pos = pos;
	this->color = color;
	this->rad = rad;
	this->speed = speed;
	this->accel = accel;
}

void Bullet::update(Game* game) {
	frameCount++;
	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}

	speed += accel;
	speed = Clamp(speed, 2.0, 15.0);
	Vec2 vec = Vec2(Cos(rad) * speed, Sin(rad) * speed);
	pos += vec;
}

void Bullet::draw(Game* game) {
	Circle(pos, size).draw(color.setAlpha(200)).drawFrame(0.0, 2.0);
}
