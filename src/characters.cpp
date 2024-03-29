#ifdef _MSC_VER
#include "../include/characters.h"
#include "../include/Level.h"
#include "../include/makeBuilding.h"
#else
#include "Level.h"
#include "characters.h"
#include "makeBuilding.h"
#endif

bool TemplateCharacter::isCorrectMove() {
    auto hitBox = character.getGlobalBounds();
    {
        auto start = curLevel.freeObjects.lower_bound(
            jam::makeEmptyObject({hitBox.left, hitBox.top - jam::cellSize}));
        auto end = curLevel.freeObjects.upper_bound(
            jam::makeEmptyObject({hitBox.left, hitBox.top + jam::cellSize}));
        for (auto &i = start; i != end; i++) {
            if (i->getHitBox().intersects({hitBox.left,
                                           hitBox.top + hitBox.height / 2,
                                           hitBox.width, hitBox.height / 2})) {
                return false;
            }
        }
    }
    {
        auto start =
            curLevel.attackBuildings.lower_bound(jam::makeEmptyAttackBuilding(
                curLevel, sf::Vector2i{(int)hitBox.left / jam::cellSize - 1,
                                       (int)hitBox.top / jam::cellSize - 1}));
        auto end =
            curLevel.attackBuildings.upper_bound(jam::makeEmptyAttackBuilding(
                curLevel, sf::Vector2i((int)hitBox.left / jam::cellSize + 1,
                                       (int)hitBox.top / jam::cellSize + 1)));
        for (auto &i = start; i != end; i++) {
            if (i->getHitBox().intersects({hitBox.left,
                                           hitBox.top + hitBox.height / 2,
                                           hitBox.width, hitBox.height / 2})) {
                return false;
            }
        }
    }
    {
        auto start =
            curLevel.supportBuildings.lower_bound(jam::makeEmptySupportBuilding(
                curLevel, sf::Vector2i{(int)hitBox.left / jam::cellSize - 1,
                                       (int)hitBox.top / jam::cellSize - 1}));
        auto end =
            curLevel.supportBuildings.upper_bound(jam::makeEmptySupportBuilding(
                curLevel, sf::Vector2i((int)hitBox.left / jam::cellSize + 1,
                                       (int)hitBox.top / jam::cellSize + 1)));
        for (auto &i = start; i != end; i++) {
            if (i->getHitBox().intersects({hitBox.left,
                                           hitBox.top + hitBox.height / 2,
                                           hitBox.width, hitBox.height / 2})) {
                return false;
            }
        }
    }
    for (auto &i : curLevel.home) {
        if (i.getHitBox().intersects({hitBox.left,
                                      hitBox.top + hitBox.height / 2,
                                      hitBox.width, hitBox.height / 2})) {
            return false;
        }
    }

    return true;
}

bool Monster::isCorrectMove() {
    auto hitBox = character.getGlobalBounds();
    for (auto& i : curLevel.home) {
        if (i.getHitBox().intersects({ hitBox.left,
                                      hitBox.top + hitBox.height / 2,
                                      hitBox.width, hitBox.height / 2 })) {
                if (--curLevel.health == 0) {
                    curLevel.is_end = true;
                }
                else {
                    death();
                }
            return false;
        }
    }

    return true;
}

