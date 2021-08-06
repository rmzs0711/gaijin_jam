#pragma once
#include "SFML/Graphics.hpp"

struct Building {
protected:
    sf::Vector2f size;
    sf::Vector2i posInMap;
    sf::Sprite asset;

public:
    const sf::Vector2f &getSize() const {
        return size;
    }
    void setSize(const sf::Vector2f &newSize) {
        Building::size = newSize;
    }
    const sf::Vector2i &getPosInMap() const {
        return posInMap;
    }
    void setPosInMap(const sf::Vector2i &newPosInMap) {
        Building::posInMap = newPosInMap;
    }
    void setTexture(const sf::Texture& newTexture, bool resetRect = true) {
        asset.setTexture(newTexture, resetRect);
    }
    void setTextureRect(const sf::IntRect& newRect) {
        asset.setTextureRect(newRect);
    }
private:
    void get() {}
};

struct AttackBuilding : Building {};

struct DefenseBuilding : Building {};
