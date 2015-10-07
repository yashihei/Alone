#include "Game.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

const Size Game::stageSize = Size(640, 480);

Game::Game() {
	player = std::make_shared<Player>();
	bulletManager = std::make_shared<BulletManager>();
	enemyManager = std::make_shared<EnemyManager>();

	player->start();
	Graphics2D::SetBlendState(BlendState::Additive);
}

void Game::update() {
	if (Input::KeySpace.pressed) return;
	ClearPrint();
	Println(L"FPS:", Profiler::FPS());

	player->update(this);
	enemyManager->update(this);
	bulletManager->update(this);

	createActors();
}

void Game::createActors() {
	if (Input::Key1.clicked) {
		auto enemy = std::make_shared<TestEnemy>();
		enemy->set(Mouse::Pos());
		enemyManager->add(enemy);
	}
}

void Game::draw() {
	drawBackground();
	player->draw(this);
	enemyManager->draw(this);
	bulletManager->draw(this);
	drawHUD();
}

void Game::drawHUD() {
}

void Game::drawBackground() {
	for (auto i : step(100)) {
		RectF(RandomVec2(640, 480).asPoint(), 40, 40).drawFrame(2.0, 0.0, Color(255, 255, 255, 127));
	}
}