void Monster::isFighting() {
    std::shared_ptr<TemplateCharacter> hero =
        intersectionObjects(character, curLevel.heroes, curLevel);
    if (hero != nullptr) {
        current_frame =
            (current_frame + rand()) %
            quantity_frames;
        changeState(FIGHTING, hero);
    } else {
        changeState(NOT_FIGHTING);
    }
}
void Monster::changeState(int state_,
                          std::shared_ptr<TemplateCharacter> enemy) {
    if (state_ == NOT_FIGHTING) {
        character.setColor(sf::Color::White);
        state = state_;
    } else if (state_ == FIGHTING && state != FROZEN && state != STUNNED) {
        character.setColor(sf::Color::White);
        state = FIGHTING;
        character.setTextureRect(
            sf::IntRect((current_frame % (quantity_frames)) * size_frame.x,
                        state_ * size_frame.y, size_frame.x, size_frame.y));
        enemy ? enemy->health -= damage : 0;
        return;
    } else if (state_ == FROZEN) {
        speedCoef = 0;
        state = state_;
        character.setColor(sf::Color::Blue);
    } else if (state_ == BURNED) {
        state = state_;
        character.setColor(sf::Color::Red);
        health -= jam::fireDamage;
        enemy ? enemy->health -= damage : 0;
    } else if (state_ == SLOWED) {
        speedCoef = 0.5;
        damage = current_damage * 0.5;
        state = state_;
        enemy ? enemy->health -= damage : 0;
        character.setColor(sf::Color(100, 100, 100));
    } else if (state_ == STUNNED) {
        speedCoef = 0;
        std::shared_ptr<TemplateCharacter> hero =
            intersectionObjects(character, curLevel.heroes, curLevel);
        state = state_;
        health -= jam::earthShakeDamage;
    } else {
        float dx, dy;
        initializingCoordinates(dx, dy, state_);
        character.move(speedCoef * dx, speedCoef * dy);
        bool isRock = false;
        auto hitBox = character.getGlobalBounds();
        for (auto &i : curLevel.getFreeObjects()) {
            if (i.getObjectType() == jam::ROCK &&
                i.getHitBox().intersects({hitBox.left,
                                          hitBox.top + hitBox.height / 2,
                                          hitBox.width, hitBox.height / 2})) {
                isRock = true;
            }
        }
        if (isRock) {
            character.move(-dx, -dy);
            return;
        }

        int t = 0;
        while (!isCorrectMove() && t < 1000) {
            character.move(-dx, -dy);
            state_ = (state_ + rand()) % 4;
            initializingCoordinates(dx, dy, state_);
            dx *= 4, dy *= 4;
            character.move(dx, dy);
            t++;
        }
        state = state_;
        character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                             state_ * size_frame.y,
                                             size_frame.x, size_frame.y));
    }
}
void Monster::isEffected() {
    sf::Vector2i cell =
        sf::Vector2i(character.getPosition() / (float)jam::cellSize);
    switch (curLevel.getMap()[cell.y][cell.x].getState()) {
        case jam::NORMAL:
            changeState(NOT_FIGHTING);
            break;
        case jam::LAVA:
        case jam::BLAST:
            changeState(BURNED);
            break;
        case jam::FROZEN_BLAST:
            changeState(FROZEN);
            break;
        case jam::CLOUD:
            changeState(SLOWED);
            break;
        case jam::EARTHSHAKE:
        case jam::WALL:
            changeState(STUNNED);
            break;
        case jam::NUMBER_OF_STATES:
            break;
    }
}
void Monster::drawCharacter(sf::RenderTarget &window) {
    window.draw(character);
    life_bar.draw(window, health, character.getPosition());
}
void TemplateCharacter::takeDamage(float damage_) {
    health -= damage_;
    health = std::min(health, max_health);
}
int Monster::getState() const {
    return state;
}
void Monster::moveToPosition() {
    if (positions.size() != 0) {
        sf::Vector2f pos = character.getPosition();
        float dx =
            (positions[positions.size() - 1] - character.getPosition()).x;
        float dy =
            (positions[positions.size() - 1] - character.getPosition()).y;
        if (abs(dx) >= speed || abs(dy) >= speed) {
            current_frame =
                (current_frame + rand()) %
                quantity_frames;
            if (abs(dx) > abs(dy)) {
                if (dx > 0) {
                    changeState(RIGHT);
                } else {
                    changeState(LEFT);
                }
            } else {
                if (dy > 0) {
                    changeState(DOWN);
                } else {
                    changeState(UP);
                }
            }
        } else {
            if (positions.size() > 1) {
                positions.pop_back();
            }
        }
        if (!isCorrectMove()) {
            character.setPosition(pos);
            positions.pop_back();
            moveToPosition();
        }
    }
}
void Monster::death() {
    health = current_health * 4;
}

