
#include "Game.h"

int main() {
	const auto game = new Game();
	game->Init(1280, 768, "Sandbox");
	game->Run();
	delete game;
}
