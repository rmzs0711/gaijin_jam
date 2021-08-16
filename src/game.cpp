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
    if (type == "start") {
        RectangleButton<void> startGameButton([&]() { game.startGame(window); },
                                              "Start a new game");
        startGameButton.setSize(sizeBaseButton);
        startGameButton.setFillColor(sf::Color(74, 53, 27));
        startGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
                startGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(startGameButton);
    } else if (type == "close") {
        RectangleButton<void> closeGameButton(
            [&]() { game.closeGame(window, [&]() { window.close(); }); },
            "Exit");
        closeGameButton.setSize(sizeBaseButton);
        closeGameButton.setFillColor(sf::Color(74, 53, 27));
        closeGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
            closeGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(closeGameButton);
    }
    else if (type == "rules") {
        RectangleButton<void> rulesGameButton(
            [&]() {
                sf::Texture texture;
                texture.loadFromFile("data/images/rules_of_the_game.png");
                sf::RectangleShape shape;
                shape.setTexture(&texture);
                shape.setSize(sf::Vector2f(window.getView().getSize().x / 2.5, window.getView().getSize().y / 1.15));
                shape.setPosition(window.getView().getCenter() - shape.getSize() / 2.f);
                shape.setOutlineThickness(10);
                shape.setOutlineColor(sf::Color(112, 80, 40));
                while (window.isOpen()) {
                    sf::Event event{};
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
                            return;
                        }
                    }
                    window.draw(shape);
                    window.display();
                }
            },
            "The game rules");
        rulesGameButton.setSize(sizeBaseButton);
        rulesGameButton.setFillColor(sf::Color(74, 53, 27));
        rulesGameButton.setPosition(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2 -
            rulesGameButton.getSize().x / 2,
            position.y);
        return std::make_unique<RectangleButton<void>>(rulesGameButton);
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
    return sizeBaseButton.y + 50 + indent(number - 1);
}

void jam::Game::startGame(sf::RenderWindow &window) {
    jam::GameSession game;
    Menu mainMenu(window);
    mainMenu.addButton(
        makeButton(window, game, "start", sf::Vector2f(0, Game::indent(1))));
    mainMenu.addButton(
        makeButton(window, game, "rules", sf::Vector2f(0, Game::indent(2))));
    mainMenu.addButton(
        makeButton(window, game, "close", sf::Vector2f(0, Game::indent(3))));

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