#ifdef _MSC_VER
#include "../include/game.h"
#else
#include "game.h"
#endif

const sf::Vector2f sizeBaseButton = {390, 80};

inline std::unique_ptr<RectangleButton<void>> jam::Game::makeButton(
    sf::RenderWindow &window,
    jam::GameSession &game,
    std::string type,
    sf::Vector2f position) {
    if (type == "default") {
        RectangleButton<void> startGameButton(
            [&]() { game.startGame(window, 0); }, "Hello!");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
                startGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    } else if (type == "moscow") {
        RectangleButton<void> startGameButton(
            [&]() { game.startGame(window, 1); }, "Press");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
                startGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    } else if (type == "dota") {
        RectangleButton<void> startGameButton(
            [&]() { game.startGame(window, 2); }, "any");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
                startGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    } else if (type == "panfilov") {
        RectangleButton<void> startGameButton(
            [&]() { game.startGame(window, 3); }, "button");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
            startGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    } else if (type == "close") {
        RectangleButton<void> startGameButton(
            [&]() { game.closeGame(window, [&]() { window.close(); }); },
            "Not this");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
                startGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    }

    assert(false);
}

float jam::Game::indent(int number) {
    if (number == 1) {
        return 200;
    }
    if (number == 2) {
        return 200 + sizeBaseButton.y + 50;
    }
    if (number == 3) {
        return 200 + 2 * (sizeBaseButton.y + 50);
    }
    if (number == 4) {
        return 200 + 3 * (sizeBaseButton.y + 50);
    }
    if (number == 5) {
        return 200 + 4 * (sizeBaseButton.y + 50);
    }
    return sizeBaseButton.y + 50 + indent(number - 1);
}

void jam::Game::startGame(sf::RenderWindow &window) {
    jam::GameSession game;
    Menu mainMenu(window);
    mainMenu.addButton(
        makeButton(window, game, "default", sf::Vector2f(0, Game::indent(1))));
    mainMenu.addButton(
        makeButton(window, game, "moscow", sf::Vector2f(0, Game::indent(2))));
    mainMenu.addButton(
        makeButton(window, game, "dota", sf::Vector2f(0, Game::indent(3))));
    mainMenu.addButton(
        makeButton(window, game, "panfilov", sf::Vector2f(0, Game::indent(4))));
    mainMenu.addButton(
        makeButton(window, game, "close", sf::Vector2f(0, Game::indent(5))));
    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (auto &b : mainMenu.getButtons()) {
                            if (b->isCorrectClick(sf::Vector2f(
                                    sf::Mouse::getPosition(window)))) {
                                b->handleClick();
                            }
                        }
                    }
            }
        }
        window.draw(mainMenu);
        window.display();
    }
}