#include "Player.h"

#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"

Shot::Shot(Vec2 pos, Vec2 vec, double rad) :
pos(pos), vec(vec),
rad(rad), size(5.0)
{}

void Shot::update(Game* game) {
	pos += vec;
	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}
}

void Shot::draw(Game* game) {
	for (int i : step(10)) {
		Triangle({ 0, -15 }, { 5, 5 }, { -5, 5 }).setCentroid(pos + (vec * i) / 10).rotated(rad + Radians(90)).draw(Color(255, 165, 30, 10 * i));
	}
}

Player::Player() :
state(State::NORMAL),
pos(0.0, 0.0),
rad(0.0),
stateCount(0), fireCount(0),
hp(0)
{
	shotManager = std::make_shared<ShotManager>();
}

void Player::start() {
	pos = Vec2(500, 500);
	state = State::NORMAL;
	rad = 0.0;
	stateCount = fireCount = 0;
	hp = 100;
	shotManager->clear();
}

void Player::update(Game* game) {
	stateCount++;

	auto pad = XInput(0);
	pad.setLeftThumbDeadZone();
	pad.setRightThumbDeadZone();

	//move
	if (!Vec2(pad.leftThumbX, -pad.leftThumbY).isZero) {
		rad = Atan2(-pad.leftThumbY, pad.leftThumbX);
		pos += Vec2(Cos(rad), Sin(rad)) * 7.5;
	}
	pos = Vec2(Clamp(pos.x, 0.0, static_cast<double>(Game::stageSize.x)), Clamp(pos.y, 0.0, static_cast<double>(Game::stageSize.y)));

	//bomb
	if (Input::KeyX.clicked) {
		game->getBulletManager()->clear();
		game->getEnemyManager()->clear();
	}

	//fire
	if (!Vec2(pad.rightThumbX, -pad.rightThumbY).isZero) {
		for (int i : {-1, 1, 0}) {
			const double shotRad = Atan2(-pad.rightThumbY, pad.rightThumbX) + Radians(10 * i);
			if (fireCount % 5 == 0) {
				auto shot = std::make_shared<Shot>(pos, Vec2(Cos(shotRad), Sin(shotRad)) * 15.0, shotRad);
				shotManager->add(shot);
			}
		}
	}
	fireCount++;
	shotManager->update(game);

	checkBulletHit(game);
}

void Player::checkBulletHit(Game* game) {
	auto bulletManager = game->getBulletManager();
	for (auto& bullet : *bulletManager) {
		if (Circle(pos, 1.0).intersects(Circle(bullet->getPos(), bullet->getSize()))) {
			hp -= 5;
			bullet->kill();
		}
	}
}

void Player::draw(Game* game) {
	Triangle(pos, 30.0).rotated(rad + HalfPi).draw(Color(150, 150, 255, 122)).drawFrame();
	Line(pos, pos + Vec2(Cos(rad), Sin(rad)) * 1000.0).draw();
	shotManager->draw(game);
}
