#include "button.h"
#include "gameSession.h"
#include "menu.h"

#ifdef _MSC_VER
#include "../include/button.h"
#include "../include/gameSession.h"
#include "../include/menu.h"
#endif

int main() {
    sf::RenderWindow window(sf::VideoMode(), "SFML works!",
                            sf::Style::Fullscreen);
    jam::GameSession game;
    RectangleButton<void> startGameButton([&]() { game.startGame(window); });
    startGameButton.setSize({100.f, 50.f});
    startGameButton.setFillColor(sf::Color::Blue);
    startGameButton.setPosition(0, 100);

    std::vector<std::unique_ptr<Button<void>>> mainMenuButtons;
    mainMenuButtons.push_back(
        std::move(std::make_unique<RectangleButton<void>>(startGameButton)));

    Menu mainMenu;
    mainMenu.setSize(sf::Vector2f(window.getSize()));
    mainMenu.setColor(sf::Color::Yellow);
    mainMenu.setButtons(mainMenuButtons);
    sf::Event event{};
    window.pollEvent(event);
    while (window.isOpen()) {
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
