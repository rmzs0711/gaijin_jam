#pragma once

#include <cmath>
#include <list>
#include <vector>
#include "Level.h"
#include "SFML/Graphics.hpp"
#include "makeFreeObjects.h"
#include "usefulFunctions.h"

int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2, FIGHTING = 4,
          NOT_FIGHTING = -1;
int const BURNED = -2, FROZEN = -3, SLOWED = -4, STUNNED = -5;
enum POWER_ELEMENT { FIRE, ICE, EARTH, NUMBER_OF_POWER_ELEMENTS };
enum ABILITY { FIRE_BLAST, CLOUD, LAVA, FROZEN_BLAST, WALL, EARTHSHAKE };

bool isCorrectMove(const sf::Sprite &character,
                   const std::list<jam::FreeObject> &objects) {
    auto hitBox = character.getGlobalBounds();
    for (auto &i : objects) {
        if (i.getHitBox().intersects({hitBox.left,
                                      hitBox.top + hitBox.height / 2,
                                      hitBox.width, hitBox.height / 2})) {
            return false;
        }
    }
    return true;
}

struct TemplateCharacter {
protected:
    float speedCoef = 1;
    float speed;
    sf::Vector2f scale;
    int current_frame, quantity_frames;
    sf::Vector2i size_frame;
    float health, damage, current_health, current_damage;
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon;
    std::vector<std::vector<jam::Cell>> &map;
    std::list<jam::FreeObject> &objects;

    TemplateCharacter(const std::string &file_name,
                      int quantity_frames_,
                      sf::Vector2i size_frame_,
                      float health_,
                      float damage_,
                      std::vector<std::vector<jam::Cell>> &map_,
                      std::list<jam::FreeObject> &objects_)
        : map(map_),
          objects(objects_),
          current_frame(0),
          size_frame(size_frame_),
          quantity_frames(quantity_frames_),
          speed(0.2),
          current_health(health_),
          current_damage(damage_),
          health(health_),
          damage(damage_),
          scale(sf::Vector2f(0, 0)) {
        sf::Clock clock;
        sf::Image character_image;
        character_image.loadFromFile(file_name);
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        character.setOrigin((float)size_frame.x / 2,
                            (float)size_frame.y * 0.9f);
        sf::Image icon_image;
        icon_image.loadFromFile(
            "data/images/MiniWorldSprites/Objects/FireballProjectile.png");
        icon_texture.loadFromImage(icon_image);
        icon.setTexture(icon_texture);
        icon.setOrigin(character.getOrigin());

        character.setTextureRect(sf::IntRect(0, 0, size_frame.x, size_frame.y));
        icon.setTextureRect(sf::IntRect(
            (static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4) *
                size_frame.x,
            0, size_frame.x, size_frame.y));
    }

    void initializingCoordinates(float &dx, float &dy, int direction) {
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

public:
    std::list<jam::FreeObject> &getObjects() const {
        return objects;
    }
    sf::Sprite *getSprite() {
        return &character;
    }

    virtual void setPosition(float x, float y) {
        character.setPosition(x, y);
    }

    virtual void setPosition(sf::Vector2f position_) {
        character.setPosition(position_);
    }

    void setSpeed(float speed_) {
        speed = speed_;
    }

    void setScale(float x, float y) {
        character.scale(x, y);
        icon.scale(x, y);
        scale = sf::Vector2f(x, y);
    }

    void setScale(sf::Vector2f scale_) {
        character.scale(scale_);
        icon.scale(scale_);
        scale = scale_;
    }

    float getDamage() {
        if (isLive()) {
            return damage;
        }
        return 0;
    }

    bool isLive() {
        if (health <= 0 || health > current_health) {
            return false;
        }
        return true;
    }

    bool isDraw() {
        if (health >= current_health * 4) {
            return false;
        }
        return true;
    }

    virtual void drawCharacter(
        sf::RenderWindow &window,
        std::vector<std::shared_ptr<TemplateCharacter>> &) = 0;
};

std::shared_ptr<TemplateCharacter> intersectionObjects(
    const sf::Sprite &character,
    std::vector<std::shared_ptr<TemplateCharacter>> &objects) {
    for (auto &i : objects) {
        if (((*i).getSprite())
                ->getGlobalBounds()
                .intersects(character.getGlobalBounds()) &&
            (*i).getSprite() != &character) {
            return i;
        }
    }
    return nullptr;
}

struct Monster : TemplateCharacter {
protected:
    int state;
    std::vector<sf::Vector2f> positions;

