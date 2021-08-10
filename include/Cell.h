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
    EARTHSHAKE,
    WALL,
    NUMBER_OF_STATES
};

enum Object {
    NONE = 0,

    TREE,
    DEAD_TREE,
    FROZEN_TREE,
    FROZEN_DEAD_TREE,
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
inline int NUMBER_OF_TEXTURES = NUMBER_OF_BACKGROUNDS;

inline const sf::Vector2i assetCellSize = {16, 16};
inline float fireDamage = 0.1;
inline float earthShakeDamage = 0.05;
inline const std::vector<std::tuple<int, sf::IntRect, std::string>> assetInfo
    = {
    std::make_tuple(NONE,
                    sf::IntRect(sf::Vector2i(0, 0), assetCellSize),
                    "data/images/black"
                    ".png"),
    std::make_tuple(LIGHT_GREEN_GRASS,
                    sf::IntRect({0, 0}, {assetCellSize.x * 3, assetCellSize.y}),
                    "data/images/MiniWorldSprites/Ground/TexturedGrass.png"),
    std::make_tuple(
        DARK_GREEN_GRASS,
        sf::IntRect({0, 16}, {assetCellSize.x * 3, assetCellSize.y}),
        "data/images/MiniWorldSprites/Ground/TexturedGrass.png"),
    std::make_tuple(
        DEAD_GRASS,
        sf::IntRect({48, 0}, {assetCellSize.x * 2, assetCellSize.y}),
        "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(LAVA_FLOOR,
                    sf::IntRect({0, 0}, assetCellSize),
                    "data/images/lava.png"),
    std::make_tuple(
        TREE,
        sf::IntRect({16, 0}, {assetCellSize.x * 3, assetCellSize.y}),
        "data/images/MiniWorldSprites/Nature/Trees.png"),
    std::make_tuple(
        DEAD_TREE,
        sf::IntRect({32, 0}, {assetCellSize.x * 2, assetCellSize.y}),
        "data/images/MiniWorldSprites/Nature/DeadTrees.png"),
    std::make_tuple(
        FROZEN_TREE,
        sf::IntRect({16, 48}, {assetCellSize.x * 3, assetCellSize.y}),
        "data/images/MiniWorldSprites/Nature/WinterTrees.png"),
    std::make_tuple(FROZEN_GRASS,
                    sf::IntRect({0, 0}, assetCellSize),
                    "data/images/MiniWorldSprites/Ground/Grass.png"),
    std::make_tuple(BUILD_SIGN,
                    sf::IntRect({0, 0}, assetCellSize),
                    "data/images/MiniWorldSprites/Miscellaneous/Signs.png"),
    std::make_tuple(FROZEN_DEAD_TREE,
                    sf::IntRect({0, 0}, {assetCellSize.x * 4, assetCellSize.y}),
                    "data/images/MiniWorldSprites/Nature"
                    "/WinterDeadTrees.png"),
    std::make_tuple(ROCK,
                    sf::IntRect({0, 0}, {assetCellSize.x * 3, assetCellSize.y}),
                    "data/images/MiniWorldSprites/Nature/Rocks.png"),
    std::make_tuple(
        FROZEN_ROCK,
        sf::IntRect({0, 48}, {assetCellSize.x * 3, assetCellSize.y}),
        "data/images/MiniWorldSprites/Nature/Rocks.png"),
    std::make_tuple(FIRE,
                    sf::IntRect({0, 0}, {assetCellSize.x * 4, assetCellSize.x}),
                    "data/images/fire.png"),
};

inline std::vector<sf::Vector2i> texturesNumberOfFrames;

inline const int cellSize = 128;
inline std::vector<sf::Texture *> texturePtrs;
inline std::vector<sf::Time> stateDurations;

struct Cell {
public:
    void setBackgroundType(int newBackgroundType) {
        Cell::backgroundType = newBackgroundType;
        prevBackground = backgroundType;
        setState(NORMAL);
    }
    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time()) {
        stateStartTime = newStateStartTime;
        switch (stateType) {
            case NORMAL:
                break;

            case WALL:
            case LAVA:
            case BLAST:
                prevBackground = DEAD_GRASS;
                break;
            case EARTHSHAKE:
            case FROZEN_BLAST:
                backgroundType = prevBackground;
                break;
            case CLOUD:
                background.setColor(sf::Color::White);
                break;
            case NUMBER_OF_STATES:
                assert(0);
                break;
        }
        switch (newState) {
            case NORMAL:
                background.setTexture(*texturePtrs[prevBackground]);
                backgroundType = prevBackground;
                background.setColor(sf::Color::White);
                break;
            case LAVA:
                background.setTexture(*texturePtrs[LAVA_FLOOR]);
                backgroundType = LAVA_FLOOR;
                break;
            case FROZEN_BLAST:
                prevBackground = backgroundType;
                background.setTexture(*texturePtrs[FROZEN_GRASS]);
                backgroundType = FROZEN_GRASS;
                break;
            case BLAST:
                prevBackground = DEAD_GRASS;
                background.setTexture(*texturePtrs[DEAD_GRASS]);
                backgroundType = DEAD_GRASS;
                break;
            case CLOUD:
                background.setTexture(*texturePtrs[prevBackground]);
                background.setColor(sf::Color(100, 100, 100, 100));
                backgroundType = prevBackground;
                break;
            case EARTHSHAKE:
            case WALL:
                break;
            case NUMBER_OF_STATES:
                assert(0);
                break;
        }
        background.setScale(sf::Vector2f(cellSize, cellSize) /
                            (float)assetCellSize.x);
        setNumberOfFrames(texturesNumberOfFrames[backgroundType]);
        setCurrentFrame({rand() % numberOfFrames.x, rand() % numberOfFrames.y});
        stateType = newState;
    }
    sf::FloatRect getGlobalBounds() const {
        return background.getGlobalBounds();
    }
    void draw(sf::RenderWindow& window) {
        window.draw(background);
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
            case EARTHSHAKE:
            case WALL:
            case FROZEN_BLAST:
                background.setColor(
                    sf::Color(brightness, brightness, brightness));
                break;
            case CLOUD:
                background.setColor(
                    sf::Color(100, 100, brightness, 80));
                break;
            case NORMAL:
            case NUMBER_OF_STATES:
                assert(0);
                break;
        }
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
    int getBackgroundType() const {
        return backgroundType;
    }
    int getPrevBackground() const {
        return prevBackground;
    }
    sf::Vector2i getCurrentFrame() const {
        return currentFrame;
    }
    void setCurrentFrame(sf::Vector2i newCurrentFrame) {
        Cell::currentFrame = newCurrentFrame;
        background.setTextureRect(
            {newCurrentFrame * assetCellSize.x, assetCellSize});
    }
    const sf::Vector2i &getNumberOfFrames() const {
        return numberOfFrames;
    }
    void setNumberOfFrames(const sf::Vector2i &newNumberOfFrames) {
        Cell::numberOfFrames = newNumberOfFrames;
    }

private:
    sf::Time stateStartTime;
    CellState stateType = NORMAL;

