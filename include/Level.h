#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include <memory>
#include "Cell.h"
#include "makeCharacters.h"
#include "usefulFunctions.h"

namespace jam {

struct Level {
    explicit Level(const std::vector<std::vector<int>> &mapObjects) {
        heroes.push_back(makeAssasinLime(map));
        std::vector<sf::Vector2f> monster_path;
        monster_path.push_back(sf::Vector2f(200, 200));
        monster_path.push_back(sf::Vector2f(220, 280));
        monster_path.push_back(sf::Vector2f(260, 340));
        monsters.push_back(makeYeti(map, monster_path));
        monsters.push_back(makePirateGunnern(map, monster_path));


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
                if ((*monsters[i]).isLive()) {
                    monsters_.push_back(monsters[i].get());
                }
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
                if ((*heroes[i]).isLive()) {
                    heroes_.push_back(heroes[i].get());
                }
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

    const std::vector<std::shared_ptr<Hero>> &getHeroes() const {
        return heroes;
    }
    const std::vector<std::shared_ptr<MonsterStanding>> &getMonsters() const {
        return monsters;
    }

private:
    std::vector<std::vector<Cell>> map;

    std::vector<std::shared_ptr<Hero>> heroes;
    std::vector<std::shared_ptr<MonsterStanding>> monsters;
};
}  // namespace jam