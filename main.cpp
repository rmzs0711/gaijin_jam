#include <cassert>
#include "SFML/Graphics.hpp"
#include "include/button.h"
#include "include/menu.h"

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

    sf::Texture gay;
    if (!gay.loadFromFile("../data/images/gay.png")) {
        assert(0);
    }
    sf::Texture il;
    if (!il.loadFromFile("../data/images/i.png")) {
        assert(0);
    }
    sf::Sprite back;

    back.setTexture(gay);
    back.setScale(static_cast<float>(window.getSize().x) / gay.getSize().x,
                  static_cast<float>(window.getSize().y) / gay.getSize().y);

    CircleButton<void> button([&]() { window.close(); }, "");
    button.setRadius(100);
    button.setPosition({400, 300});
    button.setTexture(&gay);
    button.setClickableTexture(&il);
    //
    Menu mainMenu;
    mainMenu.setBackground(back);
    std::vector<std::unique_ptr<Button<void>>> buttons;
    //    std::unique_ptr<Button<void>> x();
    buttons.push_back(std::move(std::make_unique<CircleButton<void>>(button)));
    mainMenu.setButtons(buttons);

    while (window.isOpen()) {
        //        window.draw(back);
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (auto &i : mainMenu.getButtons()) {
                        i->handleClick();
                    }
                }
            }
        }
        window.draw(mainMenu);
        //        button.drawButton(circle);
//        button.drawButton(window);
        window.display();
    }
    return EXIT_SUCCESS;
}
