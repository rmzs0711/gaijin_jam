#include "include/moving_object.h"
#include "include/character.h"
#include <cassert>

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Default);
    sf::Vector2f mouse;

    Character EntitySprite("data/images/up.png", "data/images/right.png", "data/images/down.png", "data/images/left.png", false,
        "data/images/move_up.png", "data/images/move_right.png", "data/images/move_down.png", "data/images/move_left.png");

    while (window.isOpen()) {
        window.clear(sf::Color(255, 160, 122));
        sf::Event event;
        EntitySprite.draw(window);
        while (window.pollEvent(event)) {
            float dy = 0, dx = 0;
            switch (event.type) {
                case sf::Event::KeyPressed: {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    EntitySprite.keyPressed(event);
                } break;
                case sf::Event::Closed: {
                    window.close();
                } break;
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        EntitySprite.click_mouse(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));
                    }
                } break;
                default: {
                }
            }
        }

        window.display();
    }

    return 0;
}