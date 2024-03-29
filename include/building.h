#pragma once

#include <functional>
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
    void draw(sf::RenderTarget &window) const;
    bool isFinished();
    sf::FloatRect getGlobalBounds() const;
    void setTargetPtr(const std::shared_ptr<TemplateCharacter> &targetPtr_);
    bool operator<(const FlyingObject &rhs) const;

protected:
    std::shared_ptr<TemplateCharacter> targetPtr{};
    mutable sf::Sprite object;
    float speed = 0;
    float damage = 0;
    mutable sf::Vector2f targetPos;
};

struct Building {
protected:
    sf::Vector2i sizeOnMap;
    sf::Vector2i posInMap;
    mutable sf::Sprite building;
    sf::FloatRect hitBox;
    Level &level;

    sf::Texture buildingTexture;

public:
    sf::FloatRect getGlobalBounds() const;
    const sf::Sprite &getBuilding() const;
    explicit Building(Level &level_);
    const sf::FloatRect &getHitBox() const;
    const sf::Texture &getBuildingTexture() const;
    void loadBuildingTexture(const std::string &path);
    void setHitBox(const sf::FloatRect &newHitBox);
    void setScale(sf::Vector2f newScale);
    const sf::Vector2i &getSizeInMap() const;
    void setSizeInMap(const sf::Vector2i &newSize);
    const sf::Vector2i &getPosInMap() const;
    void setPosInMap(const sf::Vector2i &newPosInMap);
    void setTextureRect(const sf::IntRect &newRect);
    virtual void draw(sf::RenderTarget &window) const;
    virtual ~Building() = default;
    virtual bool operator<(const jam::Building &rhs) const;
    sf::Sprite *getSprite();

private:
};

struct AttackBuilding;

struct AttackBuildingAnimatedObject {
    void updateFrame(float) const;
    void draw(sf::RenderTarget &window) const;
    void loadTexture(const std::string &);

    mutable sf::Sprite object;
    sf::Vector2f pos;
    sf::Texture texture;
    std::vector<sf::IntRect> frames;
    mutable int curFrame = 0;
};

struct AttackBuilding : Building {
    friend AttackBuildingAnimatedObject;
    AttackBuildingAnimatedObject attackBuildingAnimatedObject;

public:
    explicit AttackBuilding(Level &level_);
    void draw(sf::RenderTarget &window) const override;
    const sf::Time &getAttackCooldown() const;
    void setAttackCooldown(const sf::Time &newAttackCooldown);
    float getAttackRange() const;
    void setAttackRange(float newAttackRange);

    void loadFlyingObjectTexture(const std::string &path);
    void setAttackPosition(const sf::Vector2f &newPos);
    const sf::Vector2f &getFirePosition() const;

    void setFlyingObject(const FlyingObject &flyingObject_);
    void attack(const sf::Time &) const;

protected:
    sf::Texture flyingObjectTexture;
    mutable sf::Time lastAttackTime;
    sf::Time attackCooldown;
    float attackRange = 0;
    mutable FlyingObject flyingObject;
    sf::Vector2f firePosition;
};

struct SupportBuilding : Building {
    explicit SupportBuilding(Level &level_);

    std::function<void(Level &, const SupportBuilding &)> magic;
    void doMagic(const sf::Time &) const;
    mutable sf::Time lastMagicTime;
    sf::Time magicCooldown;

    mutable std::vector<sf::Vector2f> path;
};

struct Home : Building {
public:
    Home(Level &level_) : Building(level_) {}
};
}  // namespace jam