#include "Player.h"

#include "Game.h"
#include "Bullet.h"
#include "Enemy.h"

Shot::Shot() :
pos(0.0, 0.0), vec(0.0, 0.0),
rad(0.0), size(5.0)
{}

void Shot::set(Vec2 pos, Vec2 vec, double rad) {
	this->pos = pos;
	this->vec = vec;
	this->rad = rad;
}

void Shot::update(Game* game) {
	pos += vec;
	if (pos.x > Game::stageSize.x || pos.x < 0 || pos.y > Game::stageSize.y || pos.y < 0) {
		kill();
	}
}

void Shot::draw(Game* game) {
	Circle(pos, size).draw(Color(Palette::Orange).setAlpha(200)).drawFrame(0.0, 1.0);
}

Player::Player() :
state(State::NORMAL),
pos(0.0, 0.0),
rad(0.0), shotRad(0.0),
stateCount(0), fireCount(0)
{
	shotManager = std::make_shared<ShotManager>();
}

void Player::start() {
	pos = Vec2(Game::stageSize.x / 2, 420);
	state = State::NORMAL;
	rad = shotRad = 0.0;
	stateCount = fireCount = 0;
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
		shotRad = Atan2(-pad.rightThumbY, pad.rightThumbX);
		if (fireCount % 3 == 0) {
			auto shot = std::make_shared<Shot>();
			shot->set(pos, Vec2(Cos(shotRad), Sin(shotRad)) * 15.0, shotRad);
			shotManager->add(shot);
		}
	}
	fireCount++;
	shotManager->update(game);
}

void Player::checkBulletHit(Game* game) {
}

void Player::draw(Game* game) {
	Triangle(pos, 30.0).rotated(rad + Radians(90)).draw(Color(150, 150, 255, 122)).drawFrame();
	Line(pos, pos + Vec2(Cos(rad), Sin(rad)) * 1000.0).draw();
	Line(pos, pos + Vec2(Cos(shotRad), Sin(shotRad)) * 1000.0).draw(Palette::Lightblue);
	shotManager->draw(game);
}
