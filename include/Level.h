#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include <set>
#include <tuple>
#include <variant>
#include <vector>
#include "Cell.h"
#include "characters.h"
#include "makeCharacters.h"
#include "makeFreeObjects.h"
#include "usefulFunctions.h"
struct TemplateCharacter;
namespace jam {

struct Level {
    explicit Level(const std::vector<std::vector<int>> &mapObjects) {
        heroes.push_back(makeAssasinLime(map, freeObjects));
        std::vector<sf::Vector2f> monster_path;
        monster_path.push_back(sf::Vector2f(200, 200));
        monster_path.push_back(sf::Vector2f(220, 280));
        monster_path.push_back(sf::Vector2f(260, 340));
        monsters.push_back(makeYeti(map, freeObjects, monster_path));
        monsters.push_back(makePirateGunnern(map, freeObjects, monster_path));
        freeObjects.push_back(makeTree({200, 300}));
        map.resize(mapObjects.size());
        for (int i = 0; i < mapObjects.size(); i++) {
            map[i].resize(mapObjects[i].size());
            for (int j = 0; j < mapObjects[i].size(); j++) {
                map[i][j].setPosInMap({j, i});
                map[i][j].setBackgroundType(mapObjects[i][j]);
            }
        }
    }

    void addHero(std::shared_ptr<Hero> hero) {
        heroes.push_back(hero);
    }

    void addMonster(std::shared_ptr<Monster> monster) {
        monsters.push_back(monster);
    }

    void heroSetPosition(const sf::Vector2f &newPos, std::size_t i = 0) {
        (*heroes[i]).setPosition(newPos);
    }
    void monsterSetScale(const sf::Vector2f &newScale, std::size_t i = 0) {
        (*monsters[i]).setScale(newScale);
    }
    void monsterSetPosition(const sf::Vector2f &newPos, std::size_t i = 0) {
        (*monsters[i]).setPosition(newPos);
    }
    void heroSetScale(const sf::Vector2f &newScale, std::size_t i = 0) {
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
        for (auto i = freeObjects.begin(); i != freeObjects.end();) {
            i->draw(window);
            auto cell = sf::Vector2i(i->getPosition() / (float)cellSize);

            if (i->getObjectType() == ROCK &&
                    map[cell.y][cell.x].getState() != WALL ||
                i->getObjectType() == FIRE &&
                    map[cell.y][cell.x].getState() != BLAST) {
                i = freeObjects.erase(i);
                continue;
            }

            i++;
        }
        std::vector<TemplateCharacter *> monsters_;
        for (int i = 0; i < monsters.size(); i++) {
            if ((*monsters[i]).isDraw()) {
                if ((*monsters[i]).isLive()) {
                    monsters_.push_back(monsters[i].get());
                }
            } else {
                monsters.erase(monsters.begin() + i);
                i--;
            }
        }

        for (auto &i : heroes) {
            (*i).drawCharacter(window, monsters_);
        }

        std::vector<TemplateCharacter *> heroes_;
        for (int i = 0; i < heroes.size(); i++) {
            if ((*heroes[i]).isDraw()) {
                if ((*heroes[i]).isLive()) {
                    heroes_.push_back(heroes[i].get());
                }
            } else {
                heroes.erase(heroes.begin() + i);
                i--;
            }
        }
        for (auto &i : monsters) {
            (*i).drawCharacter(window, heroes_);
        }
    }

    [[nodiscard]] std::vector<std::vector<Cell>> &getMap() {
        return map;
    }

    [[nodiscard]] std::list<FreeObject>& getfreeObjects()  {
        return freeObjects;
    }

    const std::vector<std::shared_ptr<Hero>> &getHeroes() const {
        return heroes;
    }
    const std::vector<std::shared_ptr<Monster>> &getMonsters() const {
        return monsters;
    }

private:
    std::vector<std::vector<Cell>> map;

    std::vector<std::shared_ptr<Hero>> heroes;
    std::vector<std::shared_ptr<Monster>> monsters;
    std::list<FreeObject> freeObjects;
};
}  // namespace jam