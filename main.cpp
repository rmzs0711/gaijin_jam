#include "include/moving_object.h"
#include <cassert>

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Default);

    sf::Texture gay;
    if (!gay.loadFromFile("data/images/gay.png")) {
        assert(0);
    }
    sf::Texture il;
    if (!il.loadFromFile("data/images/i.png")) {
        assert(0);
    }

    std::vector<sf::Vector2f> point;
    point.push_back(sf::Vector2f(0, 0));
    point.push_back(sf::Vector2f(200, 0));
    point.push_back(sf::Vector2f(200, 200));
    point.push_back(sf::Vector2f(0, 200));
    move::Polygon<void> button(point, [&]() {}, sf::Vector2f(200, 600));
    button.setString("LaLa");
    button.setFillColor(sf::Color::Red);
    move::Object dragon("data/images/dragon.png", sf::Vector2f(50, 700));
    move::Circle<void> circle([&]() {});
    circle.setString("LoLo");
    circle.setFillColor(sf::Color::White);
    circle.setTexture(&gay);
    circle.setClickableTexture(&il);

    sf::Vector2f mouse;

    while (window.isOpen()) {
        window.clear(sf::Color(255, 160, 122));
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
                    button.move_object(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
                    dragon.move_object(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
                    circle.move_object(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
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