#include "Enemy.h"

#include "Game.h"
#include "Bullet.h"
#include "Player.h"

Enemy::Enemy() :
pos(0.0, 0.0),
rad(0.0), size(0.0),
hp(0),
frameCount(0), fireCount(0)
{}

void Enemy::set(Vec2 pos) {
	this->pos = pos;
}

void Enemy::update(Game* game) {
	frameCount++; fireCount++;

	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}

	auto shotManager = game->getPlayer()->getShotManager();
	for (auto& shot : *shotManager) {
		if (Circle(pos, size).intersects(Circle(shot->getPos(), shot->getSize()))) {
			hp--;
			shot->kill();
		}
	}
	if (hp <= 0) {
		kill();
	}
}

void Enemy::draw(Game* game) {
	RectF(size * 2).setCenter(pos).rotated(rad).draw(Color(Palette::Yellow).setAlpha(123)).drawFrame();
}

TestEnemy::TestEnemy() {
	hp = 10;
	size = 15.0;
}

void TestEnemy::update(Game* game) {
	Super::update(game);
	rad += Radians(3.0);

	auto bulletManager = game->getBulletManager();
	Vec2 playerPos = game->getPlayer()->getPos();
	if (fireCount % 15 == 0) {
		for (auto i : step_to(-2, 2, 1)) {
			double shotRad = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);
			shotRad += Radians(30) * i;
			auto bullet = std::make_shared<Bullet>();
			bullet->set(pos, Color(255, 100, 100), shotRad, 8.0, 0.0);
			bulletManager->add(bullet);
		}
	}
}