    sf::Sprite background;
    sf::Vector2i currentFrame;
    sf::Vector2i numberOfFrames;
    int backgroundType = LIGHT_GREEN_GRASS;
    int prevBackground = LIGHT_GREEN_GRASS;
};

struct FreeObject {
    const Object &getObjectType() const {
        return objectType;
    }
    void setScale(const sf::Vector2f &newScale) {
        object.setScale(newScale);
    }
    explicit FreeObject(const Object &newObjectType) {
        objectType = newObjectType;
        object.setTexture(*texturePtrs[objectType]);
        setPosition({-1, -1});
        setNumberOfFrames(texturesNumberOfFrames[objectType]);
        object.setColor(sf::Color::White);
        setCurrentFrame({rand() % numberOfFrames.x, rand() % numberOfFrames.y});
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
    const sf::Sprite &getSprite() const {
        return object;
    }
    void draw(sf::RenderWindow &window) {
        if (isAnime) {
            setCurrentFrame({rand() % numberOfFrames.x, rand() %
                                                            numberOfFrames.y});
        }
        sf::RectangleShape rect;
        rect.setPosition(hitBox.left, hitBox.top);
        rect.setSize({hitBox.width, hitBox.height});
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineThickness(2);
        rect.setOutlineColor(sf::Color::Red);
        window.draw(object);
        window.draw(rect);
        rect.setPosition(object.getGlobalBounds().left,
                         object.getGlobalBounds().top);
        rect.setSize(
            {object.getGlobalBounds().width, object.getGlobalBounds().height});
        window.draw(rect);
    }
    const sf::FloatRect &getHitBox() const {
        return hitBox;
    }
    void setHitBox(const sf::FloatRect &newHitBox) {
        FreeObject::hitBox = newHitBox;
    }
    const sf::Vector2i &getCurrentFrame() const {
        return currentFrame;
    }
    void setCurrentFrame(const sf::Vector2i &newCurrentFrame) {
        FreeObject::currentFrame = newCurrentFrame;
        object.setTextureRect(
            {{currentFrame * assetCellSize.x}, assetCellSize});
    }
    const sf::Vector2i &getNumberOfFrames() const {
        return numberOfFrames;
    }
    void setNumberOfFrames(const sf::Vector2i &newNumberOfFrames) {
        FreeObject::numberOfFrames = newNumberOfFrames;
    }
    void setAnimation(bool condition) {
        isAnime = condition;
    }
private:
    sf::Sprite object;
    Object objectType = NONE;
    bool isAnime = false;
    sf::Vector2i currentFrame;
    sf::Vector2i numberOfFrames;
    sf::FloatRect hitBox = {0, 0, 0, 0};
};
}  // namespace jam