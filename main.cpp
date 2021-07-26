#include "SFML/Graphics.hpp"
#include "include/button.h"
int main() {
    sf::CircleShape circle;
    circle.setRadius(100);
    circle.setOrigin(100, 100);
//    circle.setPosition(sf::Vector2f(400, 300));
    circle.setFillColor(sf::Color::White);
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");


    CircleButton<void> button([]() {}, "1");
    button.setPosition({0, 0});
    while (window.isOpen()) {
        // Process events
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.draw(circle);
        window.display();
    }
    return EXIT_SUCCESS;
}
