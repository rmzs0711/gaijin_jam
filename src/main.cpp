#include "moving_object.h"
#include "characters.h"
#include <cassert>
#include <iostream>

std::vector<sf::Sprite *> objects;

// нажми сначала правой на перса, он выделится, и им ходить можно будет

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Default);
    CharacterMouse character2("data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png", false); // убери false, если хочешь, чтоб он всегда ходил, без выделений
    character2.setScale(4, 4);

    sf::Image pop;
    sf::Texture pop2;
    sf::Sprite pop3;
    pop.loadFromFile("data/images/MiniWorldSprites/Ground/TexturedGrass.png");
    pop2.loadFromImage(pop);
    pop3.setTexture(pop2);
    pop3.setPosition(300, 500);
    objects.push_back(&pop3);

    while (window.isOpen()) {
        window.clear(sf::Color(255, 160, 122));
        sf::Event event;
        window.draw(pop3);
        character2.drawCharacter(window);

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
                default: {
                }
            }

            character2.event(event, window);
        }

        window.display();
    }

    return 0;
}