#include "include/moving_object.h"
#include "include/characters.h"
#include <cassert>
#include <iostream>

std::vector<sf::Sprite *> not_move_objects;
std::vector<TemplateCharacter *> monsters;
std::vector<TemplateCharacter*> heroes;

int main() {
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Default);
    Hero hero("data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png", 100, 0.5, false);
    MonsterStanding monster("data/images/MiniWorldSprites/Characters/Monsters/Demons/ArmouredRedDemon.png", 100, 0.2);
    monster.setScale(4, 4);
    hero.setScale(4, 4);
    hero.setPosition(600, 200);
    monster.setPosition(800, 500);

    sf::Image pop;
    sf::Texture pop2;
    sf::Sprite pop3;
    pop.loadFromFile("data/images/MiniWorldSprites/Ground/TexturedGrass.png");
    pop2.loadFromImage(pop);
    pop3.setTexture(pop2);
    pop3.setPosition(300, 500);
    not_move_objects.push_back(&pop3);
    monsters.push_back(&monster);
    heroes.push_back(&hero);

    while (window.isOpen()) {
        window.clear(sf::Color(255, 160, 122));
        sf::Event event;
        window.draw(pop3);
        hero.drawCharacter(window);
        monster.drawCharacter(window);

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

            hero.event(event, window);
           // monster.event(event, window);
        }

        window.display();
    }

    return 0;
}