#include "moving_object.h"

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Default);

    std::vector<sf::Vector2f> point;
    point.push_back(sf::Vector2f(0, 0));
    point.push_back(sf::Vector2f(200, 0));
    point.push_back(sf::Vector2f(200, 200));
    point.push_back(sf::Vector2f(0, 200));
    Polygon button(point, sf::Vector2f(300, 500), JUST_POLYGON, sf::Color::Red, 10);
    Object dragon("image/dragon.png", sf::Vector2f(50, 700));
    Circle circle(92, sf::Vector2f(200, 100), JUST_POLYGON, sf::Color::Red, 10);

    sf::Vector2f mouse;

    while (window.isOpen()) {
        window.clear();
        button.draw(window);
        dragon.draw(window);
        circle.draw(window);
        sf::Event event;

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::KeyPressed: {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                } break;
                case sf::Event::Closed: {
                    window.close();
                } break;
                case sf::Event::MouseMoved: {
                    button.move(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
                    dragon.move(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
                    circle.move(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
                } break;
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        button.click_mouse_left(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
                        dragon.click_mouse_left(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
                        circle.click_mouse_left(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
                    }
                    else {
                        if (event.mouseButton.button == sf::Mouse::Right) {
                            button.click_mouse_right(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
                            dragon.click_mouse_right(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
                            circle.click_mouse_right(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
                        }
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