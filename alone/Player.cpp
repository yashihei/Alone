#include "Interface.h"

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
	const int n = 10;
	for (int i : step(n)) {
		Triangle shotPolygon({ -5, 5 }, { 0, -15 }, { 5, 5 });
		shotPolygon.setCentroid(pos + (vec / n) * i).rotated(rad + HalfPi).draw(HSV(Color(255, 165, 30)).toColorF(0.035 * i));
	}
}

#ifdef DEBUG
HormingShot::HormingShot(Vec2 pos, double rad) :
Shot(pos, rad),
accel(0.0), count(0.0)
{}

void HormingShot::update(Game* game) {
	Shot::update(game);
	accel += 0.2;

	const Vec2 targetPos = game->getNearEnemyPos();
	const double rad2 = Atan2(targetPos.y - pos.y, targetPos.x - pos.x);
	const double radLimit = Radians(10);
	if (count < TwoPi) {
		if (Abs(rad2 - rad) < radLimit) {
			rad = rad2;
			count += rad2;
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
			count += radLimit;
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
		Line(beforePos, trackPos).draw(1.5, HSV(Color(100, 255, 100)).toColorF(1.0 - (1.0 / tracks.size()) * i));
		i++;
		beforePos = trackPos;
	}
}
#endif

namespace {
	const int HP_MAX = 100;
	const int SHIELD_MAX = 20;
}

Player::Player() :
state(State::NORMAL),
pos(0.0, 0.0),
rad(0.0),
frameCount(0), fireCount(0), damageCount(0),
hp(0), shield(0)
{
	shotManager = std::make_shared<ShotManager>();
}

void Player::start() {
	pos = Vec2(500, 500);
	state = State::NORMAL;
	rad = 0.0;
	frameCount = fireCount = 0;
	hp = HP_MAX;
	shield = SHIELD_MAX;
	shotManager->clear();
	tracks.clear();
}

void Player::update(Game* game) {
	frameCount++;

	auto pad = XInput(0);
	pad.setLeftThumbDeadZone();
	pad.setRightThumbDeadZone();

	//move
	if (!Vec2(pad.leftThumbX, pad.leftThumbY).isZero) {
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
	if (!Vec2(pad.rightThumbX, pad.rightThumbY).isZero) {
		for (int i : {-1, 1, 0}) {
			const double fireRad = Atan2(-pad.rightThumbY, pad.rightThumbX) + Radians(5 * i);
			if (fireCount % 5 == 0) {
				auto shot = std::make_shared<NormalShot>(pos, fireRad);
				shotManager->add(shot);
			}
		}
	}
	fireCount++;
	shotManager->update(game);

	tracks.push_front(pos);
	if (tracks.size() > 20) tracks.pop_back();

	damageCount++;
	checkBulletHit(game);
	if (frameCount % 10 == 0 && damageCount) shield++;
	shield = Clamp(shield, 0, SHIELD_MAX);
	hp = Clamp(hp, 0, HP_MAX);
}

void Player::checkBulletHit(Game* game) {
	auto bulletManager = game->getBulletManager();
	for (auto& bullet : *bulletManager) {
		if (Circle(pos, 1.0).intersects(Circle(bullet->getPos(), bullet->getSize()))) {
			if (shield == 0) hp -= 5;
			else shield -= 5;
			bullet->kill();
			game->addLog(L"DAMAGE +5");
		}
	}
}

void Player::draw(Game* game) {
	Triangle(pos, 30.0).rotated(rad + HalfPi).draw(Color(150, 150, 255, 122)).drawFrame();
	shotManager->draw(game);

	Circle(pos, 1.0).draw(Color(255, 100, 100, 122));
	//Circle(pos, 30.0).drawArc(0.0, TwoPi * (static_cast<double>(shield) / SHIELD_MAX), 0.0, 2.0, Color(200, 200, 255, 122));
	//Circle(pos, 25.0).drawArc(0.0, TwoPi * (static_cast<double>(hp) / HP_MAX), 0.0, 2.0, Color(255, 150, 150, 122));

	int i = 0;
	Vec2 beforePos = pos;
	for (auto trackPos : tracks) {
		Line(beforePos, trackPos).draw(1.0, HSV(Color(150, 150, 255)).toColorF(1.0 - (1.0 / tracks.size()) * i));
		i++;
		beforePos = trackPos;
	}
}