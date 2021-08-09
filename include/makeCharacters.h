#pragma once

#include "characters.h"
// makeDemon

std::shared_ptr<Monster> makeArmouredRedDemon(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Demons/"
            "ArmouredRedDemon.png",
            50, 0.1, monster_path, map, objects, 5);
    (*monster).setScale(3, 3);
    return monster;
}

std::shared_ptr<Monster> makeRedDemon(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<
        Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Demons/RedDemon.png",
        50, 0.1, monster_path, map, objects, 5);
    (*monster).setScale(3, 3);
    return monster;
}

std::shared_ptr<Monster> makePurpleDemon(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Demons/"
            "PurpleDemon.png",
            50, 0.1, monster_path, map, objects, 5);
    (*monster).setScale(3, 3);
    return monster;
}

// makeFrostborn

std::shared_ptr<Monster> makeMammoth(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/"
            "Mammoth.png",
            70, 0.07, monster_path, map, objects, 4);
    (*monster).setScale(4.3, 4.3);
    return monster;
}

std::shared_ptr<Monster> makeWendigo(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/"
            "Wendigo.png",
            70, 0.07, monster_path, map, objects, 4);
    (*monster).setScale(4.3, 4.3);
    return monster;
}

std::shared_ptr<Monster> makeYeti(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<
        Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/Yeti.png",
        70, 0.07, monster_path, map, objects, 5);
    (*monster).setScale(4.3, 4.3);
    return monster;
}

// makeOrcs

std::shared_ptr<Monster> makeArcherGoblin(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
            "ArcherGoblin.png",
            40, 0.13, monster_path, map, objects, 4);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> makeClubGoblin(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<
        Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/ClubGoblin.png",
        40, 0.13, monster_path, map, objects, 4);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> makeFarmerGoblin(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
            "FarmerGoblin.png",
            40, 0.13, monster_path, map, objects, 4);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> makeKamikazeGoblin(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
            "KamikazeGoblin.png",
            40, 0.13, monster_path, map, objects, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> makeOrc(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Orcs/Orc.png", 40,
            0.13, monster_path, map, objects, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> makeOrcMage(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Orcs/OrcMage.png",
            40, 0.13, monster_path, map, objects, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> makeOrcShaman(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<
        Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/OrcShaman.png",
        40, 0.13, monster_path, map, objects, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

// makePirates

std::shared_ptr<Monster> makePirateCaptain(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
            "PirateCaptain.png",
            80, 0.15, monster_path, map, objects, 5);
    (*monster).setScale(3.8, 3.8);
    return monster;
}

std::shared_ptr<Monster> makePirateGrunt(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
            "PirateGrunt.png",
            80, 0.15, monster_path, map, objects, 5);
    (*monster).setScale(3.8, 3.8);
    return monster;
}

std::shared_ptr<Monster> makePirateGunnern(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
            "PirateGunner.png",
            80, 0.15, monster_path, map, objects, 5);
    (*monster).setScale(3.8, 3.8);
    return monster;
}

// makeUndead

std::shared_ptr<Monster> makeNecromancer(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Undead/"
            "Necromancer.png",
            35, 0.15, monster_path, map, objects, 5);
    (*monster).setScale(3, 3);
    return monster;
}

std::shared_ptr<Monster> makeSkeletonSoldier(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster =
        std::make_shared<Monster>(
            "data/images/MiniWorldSprites/Characters/Monsters/Undead/"
            "Skeleton-Soldier.png",
            35, 0.15, monster_path, map, objects, 5);
    (*monster).setScale(3, 3);
    return monster;
}

// makeHero

std::shared_ptr<Hero> makeAssasinPurple(
    std::vector<std::vector<jam::Cell>> &map,
    std::list<jam::FreeObject> &objects, sf::Vector2f position = sf::Vector2f(0, 0)) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png",
        100, 0.1, map, objects, false, 5);
    (*monster).setScale(4, 4);
    (*monster).setSpeed(0.3);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> makeAssasinLime(std::vector<std::vector<jam::Cell>> &map,
                                      std::list<jam::FreeObject> &objects, sf::Vector2f position = sf::Vector2f(0, 0)) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/AssasinLime.png",
        120, 0.12, map, objects, false, 5);
    (*monster).setScale(4.2, 4.2);
    (*monster).setSpeed(0.13);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> makeAssasinCyan(std::vector<std::vector<jam::Cell>> &map,
                                      std::list<jam::FreeObject> &objects, sf::Vector2f position = sf::Vector2f(0, 0)) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/AssasinCyan.png",
        110, 0.8, map, objects, false, 5);
    (*monster).setScale(4.1, 4.1);
    (*monster).setSpeed(0.22);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> makeAssasinRed(std::vector<std::vector<jam::Cell>> &map,
                                     std::list<jam::FreeObject> &objects, sf::Vector2f position = sf::Vector2f(0, 0)) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/AssasinRed.png",
        90, 0.14, map, objects, false, 5);
    (*monster).setScale(3.9, 3.9);
    (*monster).setSpeed(0.35);
    (*monster).setPosition(position);
    return monster;
}