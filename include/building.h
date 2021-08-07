#pragma once
#include "SFML/Graphics.hpp"
#include "characters.h"

namespace jam {

struct FlyingObject : sf::Transformable {
public:
    explicit FlyingObject(const std::shared_ptr<MonsterStanding> &targetPtr_)
        : targetPtr(targetPtr_) {}

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
        auto &targetPos = targetPtr->getSprite()->getPosition();

        auto xDist = targetPos.x - curPos.x;
        auto yDist = targetPos.y - curPos.y;
        auto Dist = std::sqrt((xDist * xDist) + (yDist * yDist));

//        move(speed * sf::Vector2f{xDist / Dist, yDist / Dist});

//        object.setRotation(std::acos(xDist / Dist));

        object.setPosition(getPosition());
        window.draw(object);
        object.setColor(sf::Color::Red);
        object.setScale(1000, 1000);
        window.draw(object);
    }
    bool isFinished() {
        return object.getGlobalBounds().intersects(
            targetPtr->getSprite()->getGlobalBounds());
    }

protected:
    std::shared_ptr<MonsterStanding> targetPtr{};
    sf::Sprite object;
    float speed = 0;
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
    }
    void setTexture(const sf::Texture &newTexture, bool resetRect = true) {
        building.setTexture(newTexture, resetRect);
    }
    void setTextureRect(const sf::IntRect &newRect) {
        building.setTextureRect(newRect);
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
    float getDamage() const {
        return damage;
    }
    void setDamage(float newDamage) {
        AttackBuilding::damage = newDamage;
    }
    void loadFlyingObjectTextureFromFile(const std::string &path) {
        checkLoad(flyingObjectTexture, path);
    }
    sf::Vector2f getPosition() {
        return sf::Vector2f(getPosInMap() * (int)cellSize) +
               sf::Vector2f(getSizeInMap() * (int)cellSize) / 2.f;
    }

    void attack(const std::vector<std::shared_ptr<MonsterStanding>> &monsters) {
        if (clock.getElapsedTime() - lastAttackTime < attackCooldown) {
            return;
        }
        for (auto &i : monsters) {
            if (quadraticDist(getPosition(), i->getSprite()->getPosition()) <
                getAttackRange() * getAttackRange()) {
                FlyingObject arrow(i);
                arrow.setTexture(flyingObjectTexture);
                arrow.setTextureRect({{0, 16}, assetCellSize / 2});
                arrow.setPosition(getPosition());
                arrow.setScale(10, 10);
                arrow.setSpeed(10);
                arrow.setOrigin(sf::Vector2f(assetCellSize / 2));
                objects.push_back(arrow);
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
    float damage = 0;
    std::vector<FlyingObject> &objects;
};

struct SupportBuilding : Building {};
}  // namespace jam