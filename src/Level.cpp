
#include <Level.h>

void jam::Level::addHero(std::shared_ptr<Hero> hero) {
    heroes.push_back(hero);
}
void jam::Level::addMonster(std::shared_ptr<Monster> monster) {
    monsters.push_back(monster);
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
jam::Level::Level(const std::vector<std::vector<int>> &mapObjects) {
    heroes.push_back(Hero::makeAssasinLime(*this));
    std::vector<sf::Vector2f> monster_path;
    monster_path.push_back(sf::Vector2f(200, 200));
    monster_path.push_back(sf::Vector2f(220, 280));
    monster_path.push_back(sf::Vector2f(260, 340));
    monsters.push_back(Monster::makeYeti(*this, monster_path));
    monsters.push_back(Monster::makePirateGunnern(*this, monster_path));
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
void jam::Level::event(sf::Event &newEvent,
                       sf::RenderWindow &window,
                       const sf::Time &currentTime) {
    for (auto &i : heroes) {
        dynamic_cast<Hero &>(*i).event(newEvent, window, currentTime);
    }
}
void jam::Level::updateStates(const sf::Time &currentTime) {
    for (auto &i : map) {
        for (auto &j : i) {
            j.updateState(currentTime);
        }
    }
}
void jam::Level::draw(sf::RenderWindow &window) {
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
        } else {
            (*hero)->drawCharacter(window);
            if (!(*hero)->isDraw()) {
                hero = heroes.erase(hero);
            } else {
                hero++;
            }
        }
    }


}
std::vector<std::vector<jam::Cell>> &jam::Level::getMap() {
    return map;
}
const std::list<jam::FreeObject> &jam::Level::getfreeObjects() const {
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
