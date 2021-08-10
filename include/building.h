#pragma once

#include "Level.h"
#include "SFML/Graphics.hpp"
#include "characters.h"

struct TemplateCharacter;

namespace jam {
struct Level;
struct FlyingObject : sf::Transformable {
public:
    float getDamage() const;
    void setDamage(float newDamage);
    const sf::Sprite &getObject() const;
    void setTexture(const sf::Texture &texture);
    void setTextureRect(const sf::IntRect &rect);
    float getSpeed() const;
    void setSpeed(float newSpeed);
    void draw(sf::RenderWindow &window);
    bool isFinished();

    void setTargetPtr(const std::shared_ptr<TemplateCharacter> &targetPtr_);

protected:
    std::shared_ptr<TemplateCharacter> targetPtr{};
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
    sf::FloatRect hitBox;
    Level &level;

    sf::Texture buildingTexture;

public:
    Building(Level &level_);
    const sf::FloatRect &getHitBox() const;
    const sf::Texture &getBuildingTexture() const;
    void loadBuildingTexture(const std::string &path);
    void setHitBox(const sf::FloatRect &newHitBox);

    const sf::Vector2i &getSizeInMap() const;
    void setSizeInMap(const sf::Vector2i &newSize);
    const sf::Vector2i &getPosInMap() const;
    void setPosInMap(const sf::Vector2i &newPosInMap);
    void setTexture(const sf::Texture &newTexture);
    void setTextureRect(const sf::IntRect &newRect);
    void draw(sf::RenderWindow &window);

private:
};

struct AttackBuilding : Building {
protected:
public:
    explicit AttackBuilding(Level &level_, sf::Clock &clock_)
        : Building(level_), clock(clock_) {}
    const sf::Time &getAttackCooldown() const;
    void setAttackCooldown(const sf::Time &newAttackCooldown);
    void setScale(sf::Vector2f newScale);
    float getAttackRange() const;
    void setAttackRange(float newAttackRange);

    void loadFlyingObjectTextureFromFile(const std::string &path);
    void setAttackPosition(const sf::Vector2f &newPos);
    const sf::Vector2f &getFirePosition() const;

    void setFlyingObject(const FlyingObject &flyingObject_);
    void attack();

protected:
    sf::Clock &clock;
    sf::Texture flyingObjectTexture;
    sf::Time lastAttackTime;
    sf::Time attackCooldown;
    float attackRange = 0;
    FlyingObject flyingObject;
    sf::Vector2f firePosition;
};

struct SupportBuilding : Building {};
}  // namespace jam