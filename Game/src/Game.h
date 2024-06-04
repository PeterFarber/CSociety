#ifndef GAME_H
#define GAME_H

#include <Society/Core/Window.h>

#include "Player.h"

namespace sc {
	class Engine;
}

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE, GAME_MENU, GAME_WIN
};

class Game {
	private:
		/* data */
		sc::Engine *m_engine{};

		double deltaTime = 0.0f;
		double lastFrame = 0.0f;

		std::vector<sc::Entity *> m_entities;

	public:
		Game();

		~Game();

		void Init(unsigned int width, unsigned int height, const char *title);

		void Update(double dt) const;

		void Run();
};

#endif

