#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <tuple>
#include <variant>
#include <vector>
#include "usefulFunctions.h"

namespace jam {
enum CellState {
    NORMAL,
    LAVA,
    BLAST,
    FROZEN_BLAST,
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
    LAVA_OBJECT,
    TREE_1,
    TREE_2,
    TREE_3,
    DEAD_TREE,
    FROZEN_TREE_1,
    FROZEN_TREE_2,
    FROZEN_TREE_3,
    FROZEN_DEAD_TREE,
    FROZEN_GRASS,
    STUMP,
    BUILD_SIGN,
    FIRE_1,
    FIRE_2,
    FIRE_3,
    FIRE_4,
    SMOKE,
    ROCK,
    FROZEN_ROCK,
    NUMBER_OF_OBJECTS
};
sf::Texture fire[6];

const sf::Vector2i assetCellSize = {16, 16};
const std::vector<std::tuple<int, sf::Vector2i, std::string>> assetInfo = {
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
    std::make_tuple(LAVA_OBJECT, sf::Vector2i(0, 0), "data/images/lava.png"),
    std::make_tuple(TREE_1,
                    sf::Vector2i(16, 0),
                    "data/images/MiniWorldSprites/Nature/Trees.png"),
    std::make_tuple(TREE_2,
                    sf::Vector2i(32, 0),
                    "data/images/MiniWorldSprites/Nature/Trees.png"),
    std::make_tuple(TREE_3,
                    sf::Vector2i(48, 0),
                    "data/images/MiniWorldSprites/Nature/Trees.png"),
    std::make_tuple(DEAD_TREE,
                    sf::Vector2i(32, 0),
                    "data/images/MiniWorldSprites/Nature/DeadTrees.png"),
    std::make_tuple(FROZEN_TREE_1,
                    sf::Vector2i(16, 48),
                    "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_TREE_2,
                    sf::Vector2i(32, 48),
                    "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_TREE_3,
                    sf::Vector2i(48, 48),
                    "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_GRASS,
                    sf::Vector2i(0, 0),
                    "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(STUMP,
                    sf::Vector2i(0, 0),
                    "data/images/MiniWorldSprites/Nature/Trees.png"),
    std::make_tuple(BUILD_SIGN,
                    sf::Vector2i(0, 0),
                    "data/images/MiniWorldSprites/Miscellaneous/Signs.png"),
    std::make_tuple(FROZEN_DEAD_TREE,
                    sf::Vector2i(32, 0),
                    "data/images/MiniWorldSprites/Nature"
                    "/WinterDeadTrees.png"),
    std::make_tuple(FIRE_1, sf::Vector2i(0, 0), "data/images/fire.png"),
    std::make_tuple(FIRE_2, sf::Vector2i(16, 0), "data/images/fire.png"),
    std::make_tuple(FIRE_3, sf::Vector2i(32, 0), "data/images/fire.png"),
    std::make_tuple(FIRE_4, sf::Vector2i(48, 0), "data/images/fire.png"),
    std::make_tuple(SMOKE,
                    sf::Vector2i(80, 0),
                    "data/images/MiniWorldSprites/Ground/Winter.png"),
    std::make_tuple(ROCK,
                    sf::Vector2i(32, 0),
                    "data/images/MiniWorldSprites/Nature/Rocks.png"),
    std::make_tuple(FROZEN_ROCK,
                    sf::Vector2i(32, 48),
                    "data/images/MiniWorldSprites/Nature/Rocks.png"),
};

const size_t cellSize = 64;
std::vector<sf::Texture *> texturePtrs;
std::vector<sf::Time> stateDurations;

struct Cell {
public:
    Cell() {
        backRect.setSize(sf::Vector2f(cellSize, cellSize));
        frontRect.setSize(backRect.getSize());
    }

    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time()) {
        stateStartTime = newStateStartTime;
        if (object == EMPTY) {
            return;
        }
        state = newState;
        switch (state) {
            case NORMAL:
                backRect.setFillColor(sf::Color::White);
                frontRect.setFillColor(sf::Color::White);
                switch (object) {
                    case STUMP:
                    case BUILD_SIGN:
                        prevObject = object;
                    case TREE_1:
                    case TREE_2:
                    case TREE_3:
                    case DEAD_TREE:
                        setObject(rand() % 2 ? LIGHT_GREEN_GRASS
                                             : DARK_GREEN_GRASS);
                        break;
                    default:
                        break;
                }
                setObject(prevObject);
                break;
            case LAVA:
                prevObject = DEAD_GRASS;
                setObject(LAVA_OBJECT);
                break;
            case FROZEN_BLAST:
                switch (object) {
                    case FROZEN_ROCK:
                    case ROCK:
                        prevObject = DEAD_GRASS;
                        setObject(FROZEN_ROCK);
                        break;
                    case SMOKE:
                        setObject(FROZEN_GRASS);
                        prevObject = object;
                        break;
                    case EMPTY:
                        break;
                    case FIRE_1:
                    case FIRE_2:
                    case FIRE_3:
                    case FIRE_4:
                    case LAVA_OBJECT:
                        setObject(FROZEN_GRASS);
                        prevObject =
                            rand() % 2 ? LIGHT_GREEN_GRASS : DARK_GREEN_GRASS;
                        break;
                    case LIGHT_GREEN_GRASS:
                    case DARK_GREEN_GRASS:
                    case DEAD_GRASS:
                        prevObject = object;
                        setObject(FROZEN_GRASS);
                        break;
                    case TREE_1:
                        prevObject = object;
                        setObject(FROZEN_TREE_1);
                        break;
                    case TREE_2:
                        prevObject = object;
                        setObject(FROZEN_TREE_2);
                        break;
                    case TREE_3:
                        prevObject = object;
                        setObject(FROZEN_TREE_3);
                        break;
                    case DEAD_TREE:
                        prevObject = object;
                        setObject(FROZEN_DEAD_TREE);
                        break;
                    case FROZEN_TREE_1:
                    case FROZEN_TREE_2:
                    case FROZEN_TREE_3:
                    case FROZEN_DEAD_TREE:
                        setObject(FROZEN_DEAD_TREE);
                        prevObject = DEAD_TREE;
                        break;
                    case FROZEN_GRASS:
                        prevObject = DEAD_GRASS;
                        break;
                    case BUILD_SIGN:
                    case STUMP:
                        prevObject = object;
                        setObject(FROZEN_GRASS);
                        break;
                    case NUMBER_OF_OBJECTS:
                        break;
                }
                break;
            case BLAST:
                prevObject = DEAD_GRASS;
                break;
            case CLOUD:
                prevObject = object;
                if (prevObject == ROCK) {
                    prevObject = DEAD_GRASS;
                }
                if (prevObject == CellObject::FIRE_1 ||
                    prevObject == CellObject::FIRE_2 ||
                    prevObject == CellObject::FIRE_3 ||
                    prevObject == CellObject::FIRE_4) {
                    setObject(DEAD_GRASS);
                    prevObject = DEAD_GRASS;
                }
                backRect.setTexture(texturePtrs[SMOKE]);
                break;
            case BIG_WALL:
                setObject(ROCK);
                prevObject = DEAD_GRASS;
                break;
            case FROZEN_WALL:
                setObject(FROZEN_ROCK);
                prevObject = DEAD_GRASS;
                break;
            case NUMBER_OF_STATES:
            default:
                assert(0);
                break;
        }
    }
    int getObject() const {
        return object;
    }
    sf::FloatRect getGlobalBounds() const {
        return backRect.getGlobalBounds();
    }

