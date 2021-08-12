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
    archersTower.setAttackCooldown(sf::seconds(0.7));
    archersTower.setAttackRange(4 * cellSize);
    archersTower.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Lime/LimeTower.png");
    archersTower.setTextureRect({{assetCellSize.x, assetCellSize.x},
                                 {assetCellSize.x, assetCellSize.x * 2}});
    archersTower.setPosInMap(newPosInMap);
    archersTower.setSizeInMap({1, 2});
    archersTower.setHitBox(level
                               .getMap()[archersTower.getPosInMap().y]
                                        [archersTower.getPosInMap().x]
                               .getGlobalBounds());
    archersTower.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    archersTower.loadFlyingObjectTexture(
        "data/images/MiniWorldSprites/Objects/ArrowLong.png");
    archersTower.setAttackPosition({cellSize / 2, cellSize / 2});
    jam::FlyingObject arrow;
    arrow.setTextureRect({{5, 3}, {5, 11}});
    arrow.setOrigin(sf::Vector2f(2.5, 0));
    arrow.setScale(5, 5);
    arrow.setSpeed(5);
    arrow.setDamage(10);
    archersTower.attackBuildingAnimatedObject.pos =
        archersTower.getFirePosition() +
        sf::Vector2f{archersTower.getBuilding().getGlobalBounds().left,
                     archersTower.getBuilding().getGlobalBounds().top};
    ;
    for (int i = 0; i < 4; i++) {
        archersTower.attackBuildingAnimatedObject.frames.emplace_back(
            sf::IntRect({(2 + i) * assetCellSize.x, assetCellSize.x},
                        assetCellSize));
    }
    archersTower.attackBuildingAnimatedObject.texture.loadFromFile(
        "data/images/MiniWorldSprites/Characters/Soldiers/Ranged/Ballista.png");
    archersTower.attackBuildingAnimatedObject.object.setTexture(
        archersTower.attackBuildingAnimatedObject.texture);
    archersTower.attackBuildingAnimatedObject.object.setPosition(
        archersTower.attackBuildingAnimatedObject.pos);
    archersTower.attackBuildingAnimatedObject.object.setOrigin(8, 8);
    archersTower.attackBuildingAnimatedObject.object.setScale(5, 5);
    archersTower.setFlyingObject(arrow);

    return archersTower;
}

inline jam::AttackBuilding makeSniperBuilding(jam::Level &level,
                                              const sf::Vector2i newPosInMap) {
    jam::AttackBuilding sniperTower(level);
    sniperTower.setAttackCooldown(sf::seconds(2.5));
    sniperTower.setAttackRange(6 * cellSize);
    sniperTower.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Red/RedTower.png");
    sniperTower.setTextureRect({{assetCellSize.x, assetCellSize.x},
                                {assetCellSize.x, assetCellSize.x * 2}});
    sniperTower.setPosInMap(newPosInMap);
    sniperTower.setSizeInMap({1, 2});
    sniperTower.setHitBox(
        level.getMap()[sniperTower.getPosInMap().y][sniperTower.getPosInMap().x]
            .getGlobalBounds());
    sniperTower.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    sniperTower.loadFlyingObjectTexture(
        "data/images/MiniWorldSprites/Objects/Bullet.png");
    sniperTower.setAttackPosition({cellSize / 2, cellSize / 2});
    jam::FlyingObject bullet;
    bullet.setTextureRect({{6, 6}, {4, 4}});
    bullet.setOrigin(sf::Vector2f(2, 2));
    bullet.setScale(2, 2);
    bullet.setSpeed(8);
    bullet.setDamage(30);

    sniperTower.attackBuildingAnimatedObject.pos =
        sniperTower.getFirePosition() +
        sf::Vector2f{sniperTower.getBuilding().getGlobalBounds().left,
                     sniperTower.getBuilding().getGlobalBounds().top - 40};

    for (int i = 0; i < 4; i++) {
        sniperTower.attackBuildingAnimatedObject.frames.emplace_back(
            sf::IntRect({(i) * assetCellSize.x, 4 * assetCellSize.x},
                        assetCellSize));
    }
    sniperTower.attackBuildingAnimatedObject.texture.loadFromFile(
        "data/images/MiniWorldSprites/Characters/Soldiers/Ranged/RedRanged"
        "/MusketeerRed.png");
    sniperTower.attackBuildingAnimatedObject.object.setTexture(
        sniperTower.attackBuildingAnimatedObject.texture);
    sniperTower.attackBuildingAnimatedObject.object.setPosition(
        sniperTower.attackBuildingAnimatedObject.pos);
    sniperTower.attackBuildingAnimatedObject.object.setOrigin(8, 8);
    sniperTower.attackBuildingAnimatedObject.object.setScale(5, 5);
    sniperTower.setFlyingObject(bullet);

    return sniperTower;
}

