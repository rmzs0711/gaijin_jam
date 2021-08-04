#pragma once
// он дебил немножко, но зато милый

#include <cmath>
#include <vector>
#include "Cell.h"
#include "SFML/Graphics.hpp"
#include "usefulFunctions.h"
int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2;
int const CAST_DOWN = 4, CAST_UP = 6, CAST_RIGHT = 8, CAST_LEFT = 10;

enum POWER_ELEMENT { FIRE, ICE, EARTH, NUMBER_OF_POWER_ELEMENTS };
enum ABILITY { FIRE_BLAST, CLOUD, LAVA, FROZEN_BLAST, FROZEN_WALL, BIG_WALL };

bool isCorrectMove(const sf::Sprite &character,
                   const std::vector<std::vector<jam::Cell>> &map) {
    for (auto &i : map) {
        for (auto &j : i) {
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

struct Character {
protected:
    std::vector<std::vector<jam::Cell>> &map;
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon;
    float speed;
    sf::Vector2f scale;
    int current_frame, quantity_frames, size_frame;
    bool is_move, is_always_move;
    std::vector<POWER_ELEMENT> elements;
    ABILITY ability;
    bool readyToCast = false;

    bool isCorrectClick(const sf::Vector2f &mouse) {
        return character.getGlobalBounds().contains(mouse);
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

    void changeState(int direction) {
        sf::Clock clock;
        float dx, dy, old_dx, old_dy;
        initializingCoordinates(dx, dy, direction);
        character.move(dx, dy);
        while (!isCorrectMove(character, map)) {
            character.move(-dx, -dy);
            direction =
                (direction +
                 static_cast<int>(clock.getElapsedTime().asMicroseconds())) %
                4;
            std::cout << direction << '\n';
            initializingCoordinates(dx, dy, direction);
            character.move(dx, dy);
        }
        character.setTextureRect(sf::IntRect(current_frame * size_frame,
                                             direction * size_frame, size_frame,
                                             size_frame));
    }

public:
    Character(const std::string &file_name,
              std::vector<std::vector<jam::Cell>> &map_,
              bool is_always_move_ = true,
              int quantity_frames_ = 4,
              int size_frame_ = 16)
        : map(map_),
          quantity_frames(quantity_frames_),
          size_frame(size_frame_),
          is_always_move(is_always_move_),
          is_move(is_always_move_),
          speed(0.1),
          current_frame(0),
          scale(sf::Vector2f(0, 0)),
          elements(2, POWER_ELEMENT::FIRE) {
        sf::Image character_image;
        checkLoad(character_image, file_name);
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        character.setTextureRect(sf::IntRect(0, 0, size_frame, size_frame));
        character.setOrigin((float)size_frame / 2, (float)size_frame / 2);

        sf::Clock clock;
        sf::Image icon_image;
        checkLoad(
            icon_image,
            "data/images/MiniWorldSprites/Objects/FireballProjectile.png");
        // В useful functions есть функция которая сразу
        //        проверяет правильно ли скачана картинка
        icon_texture.loadFromImage(icon_image);
        icon.setTexture(icon_texture);
        icon.setTextureRect(sf::IntRect(
            (static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4) *
                size_frame,
            0, size_frame, size_frame));
        icon.setOrigin((float)size_frame / 2, (float)size_frame / 2);
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

    virtual void drawCharacter(sf::RenderWindow &window) = 0;
};

struct CharacterKeyboard : Character {
private:
    void clickMouse(const sf::Event &event, sf::RenderWindow &window) {
        if (!is_always_move && event.mouseButton.button == sf::Mouse::Left) {
            if (isCorrectClick(window.mapPixelToCoords(
                    sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
                is_move = true;
            } else {
                is_move = false;
            }
        }
    }

    void keyPressed(const sf::Event &event) {
        sf::Clock clock;
        if (is_move) {
            int time =
                static_cast<int>(clock.getElapsedTime().asMicroseconds());
            if (event.key.code == sf::Keyboard::W ||
                event.key.code == sf::Keyboard::Up) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(UP);
            }
            if (event.key.code == sf::Keyboard::A ||
                event.key.code == sf::Keyboard::Left) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(LEFT);
            }
            if (event.key.code == sf::Keyboard::S ||
                event.key.code == sf::Keyboard::Down) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(DOWN);
            }
            if (event.key.code == sf::Keyboard::D ||
                event.key.code == sf::Keyboard::Right) {
                current_frame = (current_frame + time) % quantity_frames;
                changeState(RIGHT);
            }
        }
    }

public:
    CharacterKeyboard(const std::string &file_name,
                      std::vector<std::vector<jam::Cell>> &map_,
                      bool is_always_move_ = true,
                      int quantity_frames_ = 4,
                      int size_frame_ = 16)
        : Character(file_name,
                    map_,
                    is_always_move_,
                    quantity_frames_,
                    size_frame_) {}

    void event(const sf::Event &event, sf::RenderWindow &window) {
        if (event.type == sf::Event::KeyPressed) {
            keyPressed(event);
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            clickMouse(event, window);
            return;
        }
    }

    void drawCharacter(sf::RenderWindow &window) override {
        window.draw(character);
        if (is_move) {
            icon.setPosition(character.getPosition() -
                             sf::Vector2f(0, size_frame * scale.y));
            window.draw(icon);
        }
    }
};

struct CharacterMouse : Character {
private:
    sf::Vector2f position;

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
            } else {
                is_move = false;
            }
        }

        if (is_move && event.mouseButton.button == sf::Mouse::Right) {
            readyToCast = false;
            position = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
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
    void keyPressed(const sf::Event &event) {
        //        sf::Clock clock;
        //        int time =
        //        static_cast<int>(clock.getElapsedTime().asMicroseconds());
        //        current_frame = (current_frame + time) % quantity_frames;
        if (event.key.code == sf::Keyboard::R) {
            int whatAbility = (1 << elements[0]) | (1 << elements[1]);
            if (whatAbility == 4) {  // 100
                ability = BIG_WALL;
            } else if (whatAbility == 5) {  // 101
                ability = LAVA;
            } else if (whatAbility == 6) {  // 110
                ability = FROZEN_WALL;
            } else if (whatAbility == 2) {  // 010
                ability = FROZEN_BLAST;
            } else if (whatAbility == 3) {  // 011
                ability = CLOUD;
            } else if (whatAbility == 1) {  // 001
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

public:
    CharacterMouse(const std::string &file_name,
                   std::vector<std::vector<jam::Cell>> &map_,
                   bool is_always_move_ = true,
                   int quantity_frames_ = 4,
                   int size_frame_ = 16)
        : Character(file_name,
                    map_,
                    is_always_move_,
                    quantity_frames_,
                    size_frame_),
          position(character.getPosition()) {}

    void event(const sf::Event &event,
               sf::RenderWindow &window,
               const sf::Time &currentTime) {
        if (event.type == sf::Event::MouseButtonPressed) {
            clickMouse(event, window, currentTime);
            return;
        }
        if (event.type == sf::Event::KeyPressed) {
            keyPressed(event);
        }
    }

    void drawCharacter(sf::RenderWindow &window) override {
        std::cout << elements[0] << " " << elements[1] << std::endl;
        moveToPosition();
        window.draw(character);
        if (is_move) {
            icon.setPosition(character.getPosition() -
                             sf::Vector2f(0, size_frame * scale.y));
            window.draw(icon);
        }
    }
};