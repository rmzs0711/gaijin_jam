#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <limits>
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
            dynamic_cast<Hero &>(*i).event(newEvent, window, currentTime);
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
        auto freeObject = freeObjects.begin();
        auto monster = monsters.begin();
        auto hero = heroes.begin();
        for (; freeObject != freeObjects.end() || monster != monsters.end() ||
               hero != heroes.end();) {
            auto objectPos = freeObject != freeObjects.end()
                                 ? freeObject->getPosition().y
                                 : std::numeric_limits<float>::max();
            auto monsterPos = monster != monsters.end()
                                  ? (*monster)->getSprite()->getPosition().y
                                  : std::numeric_limits<float>::max();
            auto heroPos = hero != heroes.end()
                               ? (*hero)->getSprite()->getPosition().y
                               : std::numeric_limits<float>::max();
            float poses[3] = {objectPos, monsterPos, heroPos};
            std::sort(std::begin(poses), std::end(poses));
            if (poses[0] == objectPos) {
                auto cell =
                    sf::Vector2i(freeObject->getPosition() / (float)cellSize);
                if (freeObject->getObjectType() == ROCK &&
                        map[cell.y][cell.x].getState() != WALL ||
                    freeObject->getObjectType() == FIRE &&
                        map[cell.y][cell.x].getState() != BLAST) {
                    freeObject = freeObjects.erase(freeObject);
                } else {
                    freeObject++;
                }
            } else if (poses[0] == monsterPos) {
                (*monster)->drawCharacter(window, heroes);
                if (!(*monster)->isDraw()) {
                    monster = monsters.erase(monster);
                } else {
                    monster++;
                }
            } else {
                (*hero)->drawCharacter(window, monsters);
                if (!(*hero)->isDraw()) {
                    hero = heroes.erase(hero);
                } else {
                    hero++;
                }
            }
        }

        //        for (auto i = freeObjects.begin(); i != freeObjects.end();) {
        //            i->draw(window);
        //            auto cell = sf::Vector2i(i->getPosition() /
        //            (float)cellSize);
        //
        //            if (i->getObjectType() == ROCK &&
        //                    map[cell.y][cell.x].getState() != WALL ||
        //                i->getObjectType() == FIRE &&
        //                    map[cell.y][cell.x].getState() != BLAST) {
        //                i = freeObjects.erase(i);
        //                continue;
        //            }
        //            i++;
        //        }
        //
        //        for (int i = 0; i < monsters.size(); i++) {
        //
        //        }

        //        for (int i = 0; i < heroes.size(); i++) {
        //            heroes[i]->drawCharacter(window, monsters);
        //            if (!(*heroes[i]).isDraw()) {
        //                heroes.erase(heroes.begin() + i);
        //                i--;
        //            }
        //        }
    }

    [[nodiscard]] const std::vector<std::vector<Cell>> &getMap() const {
        return map;
    }

    const std::vector<std::shared_ptr<TemplateCharacter>> &getHeroes() const {
        return heroes;
    }
    const std::vector<std::shared_ptr<TemplateCharacter>> &getMonsters() const {
        return monsters;
    }

private:
    std::vector<std::vector<Cell>> map;

    std::vector<std::shared_ptr<TemplateCharacter>> heroes;
    std::vector<std::shared_ptr<TemplateCharacter>> monsters;
    std::list<FreeObject> freeObjects;
};
}  // namespace jam