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
	Circle(pos, size).draw(Palette::Orange).drawFrame(0.0, 2.0);
}

Player::Player() :
state(State::NORMAL),
pos(0.0, 0.0),
rad(0.0),
stateCount(0), fireCount(0)
{
	shotManager = std::make_shared<ShotManager>();
}

void Player::start() {
	pos = Vec2(Game::stageSize.x / 2, 420);
	state = State::NORMAL;
	stateCount = fireCount = 0;
	shotManager->clear();
}

void Player::update(Game* game) {
	stateCount++;

	auto pad = XInput(0);

	pad.setLeftThumbDeadZone();
	pad.setRightThumbDeadZone();

	Println(pad.rightThumbX, L",", pad.rightThumbY);
	Println(pad.leftThumbX, L",", pad.leftThumbY);

	//move
	const Vec2 dir(Input::KeyRight.pressed - Input::KeyLeft.pressed, Input::KeyDown.pressed - Input::KeyUp.pressed);
	if (!dir.isZero) {
		pos += Normalize(dir) * (Input::KeyShift.pressed ? 10.0 / 2 : 10.0);
	}
	pos = Vec2(Clamp(pos.x, 0.0, static_cast<double>(Game::stageSize.x)), Clamp(pos.y, 0.0, static_cast<double>(Game::stageSize.y)));

	//bomb
	if (Input::KeyX.clicked) {
		game->getBulletManager()->clear();
		game->getEnemyManager()->clear();
	}

	//fire
	fireCount++;
	if (Input::KeyZ.pressed && fireCount % 3 == 0) {
		for (auto i : { -1, 1 }) {
			auto shot = std::make_shared<Shot>();
			shot->set(pos + Vec2(10.0 * i, 0.0), Vec2(0.0, -15.0), 0.0);
			shotManager->add(shot);
		}
	}
	shotManager->update(game);
}

void Player::checkBulletHit(Game* game) {
}

void Player::draw(Game* game) {
	Triangle(pos, 30.0).rotated(rad).draw(Color(Palette::Yellow).setAlpha(123)).drawFrame(2.0);
	shotManager->draw(game);
}