inline jam::AttackBuilding makeWizardTower(jam::Level &level,
                                           const sf::Vector2i newPosInMap) {
    jam::AttackBuilding wizardTower(level);
    wizardTower.setAttackCooldown(sf::seconds(1.5));
    wizardTower.setAttackRange(10    * cellSize);
    wizardTower.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Cyan/CyanTower.png");
    wizardTower.setTextureRect(
        {{assetCellSize.x, assetCellSize.x}, {assetCellSize.x, 32}});
    wizardTower.setPosInMap(newPosInMap);
    wizardTower.setSizeInMap({1, 2});
    wizardTower.setHitBox(
        level.getMap()[wizardTower.getPosInMap().y][wizardTower.getPosInMap().x]
            .getGlobalBounds());
    wizardTower.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    wizardTower.loadFlyingObjectTexture(
        "data/images/MiniWorldSprites/Objects/FireballProjectile.png");
    wizardTower.setAttackPosition({cellSize / 2, cellSize / 2 - 10});
    jam::FlyingObject fireball;
    fireball.setTextureRect(
        {{assetCellSize.x * 3, 0}, {assetCellSize.x, assetCellSize.x}});
    fireball.setOrigin(sf::Vector2f(8, 8));
    fireball.setScale(4, 4);
    fireball.setSpeed(1);
    fireball.setDamage(40);
    wizardTower.setFlyingObject(fireball);

    wizardTower.attackBuildingAnimatedObject.loadTexture(
        "data/images/MiniWorldSprites/Characters/Soldiers/Ranged"
        "/CyanRanged/MageCyan.png");
    for (int i = 0; i < 4; i++) {
        wizardTower.attackBuildingAnimatedObject.frames.emplace_back(
            sf::IntRect({i * assetCellSize.x, assetCellSize.x * 4},
                        assetCellSize));
    }
    wizardTower.attackBuildingAnimatedObject.pos =
        wizardTower.getFirePosition() +
        sf::Vector2f{wizardTower.getBuilding().getGlobalBounds().left,
                     wizardTower.getBuilding().getGlobalBounds().top};
    wizardTower.attackBuildingAnimatedObject.object.setPosition(
        wizardTower.attackBuildingAnimatedObject.pos);
    wizardTower.attackBuildingAnimatedObject.object.setOrigin(8, 8);
    wizardTower.attackBuildingAnimatedObject.object.setScale(5, 5);
    return wizardTower;
}
inline jam::Home makeHome(jam::Level &level) {
    jam::Home home(level);
    home.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Lime/LimeHouses.png");
    home.setTextureRect({16, 3 * 16, 16, 16});
    home.setPosInMap({4, 1});
    home.setSizeInMap({1, 1});
    home.setHitBox(level.getMap()[home.getPosInMap().y][home.getPosInMap().x]
                       .getGlobalBounds());
    home.setScale({(float)cellSize / 16, (float)cellSize / 16});
    return home;
}
}  // namespace jam