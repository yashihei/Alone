#include <Siv3D.hpp>
#include <memory>

#include "Game.h"

void Main()
{
	auto game = std::make_unique<Game>();
	while (System::Update())
	{
		game->update();
		game->draw();
	}
}
