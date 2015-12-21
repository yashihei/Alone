#include "Interface.h"

Enemy::Enemy(Vec2 pos) :
state(State::APPEAR),
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
		game->addLog(L"ENEMY DESTROY");
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
	if (fireCount % 2 == 0 && frameCount > 60) {
		for (auto i : step(5)) {
			double fireRad = Radians(frameCount * 2) + TwoPi / 5 * i;
			auto bullet = std::make_shared<Bullet>(pos, Color(255, 100, 100), fireRad, 5.0, 0.0);
			bulletManager->add(bullet);
		}
	}
}

void TestEnemy::draw(Game* game) {
	Color c = damageCount < 10 ? Color(255, 200) : Color(Palette::Yellow).setAlpha(122);
	RectF(size * 2).setCenter(pos).rotated(rad).draw(c).drawFrame();
}

MiddleEnemy::MiddleEnemy(Vec2 pos) : Super(pos) {
	hp = 15;
	size = 10.0;
}

void MiddleEnemy::update(Game* game) {
	Super::update(game);
	rad += Radians(10.0);

	const Vec2 playerPos = game->getPlayer()->getPos();
	const double rad2 = Atan2(playerPos.y - pos.y, playerPos.x - pos.x);

	if (frameCount % 100 < 50) {
		pos += Vec2(Cos(rad2), Sin(rad2)) * 3.0;
	} else if (frameCount % 3 == 0) {
		const double fireRad = rad2 + Radians(Random(-15.0, 15.0));
		auto bullet = std::make_shared<Bullet>(pos, Color(255, 100, 100), fireRad, Random(2.0, 7.0), 0.0);
		game->getBulletManager()->add(bullet);
	}
}

void MiddleEnemy::draw(Game* game) {
	const Polygon polygon = Geometry2D::CreateNgon(6, size * 2);
	Color c = damageCount < 10 ? Color(255, 200) : Color(Palette::Yellow).setAlpha(122);
	polygon.rotated(rad).draw(pos, c);
	polygon.rotated(rad).drawFrame(pos);
}
