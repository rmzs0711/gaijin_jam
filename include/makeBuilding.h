#pragma once
#include "Cell.h"
#include "Level.h"
#include "building.h"
#include "characters.h"

namespace jam {
inline jam::Building makeEmptyBuilding(jam::Level &level,
                                       const sf::Vector2i newPosInMap) {
    jam::Building tower(level);
    tower.setPosInMap(newPosInMap);
    return tower;
}
inline jam::AttackBuilding makeEmptyAttackBuilding(
    jam::Level &level,
    const sf::Vector2i newPosInMap) {
    jam::AttackBuilding tower(level);
    tower.setPosInMap(newPosInMap);
    return tower;
}
inline jam::SupportBuilding makeEmptySupportBuilding(
    jam::Level &level,
    const sf::Vector2i newPosInMap) {
    jam::SupportBuilding tower(level);
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
    archersTower.setSizeInMap({1, 2});
    archersTower.setPosInMap(newPosInMap);
    archersTower.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    archersTower.setHitBox(level
                               .getMap()[archersTower.getPosInMap().y]
                                        [archersTower.getPosInMap().x]
                               .getGlobalBounds());
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
    sniperTower.setSizeInMap({1, 2});
    sniperTower.setPosInMap(newPosInMap);
    sniperTower.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    sniperTower.setHitBox(
        level.getMap()[sniperTower.getPosInMap().y][sniperTower.getPosInMap().x]
            .getGlobalBounds());
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
            sf::IntRect({(i)*assetCellSize.x, 4 * assetCellSize.x},
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
    wizardTower.setAttackRange(2 * cellSize);
    wizardTower.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Cyan/CyanTower.png");
    wizardTower.setTextureRect(
        {{assetCellSize.x, assetCellSize.x}, {assetCellSize.x, 32}});
    wizardTower.setSizeInMap({1, 2});
    wizardTower.setPosInMap(newPosInMap);
    wizardTower.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    wizardTower.setHitBox(
        level.getMap()[wizardTower.getPosInMap().y][wizardTower.getPosInMap().x]
            .getGlobalBounds());
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
inline jam::Home makeHome(jam::Level &level, int number) {
    jam::Home home(level);
    home.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Purple/PurpleKeep.png");
    home.setTextureRect({0, 0, 32, 32});
    home.setSizeInMap({2, 2});

    if (number == 1) {
        home.setPosInMap({ 1, 2 });
    }
    else {
        home.setPosInMap({ 47, 48 });
    }

    home.setScale({(float)cellSize / 16, (float)cellSize / 16});
    home.setHitBox(home.getSprite()->getGlobalBounds());
//    home.setHitBox(level.getMap()[home.getPosInMap().y][home.getPosInMap().x]
//                       .getGlobalBounds());
    return home;
}

// Support Building
inline SupportBuilding makeBarrack(jam::Level &level,
                                   const sf::Vector2i newPosInMap) {
    jam::SupportBuilding barrack(level);
    barrack.magicCooldown = sf::seconds(10);
    barrack.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Wood/Barracks.png");
    barrack.setTextureRect({{0, 0}, {assetCellSize.x, assetCellSize.x}});
    barrack.setSizeInMap({1, 1});
    barrack.setPosInMap(newPosInMap);
    barrack.setHitBox(
        level.getMap()[barrack.getPosInMap().y][barrack.getPosInMap().x]
            .getGlobalBounds());
    barrack.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    barrack.magic = [&](Level &curLevel, const SupportBuilding &building) {
        sf::Vector2i cell = {-1, -1};
        auto &map = curLevel.getMap();
        for (int i = -1; i < 2 && cell == sf::Vector2i{-1, -1}; ++i) {
            for (int j = -1; j < 2 && cell == sf::Vector2i{-1, -1}; ++j) {
                cell = {
                    bounds(building.getPosInMap().x + i, 0, (int)map[0].size()),
                    bounds(building.getPosInMap().y, 0, (int)map.size())};
                if (map[cell.y][cell.x].getBackgroundType() !=
                    jam::CellBackground::ROAD) {
                    cell = {-1, -1};
                }
            }
        }
        if (cell != sf::Vector2i{-1, -1}) {
            int randValue = rand() % 10;
            if (randValue == 0) {
                curLevel.addHero(Hero::makeAssasinRed(
                    level,
                    sf::Vector2f(
                        map[cell.y][cell.x].getGlobalBounds().left +
                            map[cell.y][cell.x].getGlobalBounds().width / 2,
                        map[cell.y][cell.x].getGlobalBounds().top +
                            map[cell.y][cell.x].getGlobalBounds().height / 2)));
            } else if (randValue >= 1 && randValue <= 2) {
                curLevel.addHero(Hero::makeAssasinCyan(
                    level,
                    sf::Vector2f(
                        map[cell.y][cell.x].getGlobalBounds().left +
                            map[cell.y][cell.x].getGlobalBounds().width / 2,
                        map[cell.y][cell.x].getGlobalBounds().top +
                            map[cell.y][cell.x].getGlobalBounds().height / 2)));
            } else if (randValue >= 3 && randValue <= 5) {
                curLevel.addHero(Hero::makeAssasinLime(
                    level,
                    sf::Vector2f(
                        map[cell.y][cell.x].getGlobalBounds().left +
                            map[cell.y][cell.x].getGlobalBounds().width / 2,
                        map[cell.y][cell.x].getGlobalBounds().top +
                            map[cell.y][cell.x].getGlobalBounds().height / 2)));
            } else if (randValue >= 6 && randValue <= 9) {
                curLevel.addHero(Hero::makeAssasinPurple(
                    level,
                    sf::Vector2f(
                        map[cell.y][cell.x].getGlobalBounds().left +
                            map[cell.y][cell.x].getGlobalBounds().width / 2,
                        map[cell.y][cell.x].getGlobalBounds().top +
                            map[cell.y][cell.x].getGlobalBounds().height / 2)));
            }
        }
    };
    return barrack;
}
inline SupportBuilding makeHospital(jam::Level &level,
                                    const sf::Vector2i newPosInMap) {
    jam::SupportBuilding hospital(level);
    hospital.magicCooldown = sf::seconds(2);
    float heal = 10;
    hospital.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Cyan/CyanChapels.png");
    hospital.setTextureRect({assetCellSize, assetCellSize});
    hospital.setSizeInMap({1, 1});
    hospital.setPosInMap(newPosInMap);
    hospital.setHitBox(
        level.getMap()[hospital.getPosInMap().y][hospital.getPosInMap().x]
            .getGlobalBounds());
    hospital.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    hospital.magic = [&](Level &curLevel, const SupportBuilding &building) {
        auto &map = curLevel.getMap();
        sf::Vector2f startPos = {building.getGlobalBounds().left,
                                 building.getGlobalBounds().top};
        sf::Vector2f endPos =
            startPos + sf::Vector2f{building.getGlobalBounds().width,
                                    building.getGlobalBounds().height};
        auto start = std::lower_bound(
            curLevel.heroes.begin(), curLevel.heroes.end(),
            Hero::makeAssasinPurple(
                level, startPos - sf::Vector2f(cellSize, cellSize)),
            charactersCompare);
        auto end = std::lower_bound(
            curLevel.heroes.begin(), curLevel.heroes.end(),
            Hero::makeAssasinPurple(level,
                                    endPos + sf::Vector2f(cellSize, cellSize)));
        for (auto i = start; i != end; i++) {
            if ((*i)->getSprite()->getPosition().x >
                    startPos.x - (float)cellSize &&
                (*i)->getSprite()->getPosition().x <
                    endPos.x + (float)cellSize &&
                dynamic_cast<Hero &>(**i).getState() != FIGHTING) {
                (*i)->takeDamage(-20);
            }
        }
    };
    return hospital;
}
inline SupportBuilding makeWell(jam::Level &level,
                                const sf::Vector2i newPosInMap) {
    jam::SupportBuilding well(level);
    well.magicCooldown = sf::seconds(1);
    float manaBonus = 0.5;
    well.loadBuildingTexture(
        "data/images/MiniWorldSprites/Miscellaneous/Well.png");
    well.setTextureRect({{0, assetCellSize.x}, assetCellSize});
    well.setSizeInMap({1, 1});
    well.setPosInMap(newPosInMap);
    well.setHitBox(level.getMap()[well.getPosInMap().y][well.getPosInMap().x]
                       .getGlobalBounds());
    well.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    well.magic = [&](Level &curLevel, const SupportBuilding &building) {
        curLevel.mana += manaBonus;
        curLevel.mana = std::min(curLevel.mana, curLevel.maxMana);
    };
    return well;
}
inline SupportBuilding makeMinerCave(jam::Level &level,
                                     const sf::Vector2i newPosInMap) {
    jam::SupportBuilding minerCave(level);
    minerCave.magicCooldown = sf::seconds(60);
    int goldBonus = 1;
    minerCave.loadBuildingTexture(
        "data/images/MiniWorldSprites/Buildings/Wood/CaveV2.png");
    int randValue = rand() % 21;
    if (randValue == 0) {
        minerCave.setTextureRect({{2 * assetCellSize.x, assetCellSize.x},
                                  {assetCellSize.x, assetCellSize.x}});
    } else if (randValue >= 1 && randValue <= 2) {
        minerCave.setTextureRect({{assetCellSize.x, assetCellSize.x},
                                  {assetCellSize.x, assetCellSize.x}});
    } else if (randValue >= 3 && randValue <= 5) {
        minerCave.setTextureRect(
            {{0, assetCellSize.x}, {assetCellSize.x, assetCellSize.x}});
    } else if (randValue >= 6 && randValue <= 9) {
        minerCave.setTextureRect(
            {{2 * assetCellSize.x, 0}, {assetCellSize.x, assetCellSize.x}});
    } else if (randValue >= 10 && randValue <= 14) {
        minerCave.setTextureRect(
            {{assetCellSize.x, 0}, {assetCellSize.x, assetCellSize.x}});
    } else if (randValue >= 15 && randValue <= 20) {
        minerCave.setTextureRect({{0, 0}, {assetCellSize.x, assetCellSize.x}});
    }
    minerCave.setSizeInMap({1, 1});
    minerCave.setPosInMap(newPosInMap);
    minerCave.setHitBox(
        level.getMap()[minerCave.getPosInMap().y][minerCave.getPosInMap().x]
            .getGlobalBounds());
    minerCave.setScale(
        {(float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x});
    minerCave.magic = [&](Level &curLevel, const SupportBuilding &building) {
        auto &map = curLevel.getMap();
        auto rect = building.getBuilding().getTextureRect();
        level.addMoney(Money::makeMoney(10 * (
            rect.left / assetCellSize.x + 1 + rect.top / assetCellSize.y * 3),
            building.getBuilding().getPosition()
                -
                sf::Vector2f(cellSize, cellSize) / 2.f
            ));
    };
    return minerCave;
}

inline bool isBackgroundForPortal(std::vector<std::vector<jam::Cell>> &map, sf::Vector2i position) {
    assert(position.x >= 0 && position.x < 50 && position.y >= 0 && position.y < 50);

    if (map[position.y][position.x].getBackgroundType() != jam::CellBackground::ROAD) {
        if ((position.y > 0 && map[position.y - 1][position.x].getBackgroundType() == jam::CellBackground::ROAD) ||
            (position.y < 49 && map[position.y + 1][position.x].getBackgroundType() == jam::CellBackground::ROAD) ||
            (position.x > 0 && map[position.y][position.x - 1].getBackgroundType() == jam::CellBackground::ROAD) ||
            (position.x < 49 && map[position.y][position.x + 1].getBackgroundType() == jam::CellBackground::ROAD)) {

            return true;
        }
    }
    return false;
}

inline SupportBuilding makeHomeMonster(jam::Level& level, sf::Vector2i newPosInMap = sf::Vector2i(0, 0)) {
    sf::Clock clock;
    jam::SupportBuilding homeMonster(level);
    homeMonster.magicCooldown = sf::seconds(13);

    homeMonster.loadBuildingTexture(
        "data/images/MiniWorldSprites/Miscellaneous/Portal.png");
    homeMonster.setTextureRect(sf::IntRect(0, 0, 16, 16));
    auto& map_ = level.getMap();
    homeMonster.setSizeInMap({ 1, 1 });
    homeMonster.setPosInMap(newPosInMap);

    homeMonster.setHitBox(
        level.getMap()[homeMonster.getPosInMap().y][homeMonster.getPosInMap().x]
        .getGlobalBounds());
    homeMonster.setScale(
        { (float)cellSize / assetCellSize.x, (float)cellSize / assetCellSize.x });

    homeMonster.path = getPathMonster(map_, level.home[rand()
        % level.home.size()].getPosInMap(), newPosInMap);

    if (homeMonster.path.size() == 0) {
        homeMonster.path.push_back({ 0, 0 });
    }

    homeMonster.magic = [&](Level& curLevel, const SupportBuilding& building) {
        auto& map = curLevel.getMap();

        if (rand() % 2 == 1) {
            return;
        }

        int randValue = rand() % 18;

            if (randValue == 0) {
                curLevel.addMonster(Monster::makeArmouredRedDemon(level, building.path));
            }
            else if (randValue == 1) {
                curLevel.addMonster(Monster::makeRedDemon(level, building.path));
            }
            else if (randValue == 2) {
                curLevel.addMonster(Monster::makePurpleDemon(level, building.path));
            }
            else if (randValue == 3) {
                curLevel.addMonster(Monster::makeMammoth(level, building.path));
            }
            else if (randValue == 4) {
                curLevel.addMonster(Monster::makeWendigo(level, building.path));
            }
            else if (randValue == 5) {
                curLevel.addMonster(Monster::makeYeti(level, building.path));
            }
            else if (randValue == 6) {
                curLevel.addMonster(Monster::makeArcherGoblin(level, building.path));
            }
            else if (randValue == 7) {
                curLevel.addMonster(Monster::makeClubGoblin(level, building.path));
            }
            else if (randValue == 8) {
                curLevel.addMonster(Monster::makeFarmerGoblin(level, building.path));
            }
            else if (randValue == 9) {
                curLevel.addMonster(Monster::makeKamikazeGoblin(level, building.path));
            }
            else if (randValue == 10) {
                curLevel.addMonster(Monster::makeOrc(level, building.path));
            }
            else if (randValue == 11) {
                curLevel.addMonster(Monster::makeOrcMage(level, building.path));
            }
            else if (randValue == 12) {
                curLevel.addMonster(Monster::makeOrcShaman(level, building.path));
            }
            else if (randValue == 13) {
                curLevel.addMonster(Monster::makePirateCaptain(level, building.path));
            }
            else if (randValue == 14) {
                curLevel.addMonster(Monster::makePirateGrunt(level, building.path));
            }
            else if (randValue == 15) {
                curLevel.addMonster(Monster::makePirateGunnern(level, building.path));
            }
            else if (randValue == 16) {
                curLevel.addMonster(Monster::makeNecromancer(level, building.path));
            }
            else if (randValue == 17) {
                curLevel.addMonster(Monster::makeSkeletonSoldier(level, building.path));
            }

            curLevel.monsters[curLevel.monsters.size() - 1]->setPosition(building.path[building.path.size() - 1]);
    };
    return homeMonster;
}

}  // namespace jam