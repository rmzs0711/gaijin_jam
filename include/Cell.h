#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <tuple>
#include <variant>
#include <vector>
#include "usefulFunctions.h"

// const double M_PI = 3.14;

namespace jam {
struct Level;
enum CellState {
    NORMAL,
    LAVA,
    BLAST,
    FROZEN_BLAST,
    CLOUD,
    EASRTHSHAKE,
    NUMBER_OF_STATES
};

enum Object {
    NONE = 0,

    TREE_1,
    TREE_2,
    TREE_3,
    DEAD_TREE,
    FROZEN_TREE_1,
    FROZEN_TREE_2,
    FROZEN_TREE_3,
    FROZEN_DEAD_TREE,
    STUMP,
    BUILD_SIGN,
    FIRE,
    ROCK,
    FROZEN_ROCK,
    NUMBER_OF_OBJECTS
};

enum CellBackground {
    LIGHT_GREEN_GRASS = NUMBER_OF_OBJECTS,
    DARK_GREEN_GRASS,
    DEAD_GRASS,
    LAVA_FLOOR,
    FROZEN_GRASS,
    NUMBER_OF_BACKGROUNDS
};
int NUMBER_OF_TEXTURES;
sf::Texture fire[6];

const sf::Vector2i assetCellSize = {16, 16};
float fireDamage = 0.1;
const std::vector<std::tuple<int, std::string>> assetInfo = {
    std::make_tuple(NONE,
                    "data/images/black"
                    ".png"),
    std::make_tuple(LIGHT_GREEN_GRASS,

                    "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(DARK_GREEN_GRASS,
                    "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(DEAD_GRASS,
                    "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(DEAD_TREE,
                    "data/images/MiniWorldSprites/Nature/DeadTrees.png"),
    std::make_tuple(FROZEN_TREE_1,
                    "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_TREE_2,
                    "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_TREE_3,
                    "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_GRASS,
                    "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(STUMP, "data/images/MiniWorldSprites/Nature/Trees.png"),
    std::make_tuple(BUILD_SIGN,
                    "data/images/MiniWorldSprites/Miscellaneous/Signs.png"),
    std::make_tuple(FROZEN_DEAD_TREE,
                    "data/images/MiniWorldSprites/Nature"
                    "/WinterDeadTrees.png"),
    std::make_tuple(FIRE, "data/images/fire.png"),
    std::make_tuple(ROCK, "data/images/MiniWorldSprites/Nature/Rocks.png"),
    std::make_tuple(FROZEN_ROCK,
                    "data/images/MiniWorldSprites/Nature/Rocks.png"),
};

const int cellSize = 128;
std::vector<sf::Texture *> texturePtrs;
std::vector<sf::Time> stateDurations;

struct Cell {
public:
    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time()) {
        stateStartTime = newStateStartTime;
        background.setColor(sf::Color::White);
        switch (stateType) {
            case NORMAL:
                switch (stateType) {
                    case LAVA:
                    case BLAST:
                        background.setTexture(*texturePtrs[DEAD_GRASS]);
                        break;
                    case EASRTHSHAKE:
                    case NORMAL:
                    case FROZEN_BLAST:
                    case CLOUD:
                        background.setTexture(*texturePtrs[prevBackground]);
                        break;
                    case NUMBER_OF_STATES:
                        assert(0);
                        break;
                }
            case LAVA:
                background.setTexture(*texturePtrs[LAVA_FLOOR]);
                break;
            case FROZEN_BLAST:
                background.setTexture(*texturePtrs[FROZEN_GRASS]);
                break;
            case BLAST:
                background.setTexture(*texturePtrs[DEAD_GRASS]);
                break;
            case CLOUD:
                background.setColor(sf::Color(100, 100, 100, 100));
                break;
            case EASRTHSHAKE:
                break;
            case NUMBER_OF_STATES:
                assert(0);
                break;
        }
        prevBackground = backgroundType;
        stateType = newState;
    }
    sf::FloatRect getGlobalBounds() const {
        return background.getGlobalBounds();
    }

    void setPosInMap(const sf::Vector2i &position) {
        background.setPosition(sf::Vector2f(position * cellSize));
    }

    void updateState(const sf::Time &currentTime) {
        if (stateType == NORMAL) {
            return;
        }
        if (stateDurations[stateType] < currentTime - stateStartTime) {
            setState(NORMAL);
            return;
        }
        static int brightDirection = 1;
        brightDirection ^= 1;
        float coef = (currentTime - stateStartTime) / stateDurations[stateType];

        auto ans = std::sin(10 * M_PI * std::log(1 - coef));

        unsigned char brightness = 200 + static_cast<unsigned char>(ans * 50);
        switch (stateType) {
            case BLAST:
            case LAVA:
            case EASRTHSHAKE:
            case FROZEN_BLAST:
                background.setColor(
                    sf::Color(brightness, brightness, brightness));
                break;
            case CLOUD:
                background.setColor(
                    sf::Color(brightness, brightness, brightness, 80));
                break;
            case NORMAL:
            case NUMBER_OF_STATES:
                assert(0);
                break;
        }
    }
    void draw(sf::RenderWindow &window) {
        window.draw(background);
    }
    const sf::Time &getStateStartTime() const {
        return stateStartTime;
    }
    CellState getState() const {
        return stateType;
    }
    const sf::Sprite &getBackground() const {
        return background;
    }
    CellBackground getBackgroundType() const {
        return backgroundType;
    }
    CellBackground getPrevBackground() const {
        return prevBackground;
    }

private:
    sf::Time stateStartTime;
    CellState stateType = NORMAL;

    sf::Sprite background;
    CellBackground backgroundType = LIGHT_GREEN_GRASS;
    CellBackground prevBackground = LIGHT_GREEN_GRASS;

};

struct FreeObject {
    void setObject(const FreeObject &newObject) {
        auto pos = getPosition();
        *this = newObject;
        setPosition(pos);
    }
    void setTextureRect(const sf::IntRect &newRect) {
        object.setTextureRect(newRect);
    }
    const Object &getObjectType() const {
        return objectType;
    }
    void setTexture(const sf::Texture &texture) {
        object.setTexture(texture);
    }
    void setPosition(const sf::Vector2f &newPos) {
        object.setPosition(newPos);
    }
    const sf::Vector2f &getPosition() {
        return object.getPosition();
    }
    void setOrigin(const sf::Vector2f &newOrigin) {
        object.setOrigin(newOrigin);
    }

    void draw(sf::RenderWindow &window) {
        if (!framesRects.empty()) {
            object.setTextureRect(
                framesRects[++currentFrame % framesRects.size()]);
        }
        window.draw(object);
    }
    void setFramesRects(std::vector<sf::IntRect> newFramesRects) {
        FreeObject::framesRects = std::move(newFramesRects);
    }
    const sf::FloatRect &getHitBox() const {
        return hitBox;
    }
    void setHitBox(const sf::FloatRect &newHitBox) {
        FreeObject::hitBox = newHitBox;
    }

private:
    sf::Sprite object;
    Object objectType = NONE;
    int currentFrame = 0;
    std::vector<sf::IntRect> framesRects;
    sf::FloatRect hitBox = {0, 0, 0, 0};
};
}  // namespace jam