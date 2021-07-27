#include <cassert>
#include "SFML/Graphics.hpp"
#include "include/button.h"

int main() {

    sf::CircleShape circle;
    circle.setRadius(100);
    circle.setOrigin(100, 100);
    //    circle.setPosition(sf::Vector2f(400, 300));
    circle.setFillColor(sf::Color::White);
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    sf::Text text;
    sf::Font font;
    assert(font.loadFromFile("../data/fonts/mono.otf"));
    text.setFont(font);
    text.setPosition(0, 0);
    text.setCharacterSize(100);
    text.setString("11111");
    text.setFillColor(sf::Color::White);

    RectangleButton<void> button([&]() { window.close(); }, "");
    button.setSize({100, 100});
    button.setPosition({400, 300});
    button.setFillColor(sf::Color::Green);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    button.handleClick();
                }
            }
        }

        button.drawButton(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
