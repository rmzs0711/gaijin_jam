
#include <Level.h>
#include <store.h>
#include "makeAttackBuilding.h"
namespace {}
void jam::Level::addHero(const std::shared_ptr<Hero> &hero) {
    heroes.insert(hero);
}
void jam::Level::addMonster(const std::shared_ptr<Monster> &monster) {
    monsters.insert(monster);
}
jam::Level::Level(const std::vector<std::vector<int>> &mapObjects) {
    heroes.insert(Hero::makeAssasinLime(*this, {100, 50}));
    std::vector<sf::Vector2f> monster_path;
    monster_path.emplace_back(200, 200);
    monster_path.emplace_back(220, 280);
    monster_path.emplace_back(260, 340);
    auto weirdo = Monster::makeYeti(*this, monster_path);
    weirdo->setPosition(340, 300);
    monsters.insert(weirdo);
    weirdo = Monster::makePirateGunnern(*this, monster_path);
    weirdo->setPosition(500, 500);
    monsters.insert(weirdo);

    freeObjects.insert(makeTree({200, 300}));
    map.resize(mapObjects.size());
    for (int i = 0; i < mapObjects.size(); i++) {
        map[i].resize(mapObjects[i].size());
        for (int j = 0; j < mapObjects[i].size(); j++) {
            map[i][j].setPosInMap({j, i});
            map[i][j].setBackgroundType(mapObjects[i][j]);
        }
    }

    attackBuildings.insert(makeArcherBuilding(*this, {4, 4}));
}

void jam::Level::updateStates() {
    for (auto &i : map) {
        for (auto &j : i) {
            j.updateState(clock1.getElapsedTime());
        }
    }
    for (auto &i : freeObjects) {
        i.updateAnimation();
    }
    for (auto &i : attackBuildings) {
        i.attack(clock1.getElapsedTime());
    }
}
void jam::Level::draw(sf::RenderWindow &window) {
    Store store(window);
    clock1.restart();
    sf::Vector2f mouse;
    while (window.isOpen()) {
        window.clear();
        updateStates();
        sf::Event event{};
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                default:
                    for (auto &i : heroes) {
                        dynamic_cast<Hero &>(*i).event(event, window,
                                                       clock1.getElapsedTime());
                    }
                    store.event(event, window, mouse, *this);
                    break;
            }
        }
        for (auto &i : map) {
            for (auto &j : i) {
                j.draw(window);
            }
        }
        auto freeObject = freeObjects.begin();
        auto monster = monsters.begin();
        auto hero = heroes.begin();
        auto flyingObject = flyingObjects.begin();
        auto building = attackBuildings.begin();
        for (; freeObject != freeObjects.end() || monster != monsters.end() ||
               hero != heroes.end() || flyingObject != flyingObjects.end() ||
               building != attackBuildings.end();) {
            auto objectPos = freeObject != freeObjects.end()
                                 ? freeObject->getPosition().y
                                 : std::numeric_limits<float>::max();
            auto monsterPos = monster != monsters.end()
                                  ? (*monster)->getSprite()->getPosition().y
                                  : std::numeric_limits<float>::max();
            auto heroPos = hero != heroes.end()
                               ? (*hero)->getSprite()->getPosition().y
                               : std::numeric_limits<float>::max();
            auto flyingObjectPos = flyingObject != flyingObjects.end()
                                       ? flyingObject->getPosition().y
                                       : std::numeric_limits<float>::max();
            auto buildingPos = building != attackBuildings.end()
                ? building->getHitBox().top
                : std::numeric_limits<float>::max();

            float poses[5] = {objectPos, monsterPos, heroPos,
                flyingObjectPos, buildingPos};
            std::sort(std::begin(poses), std::end(poses));
            if (poses[0] == objectPos) {
                freeObject->draw(window);
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
                (*monster)->drawCharacter(window);
                if (!(*monster)->isDraw()) {
                    monster = monsters.erase(monster);
                } else {
                    monster++;
                }
            } else if (poses[0] == heroPos){
                (*hero)->drawCharacter(window);
                if (!(*hero)->isDraw()) {
                    hero = heroes.erase(hero);
                } else {
                    hero++;
                }
            } else if (poses[0] == flyingObjectPos) {
                flyingObject->draw(window);
                if (flyingObject->isFinished()) {
                    flyingObject = flyingObjects.erase(flyingObject);
                } else {
                    flyingObject++;
                }
            } else if (poses[0] == buildingPos) {
                building->draw(window);
                building++;
            }
        }
        store.drawStore(window);

        window.display();
    }
}
std::vector<std::vector<jam::Cell>> &jam::Level::getMap() {
    return map;
}
const std::set<jam::FreeObject> &jam::Level::getFreeObjects() const {
    return freeObjects;
}
const std::set<std::shared_ptr<TemplateCharacter>, CharactersCompare>
    &jam::Level::getHeroes() const {
    return heroes;
}
const std::set<std::shared_ptr<TemplateCharacter>, CharactersCompare>
    &jam::Level::getMonsters() const {
    return monsters;
}
