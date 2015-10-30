#pragma once
#include <Siv3d.hpp>
#include <deque>
#include "Actor.h"

class Game;

class Shot : public Actor {
public:
	Shot(Vec2 pos, double rad);
	void update(Game* game) override;

	Vec2 getPos() const { return pos; }
	double getSize() const { return size; }
protected:
	Vec2 pos;
	double rad, size;
};
using ShotManager = ActorManager<Shot>;

class NormalShot : public Shot {
public:
	NormalShot(Vec2 pos, double rad);
	void update(Game* game) override;
	void draw(Game* game) override;
private:
};

//ŽÀ—p’iŠK‚É–³‚¢‚Ì‚Å••ˆó
class HormingShot : public Shot {
public:
	HormingShot(Vec2 pos, double rad);
	void update(Game* game) override;
	void draw(Game* game) override;
private:
	std::deque<Vec2> tracks;
	double accel, count;
};

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
	int getHp() const { return hp; }
	std::shared_ptr<ShotManager> getShotManager() const { return shotManager; }
private:
	void checkBulletHit(Game* game);

	State state;
	Vec2 pos;
	double rad;
	int frameCount, fireCount, damageCount;
	int hp, shield;
	std::shared_ptr<ShotManager> shotManager;
	std::deque<Vec2> tracks;
};
