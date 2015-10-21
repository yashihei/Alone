#include "Enemy.h"

#include "Game.h"
#include "Bullet.h"
#include "Player.h"
#include "Effect.h"

Enemy::Enemy(Vec2 pos) :
pos(pos),
rad(0.0), size(0.0),
hp(0),
frameCount(0), fireCount(0), damageCount(10)
{}

void Enemy::update(Game* game) {
	frameCount++; fireCount++; damageCount++;

	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}

	auto shotManager = game->getPlayer()->getShotManager();
	for (auto& shot : *shotManager) {
		if (Circle(pos, size).intersects(Circle(shot->getPos(), shot->getSize()))) {
			hp--;
			damageCount = 0;
			shot->kill();
		}
	}
	if (hp <= 0) {
		kill();
		game->getEffect()->add<CrashEffect>(pos);
		game->getEffect()->add<ScoreEffect>(pos, 100);
		game->addScore(100);
	}
}

TestEnemy::TestEnemy(Vec2 pos) : Super(pos) {
	hp = 10;
	size = 15.0;
}

void TestEnemy::update(Game* game) {
	Super::update(game);
	rad += Radians(2.0);

	auto bulletManager = game->getBulletManager();
	const Vec2 playerPos = game->getPlayer()->getPos();
	if (fireCount % 10 == 0) {
		for (auto i : step_to(-2, 2, 1)) {
			double shotRad = rad + TwoPi / 5 * i;
			auto bullet = std::make_shared<Bullet>(pos, Color(255, 100, 100), shotRad, 3.0, 0.0);
			bulletManager->add(bullet);
		}
	}
}

void TestEnemy::draw(Game* game) {
	Color c = damageCount < 10 ? Color(255, 200) : Color(Palette::Yellow).setAlpha(123);
	RectF(size * 2).setCenter(pos).rotated(rad).draw(c).drawFrame();
}

MiddleEnemy::MiddleEnemy(Vec2 pos) : Super(pos) {
	hp = 30;
	size = 20.0;
}

void MiddleEnemy::update(Game* game) {
	Super::update(game);
	rad += Radians(10.0);

	const Vec2 playerPos = game->getPlayer()->getPos();
	const double rad2 = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (frameCount % 100 < 50) {
		pos += Vec2(Cos(rad2), Sin(rad2)) * 3.0;
	} else {
		const double shotRad = rad2 + Radians(Random(-15.0, 15.0));
		auto bullet = std::make_shared<Bullet>(pos, Color(255, 100, 100), shotRad, Random(2.0, 7.0), 0.0);
		game->getBulletManager()->add(bullet);
	}
}

void MiddleEnemy::draw(Game* game) {
	const Polygon polygon = Geometry2D::CreateNgon(6, size * 2);
	Color c = damageCount < 10 ? Color(255, 200) : Color(Palette::Yellow).setAlpha(123);
	polygon.rotated(rad).draw(pos, c);
	polygon.rotated(rad).drawFrame(pos);
}
