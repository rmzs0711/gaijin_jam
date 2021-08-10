

#include "characters.h"
#include "Level.h"

void Monster::isFighting() {
    sf::Clock clock;
    std::shared_ptr<TemplateCharacter> hero =
        intersectionObjects(character, curLevel.heroes);
    if (hero != nullptr) {
        current_frame =
            (current_frame +
             static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
            quantity_frames;
        changeState(FIGHTING, (*hero).getDamage());
    } else {
        changeState(NOT_FIGHTING);
    }
}
void Monster::changeState(int state_, float damage_) {
    sf::Clock clock;
    if (state_ == NOT_FIGHTING) {
        character.setColor(sf::Color::White);
        state = state_;
        speedCoef = 1;
    } else if (state_ == FIGHTING && state != FROZEN && state != STUNNED) {
        character.setColor(sf::Color::White);
        state = FIGHTING;
        character.setTextureRect(
            sf::IntRect((current_frame % (quantity_frames)) * size_frame.x,
                        state_ * size_frame.y, size_frame.x, size_frame.y));
        health -= damage_;
        return;
    } else if (state_ == FROZEN) {
        speedCoef = 0;
        state = state_;
        health -= damage_;
        character.setColor(sf::Color::Blue);
    } else if (state_ == BURNED) {
        state = state_;
        character.setColor(sf::Color::Red);
        health -= jam::fireDamage;
        health -= damage_;
    } else if (state_ == SLOWED) {
        speedCoef = 0.5;
        state = state_;
        health -= damage_;
        character.setColor(sf::Color(100, 100, 100));
        // TODO speed * slow_coef
    } else if (state_ == STUNNED) {
        speedCoef = 0;
        state = state_;
        health -= damage_;
        health -= jam::earthShakeDamage;
    } else {
        float dx, dy;
        initializingCoordinates(dx, dy, state_);
        character.move(speedCoef * dx, speedCoef * dy);
        bool isRock = false;
        auto hitBox = character.getGlobalBounds();
        for (auto &i : curLevel.getfreeObjects()) {
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
        state = state_;
        character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                             state_ * size_frame.y,
                                             size_frame.x, size_frame.y));
    }
}
void Monster::isEffected() {
    float damage_ = 0;

    sf::Vector2i cell =
        sf::Vector2i(character.getPosition() / (float)jam::cellSize);
    switch (curLevel.getMap()[cell.y][cell.x].getState()) {
        case jam::NORMAL:
            changeState(NOT_FIGHTING);
            break;
        case jam::LAVA:
        case jam::BLAST:
            changeState(BURNED, damage_);
            break;
        case jam::FROZEN_BLAST:
            changeState(FROZEN, damage_);
            break;
        case jam::CLOUD:
            changeState(SLOWED, damage_);
            break;
        case jam::EARTHSHAKE:
            changeState(STUNNED, damage_);
            break;
        case jam::WALL:
        case jam::NUMBER_OF_STATES:
            break;
    }
}
void Monster::drawCharacter(sf::RenderWindow &window) {
    if (isDraw()) {
        if (isLive()) {
            if (state != STUNNED && state != FROZEN) {
                isFighting();
            }

            if (state == FIGHTING) {
                if (positions[positions.size() - 1] !=
                    character.getPosition()) {
                    positions.push_back(character.getPosition());
                }
            } else if (!positions.empty() && positions[positions.size() - 1] ==
                                                 character.getPosition()) {
                positions.pop_back();
            }
            if (state != STUNNED && state != FROZEN) {
                moveToPosition();
            }
            isEffected();
            // moving
        } else {
            death();
        }
        window.draw(character);
    }
}
void Monster::takeDamage(float damage_) {
    health -= damage_;
}
int Monster::getState() const {
    return state;
}
void Monster::moveToPosition() {
    if (positions.size() != 0) {
        sf::Clock clock;
        float dx =
            (positions[positions.size() - 1] - character.getPosition()).x;
        float dy =
            (positions[positions.size() - 1] - character.getPosition()).y;
        if (abs(dx) >= speed || abs(dy) >= speed) {
            current_frame =
                (current_frame +
                 static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
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
            positions.pop_back();
        }
    }
}
void Monster::death() {
    health = current_health * 4;
}

void Hero::isFighting() {
    sf::Clock clock;
    std::shared_ptr<TemplateCharacter> monster =
        intersectionObjects(character, curLevel.getMonsters());
    if (monster != nullptr) {
        position = character.getPosition();
        current_frame =
            (current_frame +
             static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
            quantity_frames;
        float damage_ = 0;
        sf::Vector2i cell = sf::Vector2i(monster->getSprite()->getPosition() /
                                         (float)jam::cellSize);
        if (curLevel.getMap()[cell.y][cell.x].getState() != jam::FROZEN_BLAST) {
            damage_ = monster->getDamage();
        }
        changeState(FIGHTING, damage_);
    } else {
        changeState(NOT_FIGHTING);
    }
}
void Hero::clickMouse(const sf::Event &event,
                      sf::RenderWindow &window,
                      const sf::Time &currentTime) {
    if (readyToCast && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i selectedCell = {-1, -1};
        for (int i = 0; i < curLevel.getMap().size() && selectedCell.x == -1;
             i++) {
            for (int j = 0; j < curLevel.getMap()[i].size(); j++) {
                if (curLevel.getMap()[i][j].getGlobalBounds().contains(
                        window.mapPixelToCoords(
                            sf::Mouse::getPosition(window)))) {
                    selectedCell = {i, j};
                    break;
                }
            }
        }
        if (selectedCell.x == -1) {
            assert(0);
        }
        switch (ability) {
            case FIRE_BLAST:
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        curLevel
                            .getMap()[bounds(selectedCell.x + i, 0,
                                             (int)curLevel.getMap().size())]
                                     [bounds(selectedCell.y + j, 0,
                                             (int)curLevel.getMap()[0].size())]
                            .setState(jam::BLAST, currentTime);
                        curLevel.freeObjects.push_back(jam::makeFire(
                            sf::Vector2f((float)bounds(
                                             selectedCell.y + j, 0,
                                             (int)curLevel.getMap()[0].size()) *
                                                 jam::cellSize +
                                             jam::cellSize / 2,
                                         bounds(selectedCell.x + i, 0,
                                                (int)curLevel.getMap().size()) *
                                                 jam::cellSize +
                                             jam::cellSize / 2)));
                    }
                }
                break;
            case CLOUD:
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        curLevel
                            .getMap()[bounds(selectedCell.x + i, 0,
                                             (int)curLevel.getMap().size())]
                                     [bounds(selectedCell.y + j, 0,
                                             (int)curLevel.getMap()[0].size())]
                            .setState(jam::CLOUD, currentTime);
                    }
                }
                break;
            case LAVA:
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        curLevel
                            .map[bounds(selectedCell.x + i, 0,
                                        (int)curLevel.getMap().size())]
                                [bounds(selectedCell.y + j, 0,
                                        (int)curLevel.getMap()[0].size())]
                            .setState(jam::LAVA, currentTime);
                    }
                }
                break;
            case FROZEN_BLAST:
                for (int i = -2; i < 3; i++) {
                    for (int j = -2; j < 3; j++) {
                        if (std::abs(j) + std::abs(i) >= 3) {
                            continue;
                        }
                        curLevel
                            .map[bounds(selectedCell.x + i, 0,
                                        (int)curLevel.getMap().size())]
                                [bounds(selectedCell.y + j, 0,
                                        (int)curLevel.map[0].size())]
                            .setState(jam::FROZEN_BLAST, currentTime);
                    }
                }
                break;
            case EARTHSHAKE:
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        if (curLevel
                                .map[bounds(selectedCell.x + i, 0,
                                            (int)curLevel.map.size())]
                                    [bounds(selectedCell.y + j, 0,
                                            (int)curLevel.map[0].size())]
                                .getGlobalBounds()
                                .contains(character.getPosition())) {
                            continue;
                        }
                        curLevel
                            .map[bounds(selectedCell.x + i, 0,
                                        (int)curLevel.map.size())]
                                [bounds(selectedCell.y + j, 0,
                                        (int)curLevel.map[0].size())]
                            .setState(jam::EARTHSHAKE, currentTime);
                    }
                }
                break;
            case WALL:
                curLevel.map[selectedCell.x][selectedCell.y].setState(
                    jam::WALL, currentTime);
                curLevel.freeObjects.push_back(jam::makeRock(sf::Vector2f(
                    selectedCell.y * jam::cellSize + jam::cellSize / 2,
                    selectedCell.x * jam::cellSize + jam::cellSize / 2)));
                break;
        }
        readyToCast = false;
        return;
    }
}
void Hero::changeState(int state_, float damage_) {
    sf::Clock clock;
    if (state_ == FIGHTING) {
        state_ = 4 + 2 * state;
        character.setTextureRect(sf::IntRect(
            (current_frame % (quantity_frames - 1)) * size_frame.x,
            (state_ +
             (static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 2)) *
                size_frame.y,
            size_frame.x, size_frame.y));
        health -= damage_;
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
    for (auto &i : curLevel.getfreeObjects()) {
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
    while (!isCorrectMove()) {
        character.move(-dx, -dy);
        state_ = (state_ +
                  static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
                 4;
        initializingCoordinates(dx, dy, state_);
        dx *= 2.5, dy *= 2.5;
        character.move(dx, dy);
    }
    state = state_;
    character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                         state_ * size_frame.y, size_frame.x,
                                         size_frame.y));
}
void Hero::moveToPosition() {
    sf::Clock clock;
    float dx = (position - character.getPosition()).x;
    float dy = (position - character.getPosition()).y;
    if (abs(dx) >= speed || abs(dy) >= speed) {
        current_frame =
            (current_frame +
             static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
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
                 sf::RenderWindow &window,
                 const sf::Time &currentTime) {
    if (isLive()) {
        if (event.type == sf::Event::MouseButtonPressed) {
            clickMouse(event, window, currentTime);
            return;
        } else if (event.type == sf::Event::KeyPressed) {
            keyPressed(event);
        }
    }
}
void Hero::drawCharacter(sf::RenderWindow &window) {
    if (isDraw()) {
        if (isLive()) {
            moveToPosition();
            isFighting();
        }
        death();
        window.draw(character);
        if (isLive() && is_move) {
            icon.setPosition(character.getPosition() -
                             sf::Vector2f(0, size_frame.y * scale.y));
            window.draw(icon);
        }
    }
}
void Hero::death() {
    if (health <= 0) {
        health = current_health + 1;
        sf::Clock clock;
        sf::Image character_image;
        character_image.loadFromFile(
            "data/images/MiniWorldSprites/Miscellaneous/Tombstones.png");
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        character.setTextureRect(sf::IntRect(
            static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4,
            static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 2,
            size_frame.x, size_frame.y));
    } else if (health > current_health) {
        health++;
    }
}
void Hero::keyPressed(const sf::Event &event) {
    if (event.key.code == sf::Keyboard::R) {
        int whatAbility = (1 << elements[0]) | (1 << elements[1]);
        if (whatAbility == 4) {  // 100
            ability = EARTHSHAKE;
        } else if (whatAbility == 5) {  // 101
            ability = LAVA;
        } else if (whatAbility == 6) {  // 110
            ability = WALL;
        } else if (whatAbility == 2) {  // 010
            ability = FROZEN_BLAST;
        } else if (whatAbility == 3) {  // 011
            ability = CLOUD;
        } else if (whatAbility == 1) {  // 001
            ability = FIRE_BLAST;
        } else {
            assert(0);
        }
        return;
    }
    if (event.key.code == sf::Keyboard::E) {
        readyToCast = true;
        return;
    }
    elements[0] = elements[1];
    if (event.key.code == sf::Keyboard::Z) {
        elements[1] = POWER_ELEMENT::FIRE;
    }
    if (event.key.code == sf::Keyboard::X) {
        elements[1] = POWER_ELEMENT::ICE;
    }
    if (event.key.code == sf::Keyboard::C) {
        elements[1] = POWER_ELEMENT::EARTH;
    }
}
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
}
void TemplateCharacter::setScale(float x, float y) {
    character.scale(x, y);
    icon.scale(x, y);
    scale = sf::Vector2f(x, y);
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
sf::Sprite *TemplateCharacter::getSprite() {
    return &character;
}
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
bool TemplateCharacter::isCorrectMove() {
    auto hitBox = character.getGlobalBounds();
    for (auto &i : curLevel.freeObjects) {
        if (i.getHitBox().intersects({hitBox.left,
                                      hitBox.top + hitBox.height / 2,
                                      hitBox.width, hitBox.height / 2})) {
            return false;
        }
    }
    return true;
}

std::shared_ptr<Monster> Monster::makeArmouredRedDemon(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Demons/"
        "ArmouredRedDemon.png",
        50, 0.1, monster_path, level, 5);
    (*monster).setScale(3, 3);
    return monster;
}

std::shared_ptr<Monster> Monster::makeRedDemon(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Demons/RedDemon.png",
        50, 0.1, monster_path, level, 5);
    (*monster).setScale(3, 3);
    return monster;
}

std::shared_ptr<Monster> Monster::makePurpleDemon(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Demons/"
        "PurpleDemon.png",
        50, 0.1, monster_path, level, 5);
    (*monster).setScale(3, 3);
    return monster;
}

