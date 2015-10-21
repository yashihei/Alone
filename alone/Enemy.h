#pragma once
#include <Siv3D.hpp>
#include "Actor.h"

class Game;

class Enemy : public Actor {
public:
	Enemy(Vec2 pos);
	void update(Game* game) override;

	Vec2 getPos() const { return pos; }
protected:
	Vec2 pos;
	double rad, size;
	int hp;
	int frameCount, fireCount, damageCount;
};

class TestEnemy : public Enemy {
public:
	using Super = Enemy;
	TestEnemy(Vec2 pos);
	void update(Game* game) override;
	void draw(Game* game) override;
};

class MiddleEnemy : public Enemy {
public:
	using Super = Enemy;
	MiddleEnemy(Vec2 pos);
	void update(Game* game) override;
	void draw(Game* game) override;
};
