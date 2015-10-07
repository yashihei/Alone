#pragma once
#include <Siv3D.hpp>
#include "Actor.h"

class Game;

class Enemy : public Actor {
public:
	Enemy();
	void update(Game* game) override;
	void draw(Game* game) override;
	void set(Vec2 pos);
protected:
	Vec2 pos;
	double rad, size;
	int hp;
	int frameCount, fireCount, damageCount;
};

class TestEnemy : public Enemy {
public:
	using Super = Enemy;
	TestEnemy();
	void update(Game* game) override;
};
