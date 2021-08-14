#ifdef _MSC_VER
#include "../include/building.h"
#include "../include/Level.h"
#else
#include "Level.h"
#include "building.h"
#endif

float jam::FlyingObject::getDamage() const {
    return damage;
}
const sf::Sprite &jam::FlyingObject::getObject() const {
    return object;
}
void jam::FlyingObject::setTexture(const sf::Texture &texture) {
    FlyingObject::object.setTexture(texture);
}
void jam::FlyingObject::setTextureRect(const sf::IntRect &rect) {
    FlyingObject::object.setTextureRect(rect);
}
float jam::FlyingObject::getSpeed() const {
    return speed;
}
void jam::FlyingObject::setSpeed(float newSpeed) {
    FlyingObject::speed = newSpeed;
}
void jam::FlyingObject::draw(sf::RenderTarget &window) const {
    window.draw(object);
}
bool jam::FlyingObject::isFinished() {
    object.setOrigin(getOrigin());
    object.setScale(getScale());
    auto &curPos = getPosition();
    targetPos = {targetPtr->getSprite()->getGlobalBounds().left +
                     targetPtr->getSprite()->getGlobalBounds().width / 2,
                 targetPtr->getSprite()->getGlobalBounds().top +
                     targetPtr->getSprite()->getGlobalBounds().height / 2};
    auto xDist = targetPos.x - curPos.x;
    auto yDist = targetPos.y - curPos.y;
    auto Dist = std::sqrt((xDist * xDist) + (yDist * yDist));

    move(speed * sf::Vector2f{xDist / Dist, yDist / Dist});
    object.setRotation(((float)asin(yDist / Dist) < 0 ? -1.f : 1.f) *
                           std::acos(xDist / Dist) * 180 / M_PI -
                       90);
    object.setPosition(getPosition());

    if (object.getGlobalBounds().contains(targetPos)) {
        targetPtr->health -= damage;
        return true;
    }
    return false;
}
void jam::FlyingObject::setTargetPtr(
    const std::shared_ptr<TemplateCharacter> &targetPtr_) {
    FlyingObject::targetPtr = targetPtr_;
}
void jam::FlyingObject::setDamage(float newDamage) {
    damage = newDamage;
}
bool jam::FlyingObject::operator<(const jam::FlyingObject &rhs) const {
    if (getPosition().y < rhs.getPosition().y) {
        return true;
    }
    if (getPosition().y == rhs.getPosition().y) {
        return getPosition().x < rhs.getPosition().x;
    }
    return false;
}
sf::FloatRect jam::FlyingObject::getGlobalBounds() const {
    return object.getGlobalBounds();
}
const sf::FloatRect &jam::Building::getHitBox() const {
    return hitBox;
}
void jam::Building::setHitBox(const sf::FloatRect &newHitBox) {
    Building::hitBox = newHitBox;
}
const sf::Vector2i &jam::Building::getSizeInMap() const {
    return sizeOnMap;
}
void jam::Building::setSizeInMap(const sf::Vector2i &newSize) {
    Building::sizeOnMap = newSize;
}
sf::Sprite *jam::Building::getSprite() {
    return &building;
}
const sf::Vector2i &jam::Building::getPosInMap() const {
    return posInMap;
}
void jam::Building::setPosInMap(const sf::Vector2i &newPosInMap) {
    Building::posInMap = newPosInMap;
    building.setPosition(sf::Vector2f(newPosInMap * (int)cellSize));
    hitBox = {building.getPosition(), {cellSize, cellSize}};
    building.move({0.5 * cellSize, cellSize});
}
void jam::Building::setTextureRect(const sf::IntRect &newRect) {
    building.setTextureRect(newRect);
    building.setOrigin(building.getGlobalBounds().width / 2,
                       building.getGlobalBounds().height);
}
void jam::Building::draw(sf::RenderTarget &window) const {
    building.setTexture(buildingTexture);
    window.draw(building);
    sf::RectangleShape rect({hitBox.width, hitBox.height});
    rect.setPosition(hitBox.left, hitBox.top);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineThickness(5);
    rect.setOutlineColor(sf::Color::Red);
    window.draw(rect);
}
jam::Building::Building(jam::Level &level_) : level(level_) {}

