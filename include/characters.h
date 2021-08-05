#pragma once

#include <cmath>
#include <vector>
#include <list>
#include "Cell.h"
#include "SFML/Graphics.hpp"
#include "usefulFunctions.h"

int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2, FIGHTING = 4;
// int const CAST_DOWN = 4, CAST_UP = 6, CAST_RIGHT = 8, CAST_LEFT = 10;
enum POWER_ELEMENT { FIRE, ICE, EARTH, NUMBER_OF_POWER_ELEMENTS };
enum ABILITY { FIRE_BLAST, CLOUD, LAVA, FROZEN_BLAST, FROZEN_WALL, BIG_WALL };

bool isCorrectMove(const sf::Sprite& character,
    const std::vector<std::vector<jam::Cell>>& map) {
    for (auto& i : map) {
        for (auto& j : i) {
            switch (j.getObject()) {
            case jam::EMPTY:
                if (j.getGlobalBounds().intersects(
                    character.getGlobalBounds())) {
                    return false;
                }
                break;
            default:
                break;
            }
        }
    }
    return true;
}


struct TemplateCharacter {
protected:
    std::vector<std::vector<jam::Cell>>& map;
    float speed;
    sf::Vector2f scale;
    int current_frame, quantity_frames;
    sf::Vector2i size_frame;
    float health, damage, current_health, current_damage;
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon;

    TemplateCharacter(const std::string& file_name, int quantity_frames_, sf::Vector2i size_frame_, float health_, float damage_, 
        std::vector<std::vector<jam::Cell>>& map_)
        : map(map_), current_frame(0), size_frame(size_frame_), quantity_frames(quantity_frames_), speed(0.2), current_health(health_),
        current_damage(damage_), health(health_), damage(damage_), scale(sf::Vector2f(0, 0)) {

        sf::Clock clock;
        sf::Image character_image;
        character_image.loadFromFile(file_name);
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        sf::Image icon_image;
        icon_image.loadFromFile("data/images/MiniWorldSprites/Objects/FireballProjectile.png");
        icon_texture.loadFromImage(icon_image);
        icon.setTexture(icon_texture);

        character.setTextureRect(sf::IntRect(0, 0, size_frame.x, size_frame.y));
        icon.setTextureRect(sf::IntRect((static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4) * size_frame.x, 0,
            size_frame.x, size_frame.y));
    }

public:

    sf::Sprite* getSprite() {
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

  //  virtual void drawCharacter(sf::RenderWindow& window) = 0;
};

TemplateCharacter* intersectionObjects(const sf::Sprite& character, std::vector<std::unique_ptr<TemplateCharacter>>& objects) {
    for (auto &i : objects) {
        if ((*(*i).getSprite()).getGlobalBounds().intersects(character.getGlobalBounds()) && (*i).getSprite()
            != &character) {
                return i.get();
        }
    }
    return nullptr;
}



/*extern std::vector<TemplateCharacter*> heroes;

struct MonsterStanding : TemplateCharacter {
protected:
    int state;

    void changeState(int state_, float damage_ = 0) {
        sf::Clock clock;
        if (state_ == FIGHTING) {
            state = DOWN;
            state_ = 4 + state;
            state = FIGHTING;
            character.setTextureRect(sf::IntRect((current_frame % (quantity_frames)) * size_frame.x, state_ * size_frame.y,
                size_frame.x, size_frame.y));
            health -= damage_;
            return;
        }
        else if (state == FIGHTING) {
            state = DOWN;
        }
    }

    void isFighting() {
        sf::Clock clock;
        TemplateCharacter* hero = intersectionObjects(character, heroes);
        if (hero != nullptr) {
            current_frame = (current_frame + static_cast<int>(clock.getElapsedTime().asMicroseconds())) % quantity_frames;
            changeState(FIGHTING, (*hero).getDamage());
        }
        else {
            changeState(DOWN);
        }
    }

    void death() {

        health = current_health * 4;
    }

public:

    MonsterStanding(const std::string& file_name, float health_, float damage_, int quantity_frames_ = 4,
        sf::Vector2i size_frame_ = sf::Vector2i(16, 16)) : TemplateCharacter(file_name, quantity_frames_, size_frame_, health_,
            damage_), state(DOWN) {}


    void drawCharacter(sf::RenderWindow& window) {
        if (isDraw()) {
            if (isLive()) {
                // moving
                isFighting();
            }
            else {
                death();
            }
            window.draw(character);
        }
    }
};



*/

struct Hero : TemplateCharacter {
protected:
    std::vector<std::unique_ptr<TemplateCharacter>> &monsters;
    std::vector<POWER_ELEMENT> elements;
    ABILITY ability;
    bool readyToCast = false;

