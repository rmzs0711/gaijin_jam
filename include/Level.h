#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <functional>
#include <limits>
#include <memory>
#include <set>
#include <tuple>
#include <variant>
#include <vector>
#include "Cell.h"
#include "building.h"
#include "characters.h"
#include "makeFreeObjects.h"
#include "usefulFunctions.h"

namespace {}

namespace jam {

struct Level {
    friend TemplateCharacter;
    friend Monster;
    friend Hero;

    friend Building;
    friend AttackBuilding;
    friend SupportBuilding;

    explicit Level(const std::vector<std::vector<int>> &mapObjects);

    void addHero(const std::shared_ptr<Hero> &hero);

    void addMonster(const std::shared_ptr<Monster> &monster);

    void heroSetPosition(const sf::Vector2f &newPos, std::size_t i = 0);
    void monsterSetScale(const sf::Vector2f &newScale, std::size_t i = 0);
    void monsterSetPosition(const sf::Vector2f &newPos, std::size_t i = 0);
    void heroSetScale(const sf::Vector2f &newScale, std::size_t i = 0);

    void updateStates();

    void draw(sf::RenderWindow &window);

    [[nodiscard]] std::vector<std::vector<Cell>> &getMap();

    [[nodiscard]] const std::set<FreeObject> &getFreeObjects() const;

    [[nodiscard]] const std::vector<std::shared_ptr<TemplateCharacter>>
        &getHeroes() const;
    [[nodiscard]] const std::vector<std::shared_ptr<TemplateCharacter>>
        &getMonsters() const;

private:
    std::vector<std::vector<Cell>> map;
    std::set<AttackBuilding> attackBuildings;
    std::vector<std::shared_ptr<TemplateCharacter>> heroes;
    std::vector<std::shared_ptr<TemplateCharacter>> monsters;
    std::set<FreeObject> freeObjects;
    std::list<FlyingObject> flyingObjects;
    sf::Clock clock1;
};
}  // namespace jam