#ifdef _MSC_VER
#include "include/button.h"
#include "include/gameSession.h"
#include "include/menu.h"
#else
#include "button.h"
#include "gameSession.h"
#include "menu.h"
#endif

const sf::Vector2f sizeBaseButton = {390, 80};

inline std::unique_ptr<RectangleButton<void>> makeButton(sf::RenderWindow &window, jam::GameSession &game, std::string type, sf::Vector2f position) {
    if (type == "start") {
        RectangleButton<void> startGameButton([&]() { game.startGame(window); }, "Start a new game");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize())).x 
            / 2 - startGameButton.getSize().x / 2, position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    }
    else if (type == "close") {
        RectangleButton<void> startGameButton([&]() { game.closeGame(window); }, "Get out of the game");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(window.mapPixelToCoords(sf::Vector2i(window.getSize())).x
            / 2 - startGameButton.getSize().x / 2, position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    }

    assert(false);
}

float indent(int number) {
    if (number == 1) {
        return 200;
    }
    if (number == 2) {
        return 200 + sizeBaseButton.y + 50;
    }
    return sizeBaseButton.y + 50 + indent(number - 1);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Fullscreen);
    jam::GameSession game;
    Menu mainMenu(window);
    mainMenu.addButton(makeButton(window, game, "start", sf::Vector2f(0, indent(1))));
    mainMenu.addButton(makeButton(window, game, "close", sf::Vector2f(0, indent(2))));

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    switch (event.mouseButton.button) {
                        case sf::Mouse::Left:
                            for (auto &b : mainMenu.getButtons()) {
                                if (b->isCorrectClick(sf::Vector2f(
                                        sf::Mouse::getPosition(window)))) {
                                    b->handleClick();
                                }
                            }
                        default:
                            break;
                    }
                default:
                    break;
            }
        }
        window.draw(mainMenu);
        window.display();
    }

    return 0;
}
