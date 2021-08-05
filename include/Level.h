#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include <memory>
#include "Cell.h"
#include "characters.h"
#include "usefulFunctions.h"

namespace jam {

struct Level {
    explicit Level(const std::vector<std::vector<CellObject>> &mapObjects) {
        heroes.push_back(std::make_unique<Hero>("data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png", 100, 0.1, map));
        monsters.push_back(std::make_unique<MonsterStanding>("data/images/MiniWorldSprites/Characters/Monsters/Demons/ArmouredRedDemon.png", 50, 0.1, map));
        monsters.push_back(std::make_unique<MonsterStanding>("data/images/MiniWorldSprites/Characters/Monsters/Demons/PurpleDemon.png", 200, 0.1, map));


        map.resize(mapObjects.size());
        for (std::size_t i = 0; i < mapObjects.size(); i++) {
            map[i].resize(mapObjects[i].size());
            for (std::size_t j = 0; j < mapObjects[i].size(); j++) {
                map[i][j].setPosition(sf::Vector2f(
                    sf::Vector2<size_t>(j * cellSize, i * cellSize)));
                map[i][j].setObject(mapObjects[i][j]);
            }
        }
    }
    void heroSetPosition(const sf::Vector2f &newPos, std::size_t i = 0) {
        (*heroes[i]).setPosition(newPos);
    }
    void monsterSetScale(const sf::Vector2f &newScale, std::size_t i = 0) {
        (*monsters[i]).setScale(newScale);
    }
    void monsterSetPosition(const sf::Vector2f& newPos, std::size_t i = 0) {
        (*monsters[i]).setPosition(newPos);
    }
    void heroSetScale(const sf::Vector2f& newScale, std::size_t i = 0) {
        (*heroes[i]).setScale(newScale);
    }

    void event(sf::Event &newEvent,
               sf::RenderWindow &window,
               const sf::Time &currentTime) {
        for (auto &i : heroes) {
            (*i).event(newEvent, window, currentTime);
        }
    }

    void updateStates(const sf::Time &currentTime) {
        for (auto &i : map) {
            for (auto &j : i) {
                j.updateState(currentTime);
            }
        }
    }

    void draw(sf::RenderWindow &window) {
        for (auto &i : map) {
            for (auto &j : i) {
                j.draw(window);
            }
        } 
       
        std::vector<TemplateCharacter*> monsters_;
        for (int i = 0; i < monsters.size(); i++) {
            if ((*monsters[i]).isDraw()) {
                monsters_.push_back(monsters[i].get());
            }
            else {
                monsters.erase(monsters.begin() + i);
                i--;
            }
        }

        for (auto& i : heroes) {
            (*i).drawCharacter(window, monsters_);
        }

        std::vector<TemplateCharacter*> heroes_;
        for (int i = 0; i < heroes.size(); i++) {
            if ((*heroes[i]).isDraw()) {
                heroes_.push_back(heroes[i].get());
            }
            else {
                heroes.erase(heroes.begin() + i);
                i--;
            }
        }
        for (auto& i : monsters) {
            (*i).drawCharacter(window, heroes_);
        }

    }

    [[nodiscard]] const std::vector<std::vector<Cell>> &getMap() const {
        return map;
    }

private:
    std::vector<std::vector<Cell>> map;
    std::vector<std::unique_ptr<Hero>> heroes;
    std::vector<std::unique_ptr<MonsterStanding>> monsters;
};
}  // namespace jam