#pragma once

#include "button.h"
#include "gameSession.h"
#include "menu.h"

namespace jam {
	struct Game {
		static std::unique_ptr<RectangleButton<void>> makeButton(sf::RenderWindow& window, GameSession& game, std::string type, sf::Vector2f position);
		static float indent(int number);
		static void startGame(sf::RenderWindow &window);
	};
}