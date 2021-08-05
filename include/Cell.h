#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "usefulFunctions.h"

namespace jam {
enum CellState {
    NORMAL,
    LAVA,
    AFTER_LAVA,
    BLAST,
    AFTER_BLAST,
    FROZEN_BLAST,
    AFTER_FROZEN_BLAST,
    CLOUD,
    BIG_WALL,
    FROZEN_WALL,
    NUMBER_OF_STATES
};

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

const size_t cellSize = 64;
std::vector<sf::Texture *> texturePtrs;
std::vector<sf::Time> stateDurations;

struct Cell {
public:
    Cell() {
        rect.setSize(sf::Vector2f(cellSize, cellSize));
    }

    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time()) {
        stateStartTime = newStateStartTime;
        state = newState;
        switch (state) {
            case NORMAL:
                rect.setFillColor(sf::Color::White);
                break;
            case LAVA:
                rect.setFillColor(sf::Color::Red);
                break;
            case AFTER_LAVA:
                rect.setFillColor(sf::Color::White);
                break;
            case BLAST:
                rect.setFillColor(sf::Color::Blue);
                break;
            case AFTER_BLAST:
            case FROZEN_BLAST:
            case AFTER_FROZEN_BLAST:
            case CLOUD:
            case BIG_WALL:
            case FROZEN_WALL:
                rect.setFillColor(sf::Color::Black);
                break;
            case NUMBER_OF_STATES:
                assert(0);
                break;
        }
    }
    CellObject getObject() const {
        return object;
    }
    sf::FloatRect getGlobalBounds() const {
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
                case NUMBER_OF_STATES:
                    assert(0);
                case NORMAL:
                    break;
                case LAVA:
                    setState(CellState::AFTER_LAVA);
                    break;
                case BLAST:
                    setState(AFTER_BLAST);
                    break;
                case FROZEN_BLAST:
                    setState(AFTER_FROZEN_BLAST);
                    break;
                case CLOUD:
                case BIG_WALL:
                case AFTER_LAVA:
                case AFTER_FROZEN_BLAST:
                case FROZEN_WALL:
                case AFTER_BLAST:
                    setState(NORMAL);
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
};

}  // namespace jam