const sf::Texture &jam::Building::getBuildingTexture() const {
    return buildingTexture;
}
void jam::Building::loadBuildingTexture(const std::string &path) {
    checkLoad(buildingTexture, path);
}
bool jam::Building::operator<(const jam::Building &rhs) const {
    if (posInMap.y < (rhs).getPosInMap().y) {
        return true;
    }
    if (posInMap.y == rhs.getPosInMap().y) {
        return posInMap.x < rhs.getPosInMap().x;
    }
    return false;
}
const sf::Sprite &jam::Building::getBuilding() const {
    return building;
}
void jam::AttackBuilding::attack(const sf::Time &currentTime) const {
    if (currentTime - lastAttackTime < attackCooldown) {
        float coef =
            (currentTime - lastAttackTime) % attackCooldown / attackCooldown;
        attackBuildingAnimatedObject.updateFrame(coef);
        return;
    }
    bool found = false;
    for (auto &i : level.monsters) {
        if (quadraticDist(building.getPosition(),
                          i->getSprite()->getPosition()) <
                getAttackRange() * getAttackRange() &&
            i->isLive()) {
            flyingObject.setTexture(flyingObjectTexture);
            flyingObject.setPosition(
                building.getGlobalBounds().left + firePosition.x,
                building.getGlobalBounds().top + firePosition.y);
            flyingObject.setTargetPtr(i);
            level.flyingObjects.push_back(flyingObject);
            found = true;
            break;
        }
    }
    if (found) {
        lastAttackTime = currentTime;
    }
}
const sf::Time &jam::AttackBuilding::getAttackCooldown() const {
    return attackCooldown;
}
void jam::AttackBuilding::setAttackCooldown(const sf::Time &newAttackCooldown) {
    AttackBuilding::attackCooldown = newAttackCooldown;
}
void jam::Building::setScale(sf::Vector2f newScale) {
    building.setScale(newScale);
}
sf::FloatRect jam::Building::getGlobalBounds() const {
    return building.getGlobalBounds();
}
float jam::AttackBuilding::getAttackRange() const {
    return attackRange;
}
void jam::AttackBuilding::setAttackRange(float newAttackRange) {
    AttackBuilding::attackRange = newAttackRange;
}
void jam::AttackBuilding::loadFlyingObjectTexture(const std::string &path) {
    checkLoad(flyingObjectTexture, path);
}
void jam::AttackBuilding::setAttackPosition(const sf::Vector2f &newPos) {
    firePosition = newPos;
}
const sf::Vector2f &jam::AttackBuilding::getFirePosition() const {
    return firePosition;
}
void jam::AttackBuilding::setFlyingObject(
    const jam::FlyingObject &flyingObject_) {
    AttackBuilding::flyingObject = flyingObject_;
}
jam::AttackBuilding::AttackBuilding(jam::Level &level_) : Building(level_) {}
void jam::AttackBuilding::draw(sf::RenderTarget &window) const {
    Building::draw(window);
    attackBuildingAnimatedObject.draw(window);
}

void jam::AttackBuildingAnimatedObject::draw(sf::RenderTarget &window) const {
    object.setTexture(texture);
    object.setTextureRect(frames[curFrame]);
    window.draw(object);
}

void jam::AttackBuildingAnimatedObject::updateFrame(float coef) const {
    assert(!frames.empty());
    curFrame = static_cast<int>((float)frames.size() * coef);
}
void jam::AttackBuildingAnimatedObject::loadTexture(const std::string &path) {
    checkLoad(texture, path);
}

bool jam::Home::isEndGame() const {
    for (auto &i : level.monsters) {
        auto hitBox = i->getSprite()->getGlobalBounds();
        if (getHitBox().intersects({hitBox.left, hitBox.top + hitBox.height / 2,
                                    hitBox.width, hitBox.height / 2})) {
            return true;
        }
    }
    return false;
}

// SupportBuilding
jam::SupportBuilding::SupportBuilding(jam::Level &level_) : Building(level_) {}
void jam::SupportBuilding::doMagic(const sf::Time &curTime) const {
    if (curTime - lastMagicTime > magicCooldown) {
        magic(level, *this);
        lastMagicTime = curTime;
    }
}
