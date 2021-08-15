#ifdef _MSC_VER
#include "include/game.h"
#else
#include "game.h"
#endif

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!",
                            sf::Style::Fullscreen);
    sf::Cursor cursor;
    sf::Image cursor_image;
    cursor_image.loadFromFile("data/images/Cursor3.png");
    cursor.loadFromPixels(cursor_image.getPixelsPtr(), cursor_image.getSize(), cursor_image.getSize() / 2U);
    window.setMouseCursor(cursor);
    jam::Game::startGame(window);
    return 0;
}
