#include "Game.h"

#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Effect.h"

const Size Game::stageSize = Size(1000, 1000);

Game::Game() :
offset(0.0, 0.0), score(0)
{
	player = std::make_shared<Player>();
	bulletManager = std::make_shared<BulletManager>();
	enemyManager = std::make_shared<EnemyManager>();
	effect = std::make_shared<Effect>();

	player->start();
	Graphics2D::SetBlendState(BlendState::Additive);
	FontManager::Register(L"dat/orbitron-medium.otf");
	FontAsset::Register(L"log", 10, L"Orbitron");
}

void Game::update() {
	if (Input::KeySpace.pressed) return;
	ClearPrint();

	player->update(this);
	enemyManager->update(this);
	bulletManager->update(this);

	createActors();

	offset = Vec2(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	Graphics2D::SetTransform(Mat3x2::Translate(offset));

	effect->update();
	if (Input::Key2.pressed) {
		effect->add<Snow>(player->getPos().asPoint(), 10);
	}
	if (Input::Key3.clicked) {
		effect->add<CrashEffect>(player->getPos());
	}
}

void Game::createActors() {
	if (Input::Key1.clicked) {
		auto enemy = std::make_shared<MiddleEnemy>();
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
	Graphics2D::SetTransform(Mat3x2::Identity());
	FontAsset(L"log").draw(Format(L"SCORE:", score), Vec2(5.0, 5.0), Palette::Lightgreen);
	FontAsset(L"log").draw(Format(L"FPS:", Profiler::FPS()), Vec2(5.0, 25.0), Palette::Lightgreen);

	//draw minimap
	const Rect mapRect(100);
	Graphics2D::SetTransform(Mat3x2::Translate(Vec2(Window::Width() - (mapRect.w + 20), 20)));
	const Vec2 playerPos = player->getPos();
	const double scale = 10.0;

	mapRect.draw(Color(0, 122, 255, 122)).drawFrame();
	Circle(playerPos / scale, 2.0).draw(Palette::Yellow);
	for (auto& enemy : *enemyManager) {
		Circle(enemy->getPos() / scale, 2.0).draw(Palette::Red);
	}
	Graphics2D::SetTransform(Mat3x2::Translate(offset));
}

void Game::drawBackground() {
	for (auto i : step(30)) {
		RectF(RandomVec2(stageSize.x, stageSize.y).asPoint(), 40, 40).drawFrame(1.0, 0.0, Color(255, 255, 255, 127));
	}
	RectF(stageSize).drawFrame(0.0, 1.0, Color(Palette::Red));
}
