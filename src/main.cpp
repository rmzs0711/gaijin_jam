#include "button.h"
#include "gameSession.h"
#include "menu.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!");
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