    int state;
    bool is_move, is_always_move;
    sf::Vector2f position;

    bool isCorrectClick(const sf::Vector2f& mouse) {
        return character.getGlobalBounds().contains(mouse);
    }

    void isFighting() {
        sf::Clock clock;
        TemplateCharacter* monster = intersectionObjects(character, monsters);
        if (monster != nullptr) {
            position = character.getPosition();
            current_frame = (current_frame + static_cast<int>(clock.getElapsedTime().asMicroseconds())) % quantity_frames;
            changeState(FIGHTING, (*monster).getDamage());
        }
    }

    void keyPressed(const sf::Event& event) {
        if (event.key.code == sf::Keyboard::R) {
            int whatAbility = (1 << elements[0]) | (1 << elements[1]);
            if (whatAbility == 4) {  // 100
                ability = BIG_WALL;
            }
            else if (whatAbility == 5) {  // 101
                ability = LAVA;
            }
            else if (whatAbility == 6) {  // 110
                ability = FROZEN_WALL;
            }
            else if (whatAbility == 2) {  // 010
                ability = FROZEN_BLAST;
            }
            else if (whatAbility == 3) {  // 011
                ability = CLOUD;
            }
            else if (whatAbility == 1) {  // 001
                ability = FIRE_BLAST;
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
            character_image.loadFromFile("data/images/MiniWorldSprites/Miscellaneous/Tombstones.png");
            character_texture.loadFromImage(character_image);
            character.setTexture(character_texture);
            character.setTextureRect(sf::IntRect(static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4,
                static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 2, size_frame.x, size_frame.y));
        }
        else if (health > current_health) {
            health++;
        }
    }

    void clickMouse(const sf::Event& event, sf::RenderWindow& window, const sf::Time &currentTime) {
        if (readyToCast && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i selectedCell = { -1, -1 };
            for (int i = 0; i < map.size() && selectedCell.x == -1; i++) {
                for (int j = 0; j < map[i].size(); j++) {
                    if (map[i][j].getGlobalBounds().contains(
                        window.mapPixelToCoords(
                            sf::Mouse::getPosition(window)))) {
                        selectedCell = { i, j };
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
                            [bounds(selectedCell.y + j, 0, (int)map.size())]
                        .setState(jam::BLAST, currentTime);
                    }
                }
                break;
            case CLOUD:
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        map[bounds(selectedCell.x + i, 0, (int)map.size())]
                            [bounds(selectedCell.y + j, 0, (int)map.size())]
                        .setState(jam::CLOUD, currentTime);
                    }
                }
                break;
            case LAVA:
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        map[bounds(selectedCell.x + i, 0, (int)map.size())]
                            [bounds(selectedCell.y + j, 0, (int)map.size())]
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
                        auto x =
                            bounds(selectedCell.x + i, 0, (int)map.size());
                        auto y =
                            bounds(selectedCell.y + j, 0, (int)map.size());
                        map[bounds(selectedCell.x + i, 0, (int)map.size())]
                            [bounds(selectedCell.y + j, 0, (int)map.size())]
                        .setState(jam::FROZEN_BLAST, currentTime);
                    }
                }
                break;
            case FROZEN_WALL:
                for (int i = 0; i < 1; i++) {
                    for (int j = 0; j < 1; j++) {
                        map[bounds(selectedCell.x + i, 0, (int)map.size())]
                            [bounds(selectedCell.y + j, 0, (int)map.size())]
                        .setState(jam::FROZEN_WALL, currentTime);
                    }
                }
                break;
            case BIG_WALL:
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        map[bounds(selectedCell.x + i, 0, (int)map.size())]
                            [bounds(selectedCell.y + j, 0, (int)map.size())]
                        .setState(jam::BIG_WALL, currentTime);
                    }
                }
                break;
            }
        }

        if (!is_always_move && event.mouseButton.button == sf::Mouse::Left) {
            if (isCorrectClick(window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
                is_move = true;
            }
            else {
                is_move = false;
            }
        }

        if (is_move && event.mouseButton.button == sf::Mouse::Right) {
            readyToCast = false;
            position = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
    }

    void initializingCoordinates(float& dx, float& dy, int direction) {
        if (direction == UP) {
            dx = 0, dy = -speed;
        }
        else if (direction == DOWN) {
            dx = 0, dy = speed;
        }
        else if (direction == LEFT) {
            dx = -speed, dy = 0;
        }
        else if (direction == RIGHT) {
            dx = speed, dy = 0;
        }
    }

    void changeState(int state_, float damage_ = 0) {
        sf::Clock clock;
        if (state_ == FIGHTING) {
            state_ = 4 + 2 * state;
            character.setTextureRect(sf::IntRect((current_frame % (quantity_frames - 1)) * size_frame.x, (state_ + (static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 2)) * size_frame.y, size_frame.x, size_frame.y));
            health -= damage_;
            return;
        }
        float dx, dy;
        initializingCoordinates(dx, dy, state_);
        character.move(dx, dy);
        while (!isCorrectMove(character, map)) {
            character.move(-dx, -dy);
            state_ = (state_ + static_cast<int>(clock.getElapsedTime().asMicroseconds())) % 4;
            initializingCoordinates(dx, dy, state_);
            dx *= 2.5, dy *= 2.5;
            character.move(dx, dy);
        }
        state = state_;
        character.setTextureRect(sf::IntRect(current_frame * size_frame.x, state_ * size_frame.y, size_frame.x, size_frame.y));
    }

    void moveToPosition() {
        sf::Clock clock;
        float dx = (position - character.getPosition()).x;
        float dy = (position - character.getPosition()).y;
        if (abs(dx) >= speed || abs(dy) >= speed) {
            current_frame = (current_frame + static_cast<int>(clock.getElapsedTime().asMicroseconds())) % quantity_frames;
            if (abs(dx) > abs(dy)) {
                if (dx > 0) {
                    changeState(RIGHT);
                }
                else {
                    changeState(LEFT);
                }
            }
            else {
                if (dy > 0) {
                    changeState(DOWN);
                }
                else {
                    changeState(UP);
                }
            }
        }
    }

public:
    Hero(const std::string& file_name, float health_, float damage_, std::vector<std::unique_ptr<TemplateCharacter>>& monsters_, std::vector<std::vector<jam::Cell>>& map_, bool is_always_move_ = true, int quantity_frames_ = 4,
        sf::Vector2i size_frame_ = sf::Vector2i(16, 16)) : TemplateCharacter(file_name, quantity_frames_, size_frame_, health_,
            damage_, map_), monsters(monsters_), ability(CLOUD), elements(2, POWER_ELEMENT::FIRE), state(DOWN), is_always_move(is_always_move_), is_move(is_always_move_), position(sf::Vector2f(0, 0)) {}

    void setPosition(float x, float y) {
        character.setPosition(x, y);
        position = sf::Vector2f(x, y);
    }

    void setPosition(sf::Vector2f position_) {
        character.setPosition(position_);
        position = position_;
    }

    void event(const sf::Event& event, sf::RenderWindow& window, const sf::Time &currentTime) {
        if (isLive()) {
            if (event.type == sf::Event::MouseButtonPressed) {
                clickMouse(event, window, currentTime);
                return;
            }
            else if (event.type == sf::Event::KeyPressed) {
                keyPressed(event);
            }
        }
    }

    void drawCharacter(sf::RenderWindow& window) {
        if (isDraw()) {
            if (isLive()) {
                moveToPosition();
                isFighting();
            }
            death();
            window.draw(character);
            if (isLive() && is_move) {
                icon.setPosition(character.getPosition() - sf::Vector2f(0, size_frame.y * scale.y));
                window.draw(icon);
            }
        }
    }
};