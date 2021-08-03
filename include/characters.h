#pragma once
// он дебил немножко, но зато милый

#include "SFML/Graphics.hpp"
#include <cmath>
#include <vector>

int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2;

extern std::vector<sf::Sprite *> objects; // это вектор объектов, с которыми перс не должен сталкиваться, не обязательно Sprite
                                          // главное чтоб можно было следующую функцию реализовать

bool isCorrectMove(const sf::Sprite &character) {
    for (int i = 0; i < objects.size(); i++) {
        if ((*objects[i]).getGlobalBounds().intersects(character.getGlobalBounds())) {
            return false;
        }
    }
    return true;
}

struct Character {
protected:
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon;
    float speed;
    sf::Vector2f scale;
    int current_frame, quantity_frames, size_frame;
    bool is_move, is_always_move;

    bool isCorrectClick(const sf::Vector2f& mouse) {
        return character.getGlobalBounds().contains(mouse);
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

    void changeState(int direction) {
        sf::Clock clock;
        float dx, dy, old_dx, old_dy;
        initializingCoordinates(dx, dy, direction);
        character.move(dx, dy);
        while (!isCorrectMove(character)) {
            character.move(-dx, -dy);
            direction = (direction + static_cast<int>(clock.getElapsedTime().asMicroseconds())) % 4;
            std::cout << direction << '\n';
            initializingCoordinates(dx, dy, direction);
            character.move(dx, dy);
        }
        character.setTextureRect(sf::IntRect(current_frame * size_frame, direction * size_frame, size_frame, size_frame));
    }

public:
    Character(const std::string& file_name, bool is_always_move_ = true, int quantity_frames_ = 4, int size_frame_ = 16) : quantity_frames(quantity_frames_), size_frame(size_frame_), is_always_move(is_always_move_), is_move(is_always_move_), speed(0.1), current_frame(0), scale(sf::Vector2f(0, 0)) {
        sf::Image character_image;
        character_image.loadFromFile(file_name);
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        character.setTextureRect(sf::IntRect(0, 0, size_frame, size_frame));

        sf::Clock clock;
        sf::Image icon_image;
        icon_image.loadFromFile("data/images/MiniWorldSprites/Objects/FireballProjectile.png");
        icon_texture.loadFromImage(icon_image);
        icon.setTexture(icon_texture);
        icon.setTextureRect(sf::IntRect((static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4) * size_frame, 0, size_frame, size_frame));
    }

    void setPosition(float x, float y) {
        character.setPosition(x, y);
    }

    void setPosition(sf::Vector2f position) {
        character.setPosition(position);
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

    virtual void drawCharacter(sf::RenderWindow& window) = 0;
};

struct CharacterKeyboard : Character {
private:
    void clickMouse(const sf::Event& event, sf::RenderWindow& window) {
        if (!is_always_move && event.mouseButton.button == sf::Mouse::Right) {
            if (isCorrectClick(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
                is_move = true;
            }
            else {
                is_move = false;
            }
        }
    }

    void keyPressed(const sf::Event& event) {
        sf::Clock clock;
        if (is_move) {
            int time = static_cast<int>(clock.getElapsedTime().asMicroseconds());
            if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(UP);
            }
            if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(LEFT);
            }
            if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(DOWN);
            }
            if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(RIGHT);
            }
        }
    }

public:
    CharacterKeyboard(const std::string& file_name, bool is_always_move_ = true, int quantity_frames_ = 4, int size_frame_ = 16) : Character(file_name, is_always_move_, quantity_frames_, size_frame_) {}

    void event(const sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::KeyPressed) {
            keyPressed(event);
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            clickMouse(event, window);
            return;
        }
    }

    void drawCharacter(sf::RenderWindow& window) {
        window.draw(character);
        if (is_move) {
            icon.setPosition(character.getPosition() - sf::Vector2f(0, size_frame * scale.y));
            window.draw(icon);
        }
    }
};

struct CharacterMouse : Character {
private:
    sf::Vector2f position;

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
    CharacterMouse(const std::string& file_name, bool is_always_move_ = true, int quantity_frames_ = 4, int size_frame_ = 16) : Character(file_name, is_always_move_, quantity_frames_, size_frame_), position(character.getPosition()) {}

    void event(const sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed) {
            clickMouse(event, window);
            return;
        }
    }

    void drawCharacter(sf::RenderWindow& window) {
        moveToPosition();
        window.draw(character);
        if (is_move) {
            icon.setPosition(character.getPosition() - sf::Vector2f(0, size_frame * scale.y));
            window.draw(icon);
        }
    }
};