    void changeState(int state_, float damage_ = 0) {
        sf::Clock clock;
        if (state_ == NOT_FIGHTING) {
            character.setColor(sf::Color::White);
        } else if (state == FIGHTING && state != FROZEN && state != STUNNED) {
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
            for (auto &i : objects) {
                if (i.getObjectType() == jam::ROCK &&
                    i.getHitBox().intersects(
                        {hitBox.left, hitBox.top + hitBox.height / 2,
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

    void isFighting(std::vector<std::shared_ptr<TemplateCharacter>> &heroes) {
        sf::Clock clock;
        std::shared_ptr<TemplateCharacter> hero =
            intersectionObjects(character, heroes);
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

    void death() {
        health = current_health * 4;
    }
    void isEffected(std::vector<std::shared_ptr<TemplateCharacter>> &heroes) {
        std::shared_ptr<TemplateCharacter> hero =
            intersectionObjects(character, heroes);
        float damage_ = 0;
        if (hero) {
            damage_ = hero->getDamage();
        }
        sf::Vector2i cell =
            sf::Vector2i(character.getPosition() / (float)jam::cellSize);
        switch (map[cell.y][cell.x].getState()) {
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

    void moveToPosition() {
        if (positions.size() != 0) {
            sf::Clock clock;
            float dx =
                (positions[positions.size() - 1] - character.getPosition()).x;
            float dy =
                (positions[positions.size() - 1] - character.getPosition()).y;
            if (abs(dx) >= speed || abs(dy) >= speed) {
                current_frame = (current_frame +
                                 static_cast<int>(
                                     clock.getElapsedTime().asMicroseconds())) %
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

public:
    Monster(const std::string &file_name,
            float health_,
            float damage_,
            std::vector<sf::Vector2f> &positions_,
            std::vector<std::vector<jam::Cell>> &map_,
            std::list<jam::FreeObject> &objects_,
            int quantity_frames_ = 4,
            sf::Vector2i size_frame_ = sf::Vector2i(16, 16))
        : TemplateCharacter(file_name,
                            quantity_frames_,
                            size_frame_,
                            health_,
                            damage_,
                            map_,
                            objects_),
          state(DOWN),
          positions(positions_) {}

    int getState() const {
        return state;
    }

    void takeDamage(float damage_) {
        health -= damage_;
    }

    void drawCharacter(
        sf::RenderWindow &window,
        std::vector<std::shared_ptr<TemplateCharacter>> &heroes) override {
        if (isDraw()) {
            if (isLive()) {
                if (state != STUNNED && state != FROZEN) {
                    isFighting(heroes);
                }

                if (state == FIGHTING) {
                    if (positions[positions.size() - 1] !=
                        character.getPosition()) {
                        positions.push_back(character.getPosition());
                    }
                } else if (!positions.empty() &&
                           positions[positions.size() - 1] ==
                               character.getPosition()) {
                    positions.pop_back();
                }
                if (state != STUNNED && state != FROZEN) {
                    moveToPosition();
                }
                isEffected(heroes);
                // moving
            } else {
                death();
            }
            window.draw(character);
        }
    }
};

struct Hero : TemplateCharacter {
protected:
    //  std::vector<std::unique_ptr<TemplateCharacter>> &monsters;
    std::vector<POWER_ELEMENT> elements;
    ABILITY ability;
    bool readyToCast = false;

    int state;
    bool is_move, is_always_move;
    sf::Vector2f position;

    bool isCorrectClick(const sf::Vector2f &mouse) {
        return character.getGlobalBounds().contains(mouse);
    }

    void isFighting(std::vector<std::shared_ptr<TemplateCharacter>> &monsters) {
        sf::Clock clock;
        std::shared_ptr<TemplateCharacter> monster =
            intersectionObjects(character, monsters);
        if (monster != nullptr) {
            position = character.getPosition();
            current_frame =
                (current_frame +
                 static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
                quantity_frames;
            float damage_ = 0;
            sf::Vector2i cell = sf::Vector2i(
                monster->getSprite()->getPosition() / (float)jam::cellSize);
            if (map[cell.y][cell.x].getState() != jam::FROZEN_BLAST) {
                damage_ = monster->getDamage();
            }
            changeState(FIGHTING, damage_);
        } else {
            changeState(NOT_FIGHTING);
        }
    }

    void keyPressed(const sf::Event &event) {
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

    void death() {
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

    void clickMouse(const sf::Event &event,
                    sf::RenderWindow &window,
                    const sf::Time &currentTime) {
        if (readyToCast && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i selectedCell = {-1, -1};
            for (int i = 0; i < map.size() && selectedCell.x == -1; i++) {
                for (int j = 0; j < map[i].size(); j++) {
                    if (map[i][j].getGlobalBounds().contains(
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
                            map[bounds(selectedCell.x + i, 0, (int)map.size())]
                               [bounds(selectedCell.y + j, 0,
                                       (int)map[0].size())]
                                   .setState(jam::BLAST, currentTime);
                            objects.push_back(jam::makeFire(sf::Vector2f(
                                bounds(selectedCell.y + j, 0,
                                       (int)map[0].size()) *
                                        jam::cellSize +
                                    jam::cellSize / 2,
                                bounds(selectedCell.x + i, 0, (int)map.size()) *
                                        jam::cellSize +
                                    jam::cellSize / 2)));
                        }
                    }
                    break;
                case CLOUD:
                    for (int i = -1; i < 2; i++) {
                        for (int j = -1; j < 2; j++) {
                            map[bounds(selectedCell.x + i, 0, (int)map.size())]
                               [bounds(selectedCell.y + j, 0,
                                       (int)map[0].size())]
                                   .setState(jam::CLOUD, currentTime);
                        }
                    }
                    break;
                case LAVA:
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                            map[bounds(selectedCell.x + i, 0, (int)map.size())]
                               [bounds(selectedCell.y + j, 0,
                                       (int)map[0].size())]
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
                            map[bounds(selectedCell.x + i, 0, (int)map.size())]
                               [bounds(selectedCell.y + j, 0,
                                       (int)map[0].size())]
                                   .setState(jam::FROZEN_BLAST, currentTime);
                        }
                    }
                    break;
                case EARTHSHAKE:
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                            if (map[bounds(selectedCell.x + i, 0,
                                           (int)map.size())]
                                   [bounds(selectedCell.y + j, 0,
                                           (int)map[0].size())]
                                       .getGlobalBounds()
                                       .contains(character.getPosition())) {
                                continue;
                            }
                            map[bounds(selectedCell.x + i, 0, (int)map.size())]
                               [bounds(selectedCell.y + j, 0,
                                       (int)map[0].size())]
                                   .setState(jam::EARTHSHAKE, currentTime);
                        }
                    }
                    break;
                case WALL:
                    map[selectedCell.x][selectedCell.y].setState(jam::WALL,
                                                                 currentTime);
                    objects.push_back(jam::makeRock(sf::Vector2f(
                        selectedCell.y * jam::cellSize + jam::cellSize / 2,
                        selectedCell.x * jam::cellSize + jam::cellSize / 2)));
                    break;
            }
            readyToCast = false;
            return;
        }

        if (!is_always_move && event.mouseButton.button == sf::Mouse::Left) {
            if (isCorrectClick(window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
                is_move = true;
            } else {
                is_move = false;
            }
        }

        if (is_move && event.mouseButton.button == sf::Mouse::Right) {
            readyToCast = false;
            position = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            auto bounds = sf::IntRect(
                sf::Vector2i(0, 0), sf::Vector2i(map[0].size() * jam::cellSize,
                                                 map.size() * jam::cellSize));
            if (!bounds.contains(window.mapCoordsToPixel(position))) {
                position = character.getPosition();
            }
        }
    }

    void changeState(int state_, float damage_ = 0) {
        sf::Clock clock;
        if (state_ == FIGHTING) {
            state_ = 4 + 2 * state;
            character.setTextureRect(sf::IntRect(
                (current_frame % (quantity_frames - 1)) * size_frame.x,
                (state_ +
                 (static_cast<int>(clock.getElapsedTime().asMicroseconds()) %
                  2)) *
                    size_frame.y,
                size_frame.x, size_frame.y));
            health -= damage_;
            return;
        }
        if (state_ == NOT_FIGHTING) {
            character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                                 state * size_frame.y,
                                                 size_frame.x, size_frame.y));
            return;
        }
        float dx, dy;
        initializingCoordinates(dx, dy, state_);
        character.move(dx, dy);
        // TODO
        bool isRock = false;
        auto hitBox = character.getGlobalBounds();
        for (auto &i : objects) {
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
        while (!isCorrectMove(character, objects)) {
            character.move(-dx, -dy);
            state_ = (state_ + static_cast<int>(
                                   clock.getElapsedTime().asMicroseconds())) %
                     4;
            initializingCoordinates(dx, dy, state_);
            dx *= 2.5, dy *= 2.5;
            character.move(dx, dy);
        }
        state = state_;
        character.setTextureRect(sf::IntRect(current_frame * size_frame.x,
                                             state_ * size_frame.y,
                                             size_frame.x, size_frame.y));
    }

    void moveToPosition() {
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

public:
    Hero(const std::string &file_name,
         float health_,
         float damage_,
         std::vector<std::vector<jam::Cell>> &map_,
         std::list<jam::FreeObject> &objects_,
         bool is_always_move_ = true,
         int quantity_frames_ = 4,
         sf::Vector2i size_frame_ = sf::Vector2i(16, 16))
        : TemplateCharacter(file_name,
                            quantity_frames_,
                            size_frame_,
                            health_,
                            damage_,
                            map_,
                            objects_),

          ability(CLOUD),
          elements(2, POWER_ELEMENT::FIRE),
          state(DOWN),
          is_always_move(is_always_move_),
          is_move(is_always_move_),
          position(sf::Vector2f(0, 0)) {}

    void setPosition(float x, float y) {
        character.setPosition(x, y);
        position = sf::Vector2f(x, y);
    }

    void setPosition(sf::Vector2f position_) {
        character.setPosition(position_);
        position = position_;
    }

    void event(const sf::Event &event,
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

    void drawCharacter(
        sf::RenderWindow &window,
        std::vector<std::shared_ptr<TemplateCharacter>> &monsters) override {
        if (isDraw()) {
            if (isLive()) {
                moveToPosition();
                isFighting(monsters);
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
};