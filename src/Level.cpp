
#include "Level.h"
#include "store.h"
#include "makeAttackBuilding.h"
namespace {}
void jam::Level::addHero(const std::shared_ptr<Hero> &hero) {
    heroes.emplace_back(hero);
}
void jam::Level::addMonster(const std::shared_ptr<Monster> &monster) {
    monsters.emplace_back(monster);
}

void jam::Level::heroSetPosition(const sf::Vector2f &newPos, std::size_t i) {
    heroes[i]->setPosition(newPos);
}
void jam::Level::monsterSetScale(const sf::Vector2f &newScale, std::size_t i) {
    (*monsters[i]).setScale(newScale);
}
void jam::Level::monsterSetPosition(const sf::Vector2f &newPos, std::size_t i) {
    (*monsters[i]).setPosition(newPos);
}
void jam::Level::heroSetScale(const sf::Vector2f &newScale, std::size_t i) {
    (*heroes[i]).setScale(newScale);
}

jam::Level::Level(const std::vector<std::vector<int>> &mapObjects)
    : ability(FIRE_BLAST), elements(2, POWER_ELEMENT::FIRE) {
    //    heroes.emplace_back(Hero::makeAssasinLime(*this, {100, 50}));
    //    std::vector<sf::Vector2f> monster_path;
    //    monster_path.emplace_back(200, 200);
    //    monster_path.emplace_back(220, 280);
    //    monster_path.emplace_back(260, 340);
    //    auto weirdo = Monster::makeYeti(*this, monster_path);
    //    weirdo->setPosition(340, 300);
    //    monsters.emplace_back(weirdo);
    //    weirdo = Monster::makePirateGunnern(*this, monster_path);
    //    weirdo->setPosition(500, 500);
    //    monsters.emplace_back(weirdo);

    freeObjects.insert(makeTree({200, 300}));
    map.resize(mapObjects.size());
    for (int i = 0; i < mapObjects.size(); i++) {
        map[i].resize(mapObjects[i].size());
        for (int j = 0; j < mapObjects[i].size(); j++) {
            map[i][j].setPosInMap({j, i});
            map[i][j].setBackgroundType(mapObjects[i][j]);
        }
    }

    //    attackBuildings.insert(makeArcherBuilding(*this, {4, 4}));
    attackBuildings.insert(makeArcherBuilding(*this, {4, 5}));
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
            for (auto &i : heroes) {
                dynamic_cast<Hero &>(*i).event(event, window,
                                               clock1.getElapsedTime());
            }
            for (auto &i : money) {
                store.addMoney((*i).event(event, window));
            }
            store.event(event, window, mouse, *this);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (!readyToCast) {
                        continue;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        auto selectedCell =
                            sf::Vector2i{event.mouseButton.y / cellSize,
                                         event.mouseButton.x / cellSize};
                        switch (ability) {
                            case ABILITY::FIRE_BLAST:
                                for (int i = -1; i < 2; i++) {
                                    for (int j = -1; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::BLAST,
                                                   clock1.getElapsedTime());
                                        freeObjects.insert(
                                            jam::makeFire(sf::Vector2f(
                                                (float)bounds(
                                                    selectedCell.y + j, 0,
                                                    (int)map[0].size()) *
                                                        jam::cellSize +
                                                    (float)jam::cellSize / 2,
                                                (float)bounds(
                                                    selectedCell.x + i, 0,
                                                    (int)map.size()) *
                                                        jam::cellSize +
                                                    (float)jam::cellSize / 2)));
                                    }
                                }
                                break;
                            case ABILITY::CLOUD:
                                for (int i = -1; i < 2; i++) {
                                    for (int j = -1; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::CLOUD,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::LAVA:
                                for (int i = 0; i < 2; i++) {
                                    for (int j = 0; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::LAVA,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::FROZEN_BLAST:
                                for (int i = -2; i < 3; i++) {
                                    for (int j = -2; j < 3; j++) {
                                        if (std::abs(j) + std::abs(i) >= 3) {
                                            continue;
                                        }
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::FROZEN_BLAST,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::EARTHSHAKE:
                                for (int i = 0; i < 2; i++) {
                                    for (int j = 0; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::EARTHSHAKE,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::WALL:
                                map[selectedCell.x][selectedCell.y].setState(
                                    jam::WALL, clock1.getElapsedTime());
                                freeObjects.insert(jam::makeRock(sf::Vector2f(
                                    (float)selectedCell.y * jam::cellSize +
                                        (float)jam::cellSize / 2,
                                    (float)selectedCell.x * jam::cellSize +
                                        (float)jam::cellSize / 2)));
                                break;
                        }
                        readyToCast = false;
                    }
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::R) {
                        int whatAbility =
                            (1 << elements[0]) | (1 << elements[1]);
                        if (whatAbility == 4) {  // 100
                            ability = ABILITY::EARTHSHAKE;
                        } else if (whatAbility == 5) {  // 101
                            ability = ABILITY::LAVA;
                        } else if (whatAbility == 6) {  // 110
                            ability = ABILITY::WALL;
                        } else if (whatAbility == 2) {  // 010
                            ability = ABILITY::FROZEN_BLAST;
                        } else if (whatAbility == 3) {  // 011
                            ability = ABILITY::CLOUD;
                        } else if (whatAbility == 1) {  // 001
                            ability = ABILITY::FIRE_BLAST;
                        } else {
                            assert(0);
                        }
                        continue;
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        readyToCast = true;
                        continue;
                    }
                    elements[0] = elements[1];
                    if (event.key.code == sf::Keyboard::Z) {
                        elements[1] = POWER_ELEMENT::FIRE;
                    }
                    if (event.key.code == sf::Keyboard::X) {
                        elements[1] = POWER_ELEMENT::ICE;
                    }
                    if (event.key.code == sf::Keyboard::C) {
                        elements[1] = POWER_ELEMENT::EARTH;
                    }
                    break;
                default:
                    break;
            }
        }
        for (auto &i : map) {
            for (auto &j : i) {
                j.draw(window);
            }
        }
        std::sort(monsters.begin(), monsters.end(), charactersCompare);
        std::sort(heroes.begin(), heroes.end(), charactersCompare);
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

            float poses[5] = {objectPos, monsterPos, heroPos, flyingObjectPos,
                              buildingPos};
            std::sort(std::begin(poses), std::end(poses));
            if (poses[0] == objectPos) {
                freeObject->draw(window);
                auto cell =
                    sf::Vector2i(freeObject->getPosition() / (float)cellSize);
                if (freeObject->getObjectType() == ROCK &&
                        map[cell.y][cell.x].getState() != WALL ||
                    freeObject->getObjectType() == FIRE &&
                        map[cell.y][cell.x].getState() != BLAST ||
                    freeObject->getObjectType() != ROCK &&
                        map[cell.y][cell.x].getState() == WALL ||
                    freeObject->getObjectType() != FIRE &&
                        map[cell.y][cell.x].getState() == BLAST ||
                    map[cell.y][cell.x].getState() == EARTHSHAKE ||
                    map[cell.y][cell.x].getState() == LAVA) {
                    freeObject = freeObjects.erase(freeObject);
                    continue;
                } else if (map[cell.y][cell.x].getState() == FROZEN_BLAST) {
                    switch (freeObject->getObjectType()) {
                        case TREE:
                            freeObject->changeObjectType(FROZEN_TREE);
                            break;
                        case DEAD_TREE:
                            freeObject->changeObjectType(FROZEN_DEAD_TREE);
                            break;
                        case ROCK:
                            freeObject->changeObjectType(FROZEN_ROCK);
                            break;
                        case BUILD_SIGN:
                        case FIRE:
                        case NONE:
                        case FROZEN_TREE:
                        case FROZEN_DEAD_TREE:
                        case FROZEN_ROCK:
                        case NUMBER_OF_OBJECTS:
                            break;
                    }
                } else if (map[cell.y][cell.x].getState() == NORMAL) {
                    switch (freeObject->getObjectType()) {
                        case TREE:
                        case DEAD_TREE:
                        case ROCK:
                        case BUILD_SIGN:
                        case FIRE:
                        case NONE:
                        case NUMBER_OF_OBJECTS:
                            break;
                        case FROZEN_TREE:
                            freeObject->changeObjectType(TREE);
                            break;
                        case FROZEN_DEAD_TREE:
                            freeObject->changeObjectType(DEAD_TREE);
                            break;
                        case FROZEN_ROCK:
                            freeObject->changeObjectType(ROCK);
                            break;
                    }
                }
                freeObject++;
            } else if (poses[0] == monsterPos) {
                (*monster)->drawCharacter(window);
                if (!(*monster)->isDraw()) {
                    monster = monsters.erase(monster);
                } else {
                    monster++;
                }
            } else if (poses[0] == heroPos) {
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
        for (auto& i : money) {
            (*i).draw(window);
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
const std::vector<std::shared_ptr<TemplateCharacter>> &jam::Level::getHeroes()
    const {
    return heroes;
}
const std::vector<std::shared_ptr<TemplateCharacter>> &jam::Level::getMonsters()
    const {
    return monsters;
}
void jam::Level::addMoney(const std::shared_ptr<Money> &money_) {
    money.push_back(money_);
}
