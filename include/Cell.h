#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>
#include <tuple>
#include <variant>
#include <vector>
#include "usefulFunctions.h"
#ifdef _MSC_VER
const double M_PI = 3.14;
#endif

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
    ROAD,
    NUMBER_OF_BACKGROUNDS
};
inline int NUMBER_OF_TEXTURES = NUMBER_OF_BACKGROUNDS;

inline const sf::Vector2i assetCellSize = {16, 16};
inline float fireDamage = 0.1;
inline float earthShakeDamage = 0.05;
inline const std::vector<std::tuple<int, sf::IntRect, std::string>> assetInfo =
    {
        std::make_tuple(NONE,
                        sf::IntRect(sf::Vector2i(0, 0), assetCellSize),
                        "data/images/black"
                        ".png"),
        std::make_tuple(ROAD,
                        sf::IntRect({assetCellSize.x * 5, 0},
                                    {assetCellSize.x, assetCellSize.x}),
                        "data/images/MiniWorldSprites/Ground/Winter.png"),
        std::make_tuple(
            LIGHT_GREEN_GRASS,
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
            sf::IntRect({16, 0}, {assetCellSize.x * 3, assetCellSize.y}),
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
        std::make_tuple(
            FROZEN_DEAD_TREE,
            sf::IntRect({16, 0}, {assetCellSize.x * 3, assetCellSize.y}),
            "data/images/MiniWorldSprites/Nature"
            "/WinterDeadTrees.png"),
        std::make_tuple(
            ROCK,
            sf::IntRect({0, 0}, {assetCellSize.x * 3, assetCellSize.y}),
            "data/images/MiniWorldSprites/Nature/Rocks.png"),
        std::make_tuple(
            FROZEN_ROCK,
            sf::IntRect({0, 48}, {assetCellSize.x * 3, assetCellSize.y}),
            "data/images/MiniWorldSprites/Nature/Rocks.png"),
        std::make_tuple(
            FIRE,
            sf::IntRect({0, 0}, {assetCellSize.x * 4, assetCellSize.x}),
            "data/images/fire.png"),
};

inline std::vector<sf::Vector2i> texturesNumberOfFrames;

inline const int cellSize = 128;
inline std::vector<sf::Texture *> texturePtrs;
inline std::map<CellState, sf::Time> stateDurations = {
    {BLAST, sf::seconds(5)},        {LAVA, sf::seconds(10)},
    {EARTHSHAKE, sf::seconds(3)},   {WALL, sf::seconds(5)},
    {FROZEN_BLAST, sf::seconds(6)}, {CLOUD, sf::seconds(10)},
    {NORMAL, sf::seconds(0)}};


inline sf::Vector2f toPosition(sf::Vector2i map_position) {
    sf::Vector2f pos = (sf::Vector2f)map_position;
    pos = {pos.x * cellSize + cellSize / 2, pos.y * cellSize + cellSize / 2};
    return pos;
}

inline sf::Vector2i toMapPosition(sf::RenderTarget &window,
                                  sf::Vector2f position) {
    int x = window.mapCoordsToPixel(position).x;
    int y = window.mapCoordsToPixel(position).y;
    x = ((x - (x % cellSize)) / cellSize);
    y = ((y - (y % cellSize)) / cellSize);
    return sf::Vector2i(x, y);
}

struct Cell {
public:
    void setBackgroundType(int newBackgroundType);
    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time());
    sf::FloatRect getGlobalBounds() const;

    void setPosInMap(const sf::Vector2i &position);

    void updateState(const sf::Time &currentTime);
    const sf::Time &getStateStartTime() const;
    CellState getState() const;
    const sf::Sprite &getBackground() const;
    int getBackgroundType() const;
    int getPrevBackground() const;
    sf::Vector2i getCurrentFrame() const;
    void setCurrentFrame(sf::Vector2i newCurrentFrame);
    const sf::Vector2i &getNumberOfFrames() const;
    void setNumberOfFrames(const sf::Vector2i &newNumberOfFrames);
    void draw(sf::RenderTarget &window) const;

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
    explicit FreeObject(const Object &newObjectType);
    bool operator<(const FreeObject &rhs) const;
    const Object &getObjectType() const;
    void setScale(const sf::Vector2f &newScale);
    sf::FloatRect getGlobalBounds() const;
    void setPosition(const sf::Vector2f &newPos);
    const sf::Vector2f &getPosition() const;
    void setOrigin(const sf::Vector2f &newOrigin);
    const sf::Sprite &getSprite() const;
    void updateAnimation() const;
    void changeObjectType(const Object &newObjectType) const;
    void draw(sf::RenderTarget &window) const;
    const sf::FloatRect &getHitBox() const;
    void setHitBox(const sf::FloatRect &newHitBox);
    const sf::Vector2i &getCurrentFrame() const;
    void setCurrentFrame(const sf::Vector2i &newCurrentFrame) const;
    const sf::Vector2i &getNumberOfFrames() const;
    void setNumberOfFrames(const sf::Vector2i &newNumberOfFrames);
    void setAnimation(bool condition);

private:
    mutable sf::Sprite object;
    mutable Object objectType = NONE;
    bool isAnime = false;
    mutable sf::Vector2i currentFrame;
    mutable sf::Vector2i numberOfFrames;
    sf::FloatRect hitBox = {0, 0, 0, 0};
};

inline bool FreeObject::operator<(const FreeObject &rhs) const {
    if (object.getPosition().y < rhs.object.getPosition().y) {
        return true;
    }
    if (object.getPosition().y == rhs.object.getPosition().y) {
        return object.getPosition().x < rhs.object.getPosition().x;
    }
    return false;
}
}  // namespace jam