    void setPosition(const sf::Vector2f &position) {
        backRect.setPosition(position);
        frontRect.setPosition(position);
    }

    void setObject(const int &newObject) {
        object = newObject;
        switch (object) {
            case EMPTY:
            case LIGHT_GREEN_GRASS:
            case DARK_GREEN_GRASS:
            case DEAD_GRASS:
            case LAVA_OBJECT:
            case FROZEN_GRASS:
            case STUMP:
            case BUILD_SIGN:
                break;
            case TREE_1:
            case TREE_2:
            case TREE_3:
                backRect.setTexture(texturePtrs[DARK_GREEN_GRASS]);
                break;
            case FIRE_1:
            case FIRE_2:
            case FIRE_3:
            case FIRE_4:
            case DEAD_TREE:
                backRect.setTexture(texturePtrs[DEAD_GRASS]);
                break;
            case NUMBER_OF_OBJECTS:
                assert(0);
                break;
            case FROZEN_DEAD_TREE:
            case FROZEN_TREE_1:
            case FROZEN_TREE_2:
            case FROZEN_TREE_3:
                backRect.setTexture(texturePtrs[FROZEN_GRASS]);
                break;
            case SMOKE:
                break;
            case ROCK:
                backRect.setTexture(texturePtrs[DEAD_GRASS]);
                break;
            case FROZEN_ROCK:
                backRect.setTexture(texturePtrs[FROZEN_GRASS]);
                break;
        }
        frontRect.setTexture(texturePtrs[object]);
    }

    void updateState(const sf::Time &currentTime) {
        if (stateDurations[state] < currentTime - stateStartTime) {
            switch (state) {
                case NUMBER_OF_STATES:
                    assert(0);
                case NORMAL:
                    break;
                case BLAST:
                case FROZEN_BLAST:
                case LAVA:
                case CLOUD:
                case BIG_WALL:
                case FROZEN_WALL:
                    setState(NORMAL);
                    break;
            }
            return;
        }
        static int brightDirection = 1;
        brightDirection ^= 1;
        float coef = (currentTime - stateStartTime) / stateDurations[state];

        auto ans = (float)std::sin(10 * M_PI * std::log(1 - coef));

        unsigned char brightness = 200 + static_cast<unsigned char>(ans * 50);
        switch (state) {
            case NORMAL:
                break;
            case BLAST:
                setObject(FIRE_1 + rand() % 4);
                break;
            case LAVA:
            case FROZEN_BLAST:
                frontRect.setFillColor(
                    sf::Color(brightness, brightness, brightness));
                backRect.setFillColor(frontRect.getFillColor());
                break;
            case CLOUD:
                frontRect.setFillColor(sf::Color(255, 255, 255, 80));
                break;
            case BIG_WALL:
                break;
            case FROZEN_WALL:
                break;
            case NUMBER_OF_STATES:
                break;
        }
    }
    void draw(sf::RenderWindow &window) {
        window.draw(backRect);
        window.draw(frontRect);
    }

private:
    sf::RectangleShape backRect;
    sf::RectangleShape frontRect;
    CellState state = NORMAL;
    int object = EMPTY;
    sf::Time stateStartTime;
    int prevObject = EMPTY;
};

}  // namespace jam