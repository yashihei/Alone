#pragma once
#include <Siv3d.hpp>
#include "Actor.h"

class Game;

class Shot : public Actor {
public:
	Shot();
	void set(Vec2 pos, Vec2 vec, double rad);
	void update(Game* game) override;
	void draw(Game* game) override;

	Vec2 getPos() const { return pos; }
	double getSize() const { return size; }
private:
	Vec2 pos, vec;
	double rad, size;
};
using ShotManager = ActorManager<Shot>;

class Player {
public:
	enum class State {
		REBORN,
		NORMAL,
	};

	Player();
	void start();
	void update(Game* game);
	void draw(Game* game);

	Vec2 getPos() const { return pos; }
	std::shared_ptr<ShotManager> getShotManager() const { return shotManager; }
private:
	void checkBulletHit(Game* game);

	State state;
	Vec2 pos;
	double rad;
	int stateCount, fireCount;
	std::shared_ptr<ShotManager> shotManager;
};
