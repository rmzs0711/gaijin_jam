
#include <building.h>

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
void jam::FlyingObject::draw(sf::RenderWindow &window) {
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
    object.setRotation((asin(yDist / Dist) < 0 ? -1 : 1) *
                           std::acos(xDist / Dist) * 180 / M_PI -
                       90);

    object.setPosition(getPosition());
    window.draw(object);
}
bool jam::FlyingObject::isFinished() {
    if (object.getGlobalBounds().contains(targetPos)) {
        //        targetPtr;
        // todo damage
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
const sf::Vector2i &jam::Building::getPosInMap() const {
    return posInMap;
}
void jam::Building::setPosInMap(const sf::Vector2i &newPosInMap) {
    Building::posInMap = newPosInMap;
    building.setPosition(sf::Vector2f(newPosInMap * (int)cellSize));
    hitBox = {building.getPosition(), {cellSize, cellSize}};
    building.move({0.5 * cellSize, cellSize});
}
void jam::Building::setTexture(const sf::Texture &newTexture) {
    building.setTexture(newTexture);
}
void jam::Building::setTextureRect(const sf::IntRect &newRect) {
    building.setTextureRect(newRect);
    building.setOrigin(building.getGlobalBounds().width / 2,
                       building.getGlobalBounds().height);
}
void jam::Building::draw(sf::RenderWindow &window) {
    window.draw(building);
    sf::RectangleShape rect({hitBox.width, hitBox.height});
    rect.setPosition(hitBox.left, hitBox.left);
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
    Building::buildingTexture.loadFromFile(path);
}
void jam::AttackBuilding::attack() {
    if (clock.getElapsedTime() - lastAttackTime < attackCooldown) {
        return;
    }
    for (auto &i : level.monsters) {
        if (quadraticDist(building.getPosition(),
                          i->getSprite()->getPosition()) <
            getAttackRange() * getAttackRange()) {
            flyingObject.setTexture(flyingObjectTexture);
            flyingObject.setPosition(
                building.getGlobalBounds().left + firePosition.x,
                building.getGlobalBounds().top + firePosition.y);
            flyingObject.setTargetPtr(i);
            level.flyingObjects.push_back(flyingObject);
            break;
        }
    }
    lastAttackTime = clock.getElapsedTime();
}
const sf::Time &jam::AttackBuilding::getAttackCooldown() const {
    return attackCooldown;
}
void jam::AttackBuilding::setAttackCooldown(const sf::Time &newAttackCooldown) {
    AttackBuilding::attackCooldown = newAttackCooldown;
}
void jam::AttackBuilding::setScale(sf::Vector2f newScale) {
    building.setScale(newScale);
}
float jam::AttackBuilding::getAttackRange() const {
    return attackRange;
}
void jam::AttackBuilding::setAttackRange(float newAttackRange) {
    AttackBuilding::attackRange = newAttackRange;
}
void jam::AttackBuilding::loadFlyingObjectTextureFromFile(
    const std::string &path) {
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
