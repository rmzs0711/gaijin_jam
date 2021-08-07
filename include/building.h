#pragma once
#include "SFML/Graphics.hpp"
#include "characters.h"

namespace jam {

struct FlyingObject : sf::Transformable {
public:
    float getDamage() const {
        return damage;
    }
    void setDamage(float newDamage) {
        damage = newDamage;
    }
    const sf::Sprite &getObject() const {
        return object;
    }
    void setTexture(const sf::Texture &texture) {
        FlyingObject::object.setTexture(texture);
    }
    void setTextureRect(const sf::IntRect &rect) {
        FlyingObject::object.setTextureRect(rect);
    }
    float getSpeed() const {
        return speed;
    }
    void setSpeed(float newSpeed) {
        FlyingObject::speed = newSpeed;
    }
    void draw(sf::RenderWindow &window) {
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
    bool isFinished() {
        if (object.getGlobalBounds().contains(targetPos)) {
            targetPtr->takeDamage(damage);
            return true;
        }
        return false;
    }

    void setTargetPtr(const std::shared_ptr<MonsterStanding> &targetPtr_) {
        FlyingObject::targetPtr = targetPtr_;
    }

protected:
    std::shared_ptr<MonsterStanding> targetPtr{};
    sf::Sprite object;
    float speed = 0;
    float damage = 0;
    sf::Vector2f targetPos;
};

struct Building {
protected:
    sf::Vector2i sizeOnMap;
    sf::Vector2i posInMap;
    sf::Sprite building;

public:
    const sf::Vector2i &getSizeInMap() const {
        return sizeOnMap;
    }
    void setSizeInMap(const sf::Vector2i &newSize) {
        Building::sizeOnMap = newSize;
    }
    const sf::Vector2i &getPosInMap() const {
        return posInMap;
    }
    void setPosInMap(const sf::Vector2i &newPosInMap) {
        Building::posInMap = newPosInMap;
        building.setPosition(sf::Vector2f(newPosInMap * (int)cellSize));
        building.move({0.5 * cellSize, cellSize});
    }
    void setTexture(const sf::Texture &newTexture) {
        building.setTexture(newTexture);
    }
    void setTextureRect(const sf::IntRect &newRect) {
        building.setTextureRect(newRect);
        building.setOrigin(building.getGlobalBounds().width / 2,
                           building.getGlobalBounds().height);
    }
    void draw(sf::RenderWindow &window) {
        window.draw(building);
    }

private:
};

struct AttackBuilding : Building {
protected:
public:
    explicit AttackBuilding(std::vector<FlyingObject> &objects_,
                            sf::Clock &clock_)
        : objects(objects_), clock(clock_) {}
    const sf::Time &getAttackCooldown() const {
        return attackCooldown;
    }
    void setAttackCooldown(const sf::Time &newAttackCooldown) {
        AttackBuilding::attackCooldown = newAttackCooldown;
    }
    void setScale(sf::Vector2f newScale) {
        building.setScale(newScale);
    }
    float getAttackRange() const {
        return attackRange;
    }
    void setAttackRange(float newAttackRange) {
        AttackBuilding::attackRange = newAttackRange;
    }

    void loadFlyingObjectTextureFromFile(const std::string &path) {
        checkLoad(flyingObjectTexture, path);
    }
    void setAttackPosition(const sf::Vector2f &newPos) {
        firePosition = newPos;
    }
    const sf::Vector2f &getFirePosition() const {
        return firePosition;
    }

    void setFlyingObject(const FlyingObject &flyingObject_) {
        AttackBuilding::flyingObject = flyingObject_;
    }
    void attack(const std::vector<std::shared_ptr<MonsterStanding>> &monsters) {
        if (clock.getElapsedTime() - lastAttackTime < attackCooldown) {
            return;
        }
        for (auto &i : monsters) {
            if (quadraticDist(building.getPosition(),
                              i->getSprite()->getPosition()) <
                getAttackRange() * getAttackRange()) {
                flyingObject.setTexture(flyingObjectTexture);
                flyingObject.setPosition(
                    building.getGlobalBounds().left + firePosition.x,
                    building.getGlobalBounds().top + firePosition.y);
                flyingObject.setTargetPtr(i);
                objects.push_back(flyingObject);
                break;
            }
        }
        lastAttackTime = clock.getElapsedTime();
    }

protected:
    sf::Clock &clock;
    sf::Texture flyingObjectTexture;
    sf::Time lastAttackTime;
    sf::Time attackCooldown;
    float attackRange = 0;
    std::vector<FlyingObject> &objects;
    FlyingObject flyingObject;
    sf::Vector2f firePosition;
};

struct SupportBuilding : Building {};
}  // namespace jam