void Hero::isFighting() {
    std::shared_ptr<TemplateCharacter> monster =
        intersectionObjects(character, curLevel.getMonsters(), curLevel);
    if (monster != nullptr) {
        position = character.getPosition();
        current_frame =
            (current_frame + rand()) %
            quantity_frames;
        changeState(FIGHTING, monster);
    } else {
        changeState(NOT_FIGHTING, monster);
    }
}
void Hero::clickMouse(const sf::Event &event,
                      sf::RenderTarget &window,
                      const sf::Time &currentTime) {
    if (!is_always_move && event.mouseButton.button == sf::Mouse::Left) {
        if (isCorrectClick(window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
            is_move = true;
        } else {
            is_move = false;
        }
    }

    if (is_move && event.mouseButton.button == sf::Mouse::Right) {
        position = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        auto bounds = sf::IntRect(
            sf::Vector2i(0, 0),
            sf::Vector2i((int)curLevel.map[0].size() * jam::cellSize,
                         (int)curLevel.map.size() * jam::cellSize));
        if (!bounds.contains(window.mapCoordsToPixel(position))) {
            position = character.getPosition();
        }
    }
}
void Hero::changeState(int state_, std::shared_ptr<TemplateCharacter> enemy) {
    if (state_ == FIGHTING) {
        state_ = 4 + 2 * state;
        character.setTextureRect(sf::IntRect(
            (current_frame % (quantity_frames - 1)) * size_frame.x,
            (state_ + (rand() % 2)) * size_frame.y,
            size_frame.x, size_frame.y));
        enemy ? enemy->health -= damage : 0;
        return;
    }
    if (state_ == NOT_FIGHTING) {
        character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                             state * size_frame.y, size_frame.x,
                                             size_frame.y));
        return;
    }
    float dx, dy;
    initializingCoordinates(dx, dy, state_);
    character.move(dx, dy);
    bool isRock = false;
    auto hitBox = character.getGlobalBounds();
    auto start = curLevel.freeObjects.lower_bound(
        jam::makeEmptyObject({hitBox.left, hitBox.top - jam::cellSize}));
    auto end = curLevel.freeObjects.upper_bound(
        jam::makeEmptyObject({hitBox.left, hitBox.top + jam::cellSize}));
    for (auto &i = start; i != end; i++) {
        if (i->getObjectType() == jam::ROCK &&
            i->getHitBox().intersects({hitBox.left,
                                       hitBox.top + hitBox.height / 2,
                                       hitBox.width, hitBox.height / 2})) {
            isRock = true;
        }
    }
    if (isRock) {
        character.move(-dx, -dy);
        return;
    }
    int t = 0;
    while (!isCorrectMove() && t < 1000) {
        character.move(-dx, -dy);
        state_ = (state_ + rand()) % 4;
        initializingCoordinates(dx, dy, state_);
        dx *= 2.5, dy *= 2.5;
        character.move(dx, dy);
        t++;
    }
    assert(t < 1000);
    state = state_;
    character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                         state_ * size_frame.y, size_frame.x,
                                         size_frame.y));
}
void Hero::moveToPosition() {
    float dx = (position - character.getPosition()).x;
    float dy = (position - character.getPosition()).y;
    if (abs(dx) >= speed || abs(dy) >= speed) {
        current_frame =
            (current_frame + rand()) % quantity_frames;
        if (abs(dx) > abs(dy)) {
            if (dx > 0) {
                changeState(RIGHT);
            } else {
                changeState(LEFT);
            }
        } else {
            if (dy > 0) {
                changeState(DOWN);
            } else {
                changeState(UP);
            }
        }
    } else {
        auto start = std::lower_bound(
            curLevel.monsters.begin(), curLevel.monsters.end(),
            Hero::makeEmptyHero(
                curLevel, character.getPosition() -
                              2.f * sf::Vector2f{jam::cellSize, jam::cellSize}),
            charactersCompare);
        auto end = std::upper_bound(
            curLevel.monsters.begin(), curLevel.monsters.end(),
            Hero::makeEmptyHero(
                curLevel, character.getPosition() +
                              2.f * sf::Vector2f{jam::cellSize, jam::cellSize}),
            charactersCompare);
        for (auto i = start; i != end; i++) {
            auto delta =
                (*i)->character.getPosition() - character.getPosition();
            if (abs(delta.x) < 2 * jam::cellSize &&
                abs(delta.y) < 2 * jam::cellSize) {
                position = (*i)->character.getPosition();
                break;
            }
        }
    }
}
void Hero::setPosition(float x, float y) {
    character.setPosition(x, y);
    position = sf::Vector2f(x, y);
}
void Hero::setPosition(sf::Vector2f position_) {
    character.setPosition(position_);
    position = position_;
}
void Hero::event(const sf::Event &event,
                 sf::RenderTarget &window,
                 const sf::Time &currentTime) {
    if (isLive()) {
        clickMouse(event, window, currentTime);
        return;
    }
}
void Hero::drawCharacter(sf::RenderTarget &window) {
    window.draw(character);
    if (isLive()) {
        life_bar.draw(window, health, character.getPosition());
    }
    if (isLive() && is_move) {
        icon.setPosition(character.getPosition() -
                         sf::Vector2f(0, size_frame.y * scale.y + 4));
        window.draw(icon);
    }
}

