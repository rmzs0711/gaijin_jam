#pragma once

#include "SFML/Graphics.hpp"

enum Direction { UP, RIGHT, DOWN, LEFT, NOW, MOVE, NOT_MOVE };

struct Character {
private:
    sf::Texture character_texture;
    sf::Sprite character;
    std::string up, right, down, left, now;
    std::string move_up, move_right, move_down, move_left;
    float speed;
    bool isMove, is_always_move;
public:
    Character(const std::string &up_, const std::string& right_, const std::string& down_, const std::string& left_, 
    bool is_always_move = true, const std::string& move_up_ = "", const std::string& move_right_ = "", 
    const std::string& move_down_ = "", const std::string& move_left_ = "", const sf::Vector2f& position = sf::Vector2f(0, 0),
    float speed_ = 3) : is_always_move(is_always_move), isMove(false), move_up(move_up_), move_right(move_right_), 
    move_down(move_down_), move_left(move_left_), now(down_), speed(speed_), up(up_), right(right_), down(down_), left(left_) {
        sf::Image object_image;
        object_image.loadFromFile(down);
        character_texture.loadFromImage(object_image);
        character.setTexture(character_texture);
        character.setPosition(position);
        if (is_always_move) {
            isMove = true;
            move_up = up;
            move_right = right;
            move_down = down;
            move_left = left;
        }
    }

    bool is_correct_click(const sf::Vector2f& mouse) {
        return character.getGlobalBounds().contains(mouse);
    }

    void click_mouse(const sf::Vector2f& mouse) {
        if (!is_always_move) {
            if (is_correct_click(mouse)) {
                isMove = true;
                change_now(MOVE);
            }
            else {
                isMove = false;
                change_now(NOT_MOVE);
            }
        }
    }

    void change_now(Direction direction) {
        std::string copy_now = now;
        switch (direction) {
        case MOVE: {
            if (now == left) {
                now = move_left;
                break;
            }
            if (now == right) {
                now = move_right;
                break;
            }
            if (now == up) {
                now = move_up;
                break;
            }
            if (now == down) {
                std::cout << "FFF";
                now = move_down;
            }
        } break;
        case NOT_MOVE: {
            if (now == move_left) {
                now = left;
                break;
            }
            if (now == move_right) {
                now = right;
                break;
            }
            if (now == move_up) {
                now = up;
                break;
            }
            if (now == move_down) {
                now = down;
            }
        } break;
        case UP: {
            now = move_up;
        } break;
        case DOWN: {
            now = move_down;
        } break;
        case RIGHT: {
            now = move_right;
        } break;
        case LEFT: {
            now = move_left;
        } break;
        default:
            break;
        }

        if (now != copy_now) {
            sf::Image object_image;
            object_image.loadFromFile(now);
            character_texture.loadFromImage(object_image);
            character.setTexture(character_texture);
        }
    }

    void setSpeed(float speed_) {
        speed = speed_;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(character);
    }

    void keyPressed(const sf::Event &event) {
        if (isMove) {
            float dx = 0, dy = 0;
            if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                dy = -speed;
                change_now(UP);
            }
            if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                dx = -speed;
                change_now(LEFT);
            }
            if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                dy = speed;
                change_now(DOWN);
            }
            if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                dx = speed;
                change_now(RIGHT);
            }
            character.move(dx, dy);
        }
    }
};