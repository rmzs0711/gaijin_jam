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
        heroes.push_back(std::make_unique<Hero>("data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png", 100, 0.2, monsters, map));
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
    void characterSetPosition(const sf::Vector2f &newPos, std::size_t i = 0) {
        (*heroes[i]).setPosition(newPos);
    }
    void characterSetScale(const sf::Vector2f &newScale, std::size_t i = 0) {
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
       // std::cout << heroes.size() << '\n';
        for (auto &i : heroes) {
            (*i).drawCharacter(window);
        }
        /*for (auto &i : monsters) {
            (*i).drawCharacter(window);
        }*/
    }

    [[nodiscard]] const std::vector<std::vector<Cell>> &getMap() const {
        return map;
    }

private:
    std::vector<std::vector<Cell>> map;
    std::vector<std::unique_ptr<Hero>> heroes;
    std::vector<std::unique_ptr<TemplateCharacter>> monsters;
};
}  // namespace jam