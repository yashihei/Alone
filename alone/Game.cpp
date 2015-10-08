#include "Game.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Effect.h"

const Size Game::stageSize = Size(1000, 1000);

Game::Game() {
	player = std::make_shared<Player>();
	bulletManager = std::make_shared<BulletManager>();
	enemyManager = std::make_shared<EnemyManager>();

	player->start();
	offset = Vec2(0.0, 0.0);
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

	offset = Vec2(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	Graphics2D::SetTransform(Mat3x2::Translate(offset));
}

void Game::createActors() {
	if (Input::Key1.clicked) {
		auto enemy = std::make_shared<TestEnemy>();
		enemy->set(RandomVec2(stageSize.x, stageSize.y));
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
	for (auto i : step(30)) {
		RectF(RandomVec2(stageSize.x, stageSize.y).asPoint(), 40, 40).drawFrame(1.0, 0.0, Color(255, 255, 255, 127));
	}
	Rect(stageSize).drawFrame(1.0, 0.0, Palette::Red);
}
