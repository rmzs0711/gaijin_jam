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
#include "makeFreeObjects.h"
#include "usefulFunctions.h"
struct TemplateCharacter;
struct Monster;
struct Hero;
namespace jam {

struct Level {
    friend TemplateCharacter;
    friend Monster;
    friend Hero;

    explicit Level(const std::vector<std::vector<int>> &mapObjects) ;

    void addHero(std::shared_ptr<Hero> hero);

    void addMonster(std::shared_ptr<Monster> monster) ;

    void heroSetPosition(const sf::Vector2f &newPos, std::size_t i = 0) ;
    void monsterSetScale(const sf::Vector2f &newScale, std::size_t i = 0) ;
    void monsterSetPosition(const sf::Vector2f &newPos, std::size_t i = 0) ;
    void heroSetScale(const sf::Vector2f &newScale, std::size_t i = 0) ;

    void event(sf::Event &newEvent,
               sf::RenderWindow &window,
               const sf::Time &currentTime) ;

    void updateStates(const sf::Time &currentTime) ;

    void draw(sf::RenderWindow &window) ;

    [[nodiscard]] std::vector<std::vector<Cell>> &getMap() ;

    const std::list<FreeObject>& getfreeObjects() const ;

    const std::vector<std::shared_ptr<TemplateCharacter>> &getHeroes() const ;
    const std::vector<std::shared_ptr<TemplateCharacter>> &getMonsters() const ;

private:
    std::vector<std::vector<Cell>> map;

    std::vector<std::shared_ptr<TemplateCharacter>> heroes;
    std::vector<std::shared_ptr<TemplateCharacter>> monsters;
    std::list<FreeObject> freeObjects;
};
}  // namespace jam