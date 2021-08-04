#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "usefulFunctions.h"

namespace jam {
enum CellState { NORMAL, ON_FIRE, AFTER_FIRE, NUMBER_OF_STATES };

enum CellObject {
    EMPTY,
    LIGHT_GREEN_GRASS,
    DARK_GREEN_GRASS,
    DEAD_GRASS,
    //                  BUILD_TABLE, FIRE
    NUMBER_OF_OBJECTS
};

const sf::Vector2i assetCellSize = {16, 16};
const std::vector<std::tuple<CellObject, sf::Vector2i, std::string>> assetInfo =
    {
        std::make_tuple(EMPTY,
                        sf::Vector2i(0, 0),
                        "data/images/black"
                        ".png"),
        std::make_tuple(LIGHT_GREEN_GRASS,
                        sf::Vector2i(16, 0),
                        "data/images/MiniWorldSprites/Ground/Grass.png"),
        std::make_tuple(DARK_GREEN_GRASS,
                        sf::Vector2i(32, 0),
                        "data/images/MiniWorldSprites/Ground/Grass.png"),
        std::make_tuple(DEAD_GRASS,
                        sf::Vector2i(48, 0),
                        "data/images/MiniWorldSprites/Ground/Grass.png"),
};

inline const size_t cellSize = 64;
std::vector<sf::Texture *> texturePtrs;
std::vector<sf::Time> stateDurations;

struct Cell {
public:
    explicit Cell() {
        rect.setSize(sf::Vector2f(cellSize, cellSize));
    }

    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time::Zero) {
        stateStartTime = newStateStartTime;
        switch (state) {
            case NORMAL:
                rect.setFillColor(sf::Color::White);
                break;
            case ON_FIRE:
                rect.setFillColor(sf::Color::Red);
                break;
            case AFTER_FIRE:
                rect.setFillColor(sf::Color::White);
                break;
            default:
                break;
        }
    }
    CellObject getObject() const {
        return object;
    }
    sf::FloatRect getGlobalBounds() {
        return rect.getGlobalBounds();
    }

    void setPosition(const sf::Vector2f &position) {
        rect.setPosition(position);
    }

    void setObject(const CellObject &newObject) {
        object = newObject;
        rect.setTexture(texturePtrs[object]);
    }
    void updateState(const sf::Time &currentTime) {
        if (stateDurations[state] < currentTime - stateStartTime) {
            switch (state) {
                case NORMAL:
                    break;
                case ON_FIRE:
                    setState(CellState::AFTER_FIRE);
                    break;
                case AFTER_FIRE:
                    setState(CellState::NORMAL);
                    break;
                default:
                    break;
            }
        }
    }
    void draw(sf::RenderWindow &window) {
        window.draw(rect);
    }

private:
    sf::RectangleShape rect;
    CellState state = NORMAL;
    CellObject object = EMPTY;
    sf::Time stateStartTime;
    //    const size_t cellSize = 16;
};

struct Level {
    explicit Level(const std::vector<std::vector<CellObject>> &mapObjects) {
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
    void draw(sf::RenderWindow &window) {
        for (auto &i : map) {
            for (auto &j : i) {
                j.draw(window);
            }
        }
    }
    [[nodiscard]] const std::vector<std::vector<Cell>> &getMap() const {
        return map;
    }

private:
    std::vector<std::vector<Cell>> map;
};
}  // namespace jam