#include "Player.h"

#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"

Shot::Shot(Vec2 pos, double rad) : pos(pos), rad(rad), size(5.0) {}

void Shot::update(Game* game) {
	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}
}

NormalShot::NormalShot(Vec2 pos, double rad) :
Shot(pos, rad)
{}

void NormalShot::update(Game* game) {
	Shot::update(game);

	const Vec2 vec = Vec2(Cos(rad), Sin(rad)) * 15.0;
	pos += vec;
}

void NormalShot::draw(Game* game) {
	const Vec2 vec = Vec2(Cos(rad), Sin(rad)) * 15.0;
	for (int i : step(10)) {
		Triangle({ -5, 5 }, { 0, -15 }, { 5, 5 }).setCentroid(pos + (vec * i) / 10).rotated(rad + HalfPi).draw(Color(255, 165, 30, 10 * i));
	}
}

HormingShot::HormingShot(Vec2 pos, double rad) :
Shot(pos, rad),
accel(0.0)
{}

void HormingShot::update(Game* game) {
	Shot::update(game);
	accel += 0.2;

	const Vec2 targetPos = game->getNearEnemyPos();
	const double rad2 = Atan2(targetPos.y - pos.y, targetPos.x - pos.x);
	const double radLimit = Radians(10);
	if (Abs(rad2 - rad) < radLimit) {
		rad = rad2;
	} else {
		if (rad2 < rad - Pi) {
			rad -= TwoPi;
		} else if (rad2 > rad + Pi) {
			rad += TwoPi;
		}
		if (rad2 < rad) {
			rad -= radLimit;
		} else {
			rad += radLimit;
		}
	}
	pos += Vec2(Cos(rad), Sin(rad)) * (10.0 + accel);

	tracks.push_front(pos);
	if (tracks.size() > 15) tracks.pop_back();
}

void HormingShot::draw(Game* game) {
	int i = 0;
	Vec2 beforePos = pos;
	for (auto trackPos : tracks) {
		Line(trackPos, beforePos).draw(3.0, HSV(Color(100, 255, 100)).toColorF(1.0 - (1.0 / tracks.size()) * i));
		i++;
		beforePos = trackPos;
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
				auto shot = std::make_shared<NormalShot>(pos, shotRad);
				shotManager->add(shot);
			}
		}
	}
	if (pad.buttonA.clicked || Input::KeyZ.clicked) {
		for (int i : step(10)) {
			const double shotRad = TwoPi / 10 * i;
			auto shot = std::make_shared<HormingShot>(pos, shotRad);
			shotManager->add(shot);
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
			game->addLog(L"DAMAGE!!");
		}
	}
}

void Player::draw(Game* game) {
	Triangle(pos, 30.0).rotated(rad + HalfPi).draw(Color(150, 150, 255, 122)).drawFrame();
	Line(pos, pos + Vec2(Cos(rad), Sin(rad)) * 1000.0).draw();
	const double stickRad = Atan2(-XInput(0).rightThumbY, XInput(0).rightThumbX);
	Line(pos, pos + Vec2(Cos(stickRad), Sin(stickRad)) * 1000.0).draw(Palette::Lightblue );
	shotManager->draw(game);
}