// makeFrostborn

std::shared_ptr<Monster> Monster::makeMammoth(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/"
        "Mammoth.png",
        70, 0.07, monster_path, level, 4);
    (*monster).setScale(4.3, 4.3);
    return monster;
}

std::shared_ptr<Monster> Monster::makeWendigo(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/"
        "Wendigo.png",
        70, 0.07, monster_path, level, 4);
    (*monster).setScale(4.3, 4.3);
    return monster;
}

std::shared_ptr<Monster> Monster::makeYeti(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Frostborn/Yeti.png",
        70, 0.07, monster_path, level, 5);
    (*monster).setScale(4.3, 4.3);
    return monster;
}

// makeOrcs

std::shared_ptr<Monster> Monster::makeArcherGoblin(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
        "ArcherGoblin.png",
        40, 0.13, monster_path, level, 4);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> Monster::makeClubGoblin(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/ClubGoblin.png",
        40, 0.13, monster_path, level, 4);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> Monster::makeFarmerGoblin(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
        "FarmerGoblin.png",
        40, 0.13, monster_path, level, 4);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> Monster::makeKamikazeGoblin(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/"
        "KamikazeGoblin.png",
        40, 0.13, monster_path, level, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> Monster::makeOrc(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/Orc.png", 40,
        0.13, monster_path, level, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> Monster::makeOrcMage(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/OrcMage.png", 40,
        0.13, monster_path, level, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

std::shared_ptr<Monster> Monster::makeOrcShaman(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Orcs/OrcShaman.png",
        40, 0.13, monster_path, level, 5);
    (*monster).setScale(2.5, 2.5);
    return monster;
}

// makePirates

std::shared_ptr<Monster> Monster::makePirateCaptain(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
        "PirateCaptain.png",
        80, 0.15, monster_path, level, 5);
    (*monster).setScale(3.8, 3.8);
    return monster;
}

std::shared_ptr<Monster> Monster::makePirateGrunt(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
        "PirateGrunt.png",
        80, 0.15, monster_path, level, 5);
    (*monster).setScale(3.8, 3.8);
    return monster;
}

