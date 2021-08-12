#include "Cell.h"

namespace jam {

void Cell::setBackgroundType(int newBackgroundType) {
    Cell::backgroundType = newBackgroundType;
    prevBackground = backgroundType;
    setState(NORMAL);
}
void Cell::setState(const CellState &newState,
                    const sf::Time &newStateStartTime) {
    stateStartTime = newStateStartTime;
    switch (stateType) {
        case NORMAL:
            break;
        case WALL:
        case LAVA:
        case BLAST:
            prevBackground = prevBackground == ROAD ? ROAD : DEAD_GRASS;
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
            if (prevBackground == ROAD || backgroundType == ROAD) {
                prevBackground = ROAD;
            }
            backgroundType = LAVA_FLOOR;
            break;
        case FROZEN_BLAST:
            switch (stateType) {
                case LAVA:
                case BLAST:
                    prevBackground =
                        prevBackground == ROAD ? ROAD :
                        (rand() % 2 ? DARK_GREEN_GRASS : LIGHT_GREEN_GRASS);
                    break;
                default:
                    prevBackground = backgroundType;
                    break;
            }
            background.setTexture(*texturePtrs[FROZEN_GRASS]);
            backgroundType = FROZEN_GRASS;
            break;
        case BLAST:
            if (prevBackground == ROAD) {
                break;
            }
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
sf::FloatRect Cell::getGlobalBounds() const {
    return background.getGlobalBounds();
}
void Cell::draw(sf::RenderWindow &window) {
    window.draw(background);
}

void Cell::setPosInMap(const sf::Vector2i &position) {
    background.setPosition(sf::Vector2f(position * cellSize));
}

void Cell::updateState(const sf::Time &currentTime) {
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
            background.setColor(sf::Color(brightness, brightness, brightness));
            break;
        case CLOUD:
            background.setColor(sf::Color(100, 100, brightness, 80));
            break;
        case NORMAL:
        case NUMBER_OF_STATES:
            assert(0);
            break;
    }
}
const sf::Time &Cell::getStateStartTime() const {
    return stateStartTime;
}
CellState Cell::getState() const {
    return stateType;
}
const sf::Sprite &Cell::getBackground() const {
    return background;
}
int Cell::getBackgroundType() const {
    return backgroundType;
}
int Cell::getPrevBackground() const {
    return prevBackground;
}
sf::Vector2i Cell::getCurrentFrame() const {
    return currentFrame;
}
void Cell::setCurrentFrame(sf::Vector2i newCurrentFrame) {
    Cell::currentFrame = newCurrentFrame;
    background.setTextureRect(
        {newCurrentFrame * assetCellSize.x, assetCellSize});
}
const sf::Vector2i &Cell::getNumberOfFrames() const {
    return numberOfFrames;
}
void Cell::setNumberOfFrames(const sf::Vector2i &newNumberOfFrames) {
    Cell::numberOfFrames = newNumberOfFrames;
}

const Object &FreeObject::getObjectType() const {
    return objectType;
}
void FreeObject::setScale(const sf::Vector2f &newScale) {
    object.setScale(newScale);
}
FreeObject::FreeObject(const Object &newObjectType)
    : objectType(newObjectType) {
    object.setTexture(*texturePtrs[objectType]);
    setPosition({-1, -1});
    setNumberOfFrames(texturesNumberOfFrames[objectType]);
    object.setColor(sf::Color::White);
    setCurrentFrame({rand() % numberOfFrames.x, rand() % numberOfFrames.y});
}
void FreeObject::setPosition(const sf::Vector2f &newPos) {
    object.setPosition(newPos);
}
const sf::Vector2f &FreeObject::getPosition() const {
    return object.getPosition();
}
void FreeObject::setOrigin(const sf::Vector2f &newOrigin) {
    object.setOrigin(newOrigin);
}
const sf::Sprite &FreeObject::getSprite() const {
    return object;
}
void FreeObject::updateAnimation() const {
    if (isAnime) {
        setCurrentFrame({rand() % numberOfFrames.x, rand() % numberOfFrames.y});
    }
}
void FreeObject::changeObjectType(const Object &newObjectType) const {
    object.setTexture(*texturePtrs[newObjectType]);
    objectType = newObjectType;
}
void FreeObject::draw(sf::RenderWindow &window) const {
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
const sf::FloatRect &FreeObject::getHitBox() const {
    return hitBox;
}
void FreeObject::setHitBox(const sf::FloatRect &newHitBox) {
    FreeObject::hitBox = newHitBox;
}
const sf::Vector2i &FreeObject::getCurrentFrame() const {
    return currentFrame;
}
void FreeObject::setCurrentFrame(const sf::Vector2i &newCurrentFrame) const {
    FreeObject::currentFrame = newCurrentFrame;
    object.setTextureRect({{currentFrame * assetCellSize.x}, assetCellSize});
}
const sf::Vector2i &FreeObject::getNumberOfFrames() const {
    return numberOfFrames;
}
void FreeObject::setNumberOfFrames(const sf::Vector2i &newNumberOfFrames) {
    FreeObject::numberOfFrames = newNumberOfFrames;
}
void FreeObject::setAnimation(bool condition) {
    isAnime = condition;
}

}  // namespace jam