#pragma once
#pragma warning(disable: 4100)
#include <memory>
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

	static const Size stageSize;
private:
	void createActors();
	void drawHUD();
	void drawBackground();

	std::shared_ptr<Player> player;
	std::shared_ptr<BulletManager> bulletManager;
	std::shared_ptr<EnemyManager> enemyManager;
};