std::shared_ptr<Monster> Monster::makePirateGunnern(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Pirates/"
        "PirateGunner.png",
        80, 0.15, monster_path, level, 5);
    (*monster).setScale(3.8, 3.8);
    return monster;
}

// makeUndead

std::shared_ptr<Monster> Monster::makeNecromancer(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Undead/"
        "Necromancer.png",
        35, 0.15, monster_path, level, 5);
    (*monster).setScale(3, 3);
    return monster;
}

std::shared_ptr<Monster> Monster::makeSkeletonSoldier(
    jam::Level &level,
    std::vector<sf::Vector2f> &monster_path) {
    std::shared_ptr<Monster> monster = std::make_shared<Monster>(
        "data/images/MiniWorldSprites/Characters/Monsters/Undead/"
        "Skeleton-Soldier.png",
        35, 0.15, monster_path, level, 5);
    (*monster).setScale(3, 3);
    return monster;
}

// makeHero

std::shared_ptr<Hero> Hero::makeAssasinPurple(jam::Level &level,
                                        sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/"
        "AssasinPurple.png",
        100, 0.1, level, false, 5);
    (*monster).setScale(4, 4);
    (*monster).setSpeed(0.3);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> Hero::makeAssasinLime(jam::Level &level,
                                      sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/"
        "AssasinLime.png",
        120, 0.12, level, false, 5);
    (*monster).setScale(4.2, 4.2);
    (*monster).setSpeed(0.13);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> Hero::makeAssasinCyan(jam::Level &level,
                                      sf::Vector2f position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/"
        "AssasinCyan.png",
        110, 0.8, level, false, 5);
    (*monster).setScale(4.1, 4.1);
    (*monster).setSpeed(0.22);
    (*monster).setPosition(position);
    return monster;
}

std::shared_ptr<Hero> Hero::makeAssasinRed(jam::Level &level, sf::Vector2f
                                                                  position) {
    std::shared_ptr<Hero> monster = std::make_shared<Hero>(
        "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/"
        "AssasinRed.png",
        90, 0.14, level, false, 5);
    (*monster).setScale(3.9, 3.9);
    (*monster).setSpeed(0.35);
    (*monster).setPosition(position);
    return monster;
}
