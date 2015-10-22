#pragma once
#pragma warning(disable: 4100)
#include <memory>
#include <deque>
#include "Actor.h"

class Player;
class Bullet;
class Enemy;

using BulletManager = ActorManager<Bullet>;
using EnemyManager = ActorManager<Enemy>;

class Game {
public:
	Game();
	void update();
	void draw();

	std::shared_ptr<Player> getPlayer() const { return player; }
	std::shared_ptr<BulletManager> getBulletManager() const { return bulletManager; }
	std::shared_ptr<EnemyManager> getEnemyManager() const { return enemyManager; }
	std::shared_ptr<Effect> getEffect() const { return effect; }

	void addScore(int score) { this->score += score; }
	void addLog(String str);
	Vec2 getNearEnemyPos();

	static const Size stageSize;
private:
	void createActors();
	void drawHUD();
	void drawBackground();

	std::shared_ptr<Player> player;
	std::shared_ptr<BulletManager> bulletManager;
	std::shared_ptr<EnemyManager> enemyManager;
	std::shared_ptr<Effect> effect;

	Vec2 offset;
	int score;
	std::deque<String> logStrs;
};