void Hero::death() {
    if (health <= 0) {
        health = current_health + 1;
        sf::Image character_image;
        character_image.loadFromFile(
            "data/images/MiniWorldSprites/Miscellaneous/Tombstones.png");
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        character.setTextureRect(sf::IntRect(
            rand() % 4, rand() % 2,
            size_frame.x, size_frame.y));
    } else if (health > current_health) {
        health++;
    }
}
void Hero::keyPressed(const sf::Event &event) {}
bool Hero::isCorrectClick(const sf::Vector2f &mouse) {
    return character.getGlobalBounds().contains(mouse);
}
bool TemplateCharacter::isDraw() {
    if (health >= current_health * 4) {
        return false;
    }
    return true;
}
bool TemplateCharacter::isLive() {
    if (health <= 0 || health > current_health) {
        return false;
    }
    return true;
}
float TemplateCharacter::getDamage() {
    if (isLive()) {
        return damage;
    }
    return 0;
}
void TemplateCharacter::setScale(sf::Vector2f scale_) {
    character.scale(scale_);
    icon.scale(scale_);
    scale = scale_;
    life_bar.setScale(scale_.x);
}
void TemplateCharacter::setScale(float x, float y) {
    character.scale(x, y);
    icon.scale(x, y);
    scale = sf::Vector2f(x, y);
    life_bar.setScale(x);
}
void TemplateCharacter::setSpeed(float speed_) {
    speed = speed_;
}
void TemplateCharacter::setPosition(sf::Vector2f position_) {
    character.setPosition(position_);
}
void TemplateCharacter::setPosition(float x, float y) {
    character.setPosition(x, y);
}
//sf::Sprite *TemplateCharacter::getSprite() {
//    return &character;
//}
void TemplateCharacter::initializingCoordinates(float &dx,
                                                float &dy,
                                                int direction) const {
    if (direction == UP) {
        dx = 0, dy = -speed;
    } else if (direction == DOWN) {
        dx = 0, dy = speed;
    } else if (direction == LEFT) {
        dx = -speed, dy = 0;
    } else if (direction == RIGHT) {
        dx = speed, dy = 0;
    }
}
sf::FloatRect TemplateCharacter::getGlobalBounds() const {
    return character.getGlobalBounds();
}

std::shared_ptr<Monster> Monster::makeArmouredRedDemon(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Demons/"
        "ArmouredRedDemon.png",
        80, 0.07, monster_path, level, 4, 5);
    (*monster).setScale(4, 4);
    (*monster).setSpeed(0.32);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeRedDemon(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Demons/RedDemon.png",
        60, 0.07, monster_path, level, 3, 5);
    (*monster).setScale(4, 4);
    (*monster).setSpeed(0.4);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makePurpleDemon(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Demons/"
        "PurpleDemon.png",
        60, 0.07, monster_path, level, 3, 5);
    (*monster).setScale(4, 4);
    (*monster).setSpeed(0.4);
    (*monster).setPosition(-800, -800);
    return monster;
}

// makeFrostborn

std::shared_ptr<Monster> Monster::makeMammoth(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/"
        "Mammoth.png",
        150, 0.18, monster_path, level, 20, 4);
    (*monster).setScale(4.3, 4.3);
    (*monster).setSpeed(0.3);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeWendigo(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/"
        "Wendigo.png",
        80, 0.12, monster_path, level, 8, 4);
    (*monster).setScale(4.3, 4.3);
    (*monster).setSpeed(0.3);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeYeti(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/Yeti.png",
        70, 0.11, monster_path, level, 7, 5);
    (*monster).setScale(4.4, 4.4);
    (*monster).setSpeed(0.3);
    (*monster).setPosition(-800, -800);
    return monster;
}

// makeOrcs

std::shared_ptr<Monster> Monster::makeArcherGoblin(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
        "ArcherGoblin.png",
        50, 0.05, monster_path, level, 2, 4);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.44);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeClubGoblin(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/ClubGoblin.png",
        50, 0.05, monster_path, level, 2, 4);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.32);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeFarmerGoblin(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
        "FarmerGoblin.png",
        50, 0.05, monster_path, level, 2, 4);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.55);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeKamikazeGoblin(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
        "KamikazeGoblin.png",
        50, 0.05, monster_path, level, 2, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.44);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeOrc(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/Orc.png", 60,
        0.11, monster_path, level, 5, 5);
    (*monster).setScale(3.7, 3.7);
    (*monster).setSpeed(0.35);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeOrcMage(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/OrcMage.png", 55,
        0.17, monster_path, level, 6, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.4);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeOrcShaman(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/OrcShaman.png",
        55, 0.15, monster_path, level, 6, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.5);
    (*monster).setPosition(-800, -800);
    return monster;
}

// makePirates

