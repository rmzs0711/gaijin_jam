#pragma once

#include "SFML/Graphics.hpp"
#include <cmath>
#include <vector>

int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2, FIGHTING = 4;

extern std::vector<sf::Sprite *> not_move_objects;

sf::Sprite* intersectionObjects(const sf::Sprite &character, const std::vector<sf::Sprite*> &objects) {
    for (int i = 0; i < objects.size(); i++) {
        if ((*objects[i]).getGlobalBounds().intersects(character.getGlobalBounds()) && objects[i] != &character) {
            return objects[i];
        }
    }
    return nullptr;
}



struct TemplateCharacter {
protected:
    float speed;
    sf::Vector2f scale;
    int current_frame, quantity_frames;
    sf::Vector2i size_frame;
    float health, damage, current_health, current_damage;
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon;

    TemplateCharacter(const std::string& file_name, int quantity_frames_, sf::Vector2i size_frame_, float health_, float damage_) 
        : current_frame(0), size_frame(size_frame_), quantity_frames(quantity_frames_), speed(1), current_health(health_), 
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

    virtual void drawCharacter(sf::RenderWindow& window) = 0;
};

TemplateCharacter* intersectionObjects(const sf::Sprite& character, const std::vector<TemplateCharacter*>& objects) {
    for (int i = 0; i < objects.size(); i++) {
        if ((*(*objects[i]).getSprite()).getGlobalBounds().intersects(character.getGlobalBounds()) && (*objects[i]).getSprite()
            != &character) {

            return objects[i];
        }
    }
    return nullptr;
}



extern std::vector<TemplateCharacter*> heroes;

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





extern std::vector<TemplateCharacter *> monsters;

struct Hero : TemplateCharacter {
protected:
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

    void clickMouse(const sf::Event& event, sf::RenderWindow& window) {
        if (!is_always_move && event.mouseButton.button == sf::Mouse::Right) {
            if (isCorrectClick(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
                is_move = true;
            }
            else {
                is_move = false;
            }
        }

        if (is_move && event.mouseButton.button == sf::Mouse::Left) {
            position = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
    }

    void initializingCoordinates(float &dx, float &dy, int direction) {
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
        while (intersectionObjects(character, not_move_objects) != nullptr) {
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
    Hero(const std::string& file_name, float health_, float damage_, bool is_always_move_ = true, int quantity_frames_ = 4,
        sf::Vector2i size_frame_ = sf::Vector2i(16, 16)) : TemplateCharacter(file_name, quantity_frames_, size_frame_, health_,
            damage_), state(DOWN), is_always_move(is_always_move_), is_move(is_always_move_), position(sf::Vector2f(0, 0)) {}

    void setPosition(float x, float y) {
        character.setPosition(x, y);
        position = sf::Vector2f(x, y);
    }

    void setPosition(sf::Vector2f position_) {
        character.setPosition(position_);
        position = position_;
    }

    void event(const sf::Event& event, sf::RenderWindow& window) {
        if (isLive()) {
            if (event.type == sf::Event::MouseButtonPressed) {
                clickMouse(event, window);
                return;
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