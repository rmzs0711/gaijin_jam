#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "Cell.h"
#include "characters.h"
#include "usefulFunctions.h"

namespace jam {

struct Level {
    explicit Level(const std::vector<std::vector<CellObject>> &mapObjects)
        : hero(
              "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
              "PurpleMelee/AssasinPurple.png",
              map,
              true) {
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
    void characterSetPosition(const sf::Vector2f &newPos) {
        hero.setPosition(newPos);
    }
    void characterSetScale(const sf::Vector2f &newScale) {
        hero.setScale(newScale);
    }

    void event(sf::Event &newEvent,
               sf::RenderWindow &window,
               const sf::Time &currentTime) {
        hero.event(newEvent, window, currentTime);
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
        hero.drawCharacter(window);
    }

    [[nodiscard]] const std::vector<std::vector<Cell>> &getMap() const {
        return map;
    }

private:
    std::vector<std::vector<Cell>> map;
    CharacterMouse hero;
};
}  // namespace jam