std::shared_ptr<Monster> Monster::makePirateCaptain(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
        "PirateCaptain.png",
        88, 0.15, monster_path, level, 9, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.36);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makePirateGrunt(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
        "PirateGrunt.png",
        70, 0.12, monster_path, level, 8, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.3);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makePirateGunnern(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
        "PirateGunner.png",
        60, 0.12, monster_path, level, 8, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.4);
    (*monster).setPosition(-800, -800);
    return monster;
}

// makeUndead

std::shared_ptr<Monster> Monster::makeNecromancer(

    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(

        "data/images/MiniWorldSprites/Characters/Monsters/Undead/"
        "Necromancer.png",
        65, 0.23, monster_path, level, 4, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.25);
    (*monster).setPosition(-800, -800);
    return monster;
}

std::shared_ptr<Monster> Monster::makeSkeletonSoldier(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Undead/"
        "Skeleton-Soldier.png",
        50, 0.05, monster_path, level, 2, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.44);
    (*monster).setPosition(-800, -800);
    return monster;
}
void Monster::updateState() {
    if (isDraw()) {
        if (isLive()) {
            if (state != STUNNED && state != FROZEN) {
                isFighting();
            }

            if (state == FIGHTING) {
                assert(positions.size() != 0);
                if (positions[positions.size() - 1] !=
                    character.getPosition()) {
                    positions.push_back(character.getPosition());
                }
            } else if (positions.size() > 1 &&
                       positions[positions.size() - 1] ==
                           character.getPosition()) {
                positions.pop_back();
            }
            if (state != STUNNED && state != FROZEN) {
                moveToPosition();
            }
            isEffected();
            if (state != SLOWED) {
                speedCoef = 1;
                damage = current_damage;
            }
        } else {
            curLevel.addMoney(Money::makeMoney(money, character.getPosition()));
            death();
        }
    }
}

// makeHero
std::shared_ptr<Hero> Hero::makeEmptyHero(jam::Level &level,
                                          sf::Vector2f position) {
    std::shared_ptr<Hero> monster =
        std::make_shared<Hero>("", 0, 0, level, false, 0);
    (*monster).setPosition(position);
    return monster;
}
std::shared_ptr<Hero> Hero::makeAssasinPurple(jam::Level &level,
                                              sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/"
        "AssasinPurple.png",
        90, 0.12, level, false, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.4);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> Hero::makeAssasinLime(jam::Level &level,
                                            sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/"
        "AssasinLime.png",
        110, 0.1, level, false, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.4);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> Hero::makeAssasinCyan(jam::Level &level,
                                            sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(

        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/"
        "AssasinCyan.png",
        100, 0.12, level, false, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.5);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> Hero::makeAssasinRed(jam::Level &level,
                                           sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(

        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/"
        "AssasinRed.png",
        110, 0.15, level, false, 5);
    (*monster).setScale(3.5, 3.5);
    (*monster).setSpeed(0.6);
    (*monster).setPosition(position);
    return monster;
}
void Hero::updateState() {
    if (isDraw()) {
        if (isLive()) {
            moveToPosition();
            isFighting();
        }
        death();
        if (isLive() && is_move) {
            icon.setPosition(character.getPosition() -
                             sf::Vector2f(0, size_frame.y * scale.y + 4));
        }
    }
}
int Hero::getState() const {
    return state;
}

std::shared_ptr<TemplateCharacter> intersectionObjects(
    const sf::Sprite &character,
    const std::vector<std::shared_ptr<TemplateCharacter>> &objects,
    jam::Level &level) {
    auto start = std::lower_bound(
        objects.begin(), objects.end(),
        Hero::makeEmptyHero(level,
                            character.getPosition() -
                                sf::Vector2f{jam::cellSize, jam::cellSize}),
        charactersCompare);
    auto end = std::upper_bound(
        objects.begin(), objects.end(),
        Hero::makeEmptyHero(level,
                            character.getPosition() +
                                sf::Vector2f{jam::cellSize, jam::cellSize}),
        charactersCompare);
    for (auto i = start; i != end; i++) {
        if (((*i)->character.getGlobalBounds()
                .intersects(character.getGlobalBounds()))) {
            return *i;
        }
    }
    return nullptr;
}
bool charactersCompare(const std::shared_ptr<TemplateCharacter> &first,
                       const std::shared_ptr<TemplateCharacter> &second) {
    if (first->character.getPosition().y <
        second->character.getPosition().y) {
        return true;
    }
    if (first->character.getPosition().y ==
        second->character.getPosition().y) {
        return first->character.getPosition().x <
               second->character.getPosition().x;
    }
    return false;
}
