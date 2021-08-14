#ifdef _MSC_VER
#include "include/game.h"
#else
#include "game.h"
#endif

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!",
                            sf::Style::Fullscreen);
    jam::Game::startGame(window);
    return 0;
}
