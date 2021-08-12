#pragma once
#include "Cell.h"
#include "Level.h"
#include "building.h"

namespace jam {
inline jam::AttackBuilding makeEmptyBuilding(jam::Level &level,
                                             const sf::Vector2i newPosInMap) {
    jam::AttackBuilding tower(level);
    tower.setPosInMap(newPosInMap);
    return tower;
}
inline jam::AttackBuilding makeArcherBuilding(jam::Level &level,
                                       const sf::Vector2i newPosInMap) {
    jam::AttackBuilding archersTower(level);
    archersTower.setAttackCooldown(sf::seconds(1));
    archersTower.setAttackRange(4 * cellSize);
    archersTower.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Lime/LimeTower.png");
    archersTower.setTextureRect({{16, 16}, {16, 32}});
    archersTower.setPosInMap(newPosInMap);
    archersTower.setSizeInMap({1, 2});
    archersTower.setHitBox(level.getMap()[archersTower.getPosInMap()
                                              .y][archersTower.getPosInMap()
                                                      .x].getGlobalBounds());
    archersTower.setScale({(float)cellSize / 16, (float)cellSize / 16});
    archersTower.loadFlyingObjectTexture(
        "data/images/MiniWorldSprites/Objects/ArrowLong.png");
    archersTower.setAttackPosition({cellSize / 2, cellSize / 2});
    jam::FlyingObject arrow;
    arrow.setTextureRect({{5, 3}, {5, 11}});
    arrow.setOrigin(sf::Vector2f(2.5, 0));
    arrow.setScale(5, 5);
    arrow.setSpeed(1);
    arrow.setDamage(20);

    archersTower.setFlyingObject(arrow);
    return archersTower;
}

inline jam::Home makeHome(jam::Level& level) {
    jam::Home home(level);
    home.loadBuildingTexture("data/images/MiniWorldSprites/Buildings/Lime/LimeHouses.png");
    home.setTextureRect({ 16, 3 * 16, 16, 16 });
    home.setPosInMap({ 0, 0 });
    home.setSizeInMap({ 1, 1 });
    home.setHitBox(level.getMap()[home.getPosInMap().y][home.getPosInMap().x].getGlobalBounds());
    home.setScale({ (float)cellSize / 16, (float)cellSize / 16 });
    return home;
}
}