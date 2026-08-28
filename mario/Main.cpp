#include <Siv3D.hpp>
#include "Player.h"
#include "Stage.h"

void Main()
{
	Window::Resize(1024, 896);
	Game game;

	Texture playerTexture(U"example/resource/チビマリオ.png");


	while (System::Update())
	{
		game.update();
		game.draw();


	}
}
