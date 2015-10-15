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
	effect->setSpeed(1.5);

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

	if (Input::KeyS.pressed) {
		Graphics2D::SetTransform(Mat3x2::Scale(2.0, Vec2(500, 500)));
	}

	effect->update();
}

void Game::createActors() {
	if (System::FrameCount() % 300 == 0) {
		auto pos = RandomVec2(stageSize.x, stageSize.y);
		auto enemy = std::make_shared<MiddleEnemy>();
		enemy->set(pos);
		enemyManager->add(enemy);
		effect->add<CircleEffect>(pos, 50);
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
	FontAsset(L"log").draw(Format(L"HP:",player->getHp()), Vec2(5.0, 25.0), Palette::Lightgreen);
	FontAsset(L"log").draw(Format(L"FPS:", Profiler::FPS()), Vec2(565.0, 455.0), Palette::Lightgreen);

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
	for (int i = 1; i < 20; i++) {
		double lineY = stageSize.y / 20 * i;
		double lineX = stageSize.x / 20 * i;
		Line(0.0, lineY, stageSize.x, lineY).draw(Color(255, 255, 255, 50));
		Line(lineX, 0.0, lineX, stageSize.y).draw(Color(255, 255, 255, 50));
	}
	RectF(stageSize).drawFrame(0.0, 1.0, Color(Palette::Red));
}
