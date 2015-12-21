#include "Interface.h"

const Size Game::stageSize = Size(1000, 1000);

Game::Game() :
offset(0.0, 0.0), score(0)
{
	player = std::make_shared<Player>();
	bulletManager = std::make_shared<BulletManager>();
	enemyManager = std::make_shared<EnemyManager>();
	effect = std::make_shared<Effect>();
	effect->setSpeed(1.5);

	Graphics2D::SetBlendState(BlendState::Additive);
	FontManager::Register(L"dat/orbitron-medium.otf");
	FontAsset::Register(L"log", 10, L"Orbitron");
	FontAsset::Register(L"middleLog", 15, L"Orbitron");
	FontAsset::Register(L"smallLog", 7, L"Orbitron");
	FontAsset::Register(L"title", 30, L"Orbitron");

	player->start();
	logStrs.clear();
	addLog(L"GOOD LUCK");
}

void Game::update() {
	if (Input::KeySpace.pressed) return;
	if (Input::KeyF.clicked) Window::SetVirtualFullscreen({ 640, 480 });
	if (Input::KeyG.clicked) ScreenCapture::BeginGIF(Window::Size());
	if (Input::KeyY.clicked) addLog(L"HELLO YOJO");
	ClearPrint();

	player->update(this);
	enemyManager->update(this);
	bulletManager->update(this);

	createActors();

	offset = Vec2(Window::Width() / 2 - player->getPos().x, Window::Height() / 2 - player->getPos().y);
	Graphics2D::SetTransform(Mat3x2::Translate(offset));

	effect->update();
}

void Game::createActors() {
	if (System::FrameCount() % 300 == 0) {
		auto pos = RandomVec2(stageSize.x, stageSize.y);
		auto enemy = std::make_shared<MiddleEnemy>(pos);
		enemyManager->add(enemy);
		effect->add<CircleEffect>(pos, 50);
		addLog(Format(L"ENEMY APPEAR ", pos.asPoint().x, L", ", pos.asPoint().y));
	}
}

void Game::addLog(String str) {
	logStrs.push_front(str);
	if (logStrs.size() > 10) logStrs.pop_back();
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
	FontAsset(L"log").draw(Format(L"FPS:", Profiler::FPS()), Vec2(567.0, 455.0), Palette::Lightgreen);

	//draw minimap
	const Rect mapRect(100);
	Graphics2D::SetTransform(Mat3x2::Translate({Window::Width() - (mapRect.w + 20), 20}));
	const Vec2 playerPos = player->getPos();
	const double scale = 10.0;

	mapRect.draw(Color(0, 122, 255, 122)).drawFrame();
	Circle(playerPos / scale, 2.0).draw(Palette::Yellow);
	for (auto& enemy : *enemyManager) {
		Circle(enemy->getPos() / scale, 2.0).draw(Palette::Red);
	}

	//draw systemlog
	Graphics2D::SetTransform(Mat3x2::Translate(5.0, 465));
	int i = 0;
	for (const auto& str : logStrs) {
		FontAsset(L"smallLog").draw(str, Vec2(0.0, -10 * i), HSV(Palette::Lightgreen).toColorF(1.0 - 0.1 * i));
		i++;
	}

	Graphics2D::SetTransform(Mat3x2::Translate(offset));
}

void Game::drawBackground() {
	for (int i = 1; i < 20; i++) {
		const double lineY = stageSize.y / 20 * i;
		const double lineX = stageSize.x / 20 * i;
		Line(0.0, lineY, stageSize.x, lineY).draw(Color(255, 255, 255, 50));
		Line(lineX, 0.0, lineX, stageSize.y).draw(Color(255, 255, 255, 50));
	}
	RectF(stageSize).drawFrame(0.0, 1.0, Color(Palette::Red));
}
