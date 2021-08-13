#pragma once

#include <SFML/Graphics.hpp>
#include "button.h"
#include <string>
#include <vector>
#include "Level.h"

    struct DragAndDropAndClick {
    public:
        virtual bool is_correct_click(const sf::Vector2f& mouse) = 0;
        virtual void click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) = 0;
        virtual bool condition_for_move() = 0;
        virtual void moveObject(const sf::Vector2f& position) = 0;
        virtual void move_object(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
            if (is_correct_click(mouse) && condition_for_move()) {
                moveObject(mouse - old_mouse);
                old_mouse = mouse;
            }
        }
    };

    struct Object : DragAndDropAndClick, sf::Sprite {
    private:
        sf::Texture object_texture;
        bool is_valid;
        std::string name_file;

    public:
        Object() : is_valid(false), name_file("") {}

        Object(const std::string& name_file_, const sf::Vector2f& position = sf::Vector2f(0, 0)) : is_valid(true), name_file(name_file_) {
            object_texture.loadFromFile(name_file);
            setTexture(object_texture);
            setPosition(position);
        }

        void draw(sf::RenderTarget& window) {
            if (is_valid) {
                window.draw(*this);
            }
        }

        void loadFromFile(const std::string& name_file_, sf::Vector2i size_object = sf::Vector2i(0, 0)) {
            name_file = name_file_;
            if (name_file == "") {
                is_valid = false;
                return;
            }
            is_valid = true;
            object_texture.loadFromFile(name_file);
            setTextureRect(sf::IntRect(0, 0, size_object.x, size_object.y));
            setTexture(object_texture);
        }

        bool isValid() {
            return is_valid;
        }

        bool is_correct_click(const sf::Vector2f& mouse) {
            if (is_valid) {
                return getGlobalBounds().contains(mouse);
            }
        }

        void click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
            if (is_correct_click(mouse)) {
                old_mouse = mouse;
            }
        }

        bool condition_for_move() {
            return sf::Mouse::isButtonPressed(sf::Mouse::Left);
        }

        void moveObject(const sf::Vector2f& position) {
            sf::Sprite::move(position);
        }

        std::string getNameFile() {
            return name_file;
        }
    };

    template <typename T>
    struct Polygon : Button<T>, DragAndDropAndClick, sf::ConvexShape {
    protected:
        Polygon(std::function<T()> func, const sf::Vector2f& position = sf::Vector2f(0, 0), sf::Color color = sf::Color::Transparent, float thickness = 10, std::string str_ = "") : Button<T>(func, str_) {
            setPosition(position);
            setFillColor(color);
            setOutlineColor(sf::Color::Transparent);
            setOutlineThickness(thickness);
        }

    public:
        Polygon(const std::vector<sf::Vector2f>& point, std::function<T()> func, const sf::Vector2f & position = sf::Vector2f(0, 0), sf::Color color = sf::Color::Transparent, float thickness = 10, std::string str_ = "") : Button<T>(func, str_) {
            setPointCount(point.size());
            for (int i = 0; i < point.size(); i++) {
                setPoint(i, point[i]);
            }
            setPosition(position);
            setFillColor(color);
            setOutlineColor(sf::Color::Transparent);
            setOutlineThickness(thickness);
        }

        virtual void draw(sf::RenderWindow& window) {
            CentralisedText text(Button<T>::getString());
            text.setPosition(getPosition());
            text.setCharacterSize(Button<T>::getTextSize());
            text.centralise();
            window.draw(*this);
            if (is_correct_click(sf::Vector2f(sf::Mouse::getPosition(window)))) {
                auto texture = getTexture();
                setTexture(Button<T>::getClickableTexture());
                window.draw(*this);
                setTexture(texture);
            }
            window.draw(text);
        }

        virtual bool is_correct_click(const sf::Vector2f& mouse) {
            std::vector<sf::Vector2f> point;
            for (int i = 0; i < getPointCount(); i++) {
                point.push_back(getPoint(i));
            }
            bool result = false;
            int j = point.size() - 1;
            for (int i = 0; i < point.size(); i++) {
                float xi = point[i].x + getPosition().x;
                float yi = point[i].y + getPosition().y;
                float xj = point[j].x + getPosition().x;
                float yj = point[j].y + getPosition().y;
                if ((yi < mouse.y && yj >= mouse.y || yj < mouse.y && yi >= mouse.y) &&
                    (xi + (mouse.y - yi) / (yj - yi) * (xj - xi) < mouse.x))
                    result = !result;
                j = i;
            }
            return result;
        }

        void click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
            if (is_correct_click(mouse)) {
                setOutlineColor(sf::Color::Green);
                if (getFillColor() == sf::Color::Red) {
                    setFillColor(sf::Color::Magenta);
                }
                else {
                    setFillColor(sf::Color::Red);
                }
                old_mouse = mouse;
            }
            else {
                setOutlineColor(sf::Color::Transparent);
            }
        }

        void click_mouse_right(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
            if (is_correct_click(mouse)) {
                setOutlineColor(sf::Color::Green);
                if (getFillColor() == sf::Color::Red) {
                    setFillColor(sf::Color::Blue);
                }
                else {
                    setFillColor(sf::Color::Red);
                }
            }
            else {
                setOutlineColor(sf::Color::Transparent);
            }
        }

        bool condition_for_move() {
            return sf::Mouse::isButtonPressed(sf::Mouse::Left) && getOutlineColor() == sf::Color::Green;
        }

        void change_position(const sf::Vector2f& position) {
            sf::ConvexShape::move(position);
        }
    };

    template <typename T>
    struct Circle : Polygon<T> {
    private:
        sf::CircleShape circle;
    public:
        Circle(std::function<T()> func, float radius_ = 90, const sf::Vector2f& position = sf::Vector2f(0, 0), sf::Color color = sf::Color::Transparent, float thickness = 10, std::string str_ = "") : Polygon<T>(func, position, color, thickness, str_) {
            circle.setRadius(radius_);
        }

        bool is_correct_click(const sf::Vector2f& mouse) {
            sf::Vector2f position = Polygon<T>::getPosition();
            float radius = circle.getRadius();
            return (mouse.x - position.x - radius) * (mouse.x - position.x - radius) + (mouse.y - position.y - radius) * (mouse.y - position.y - radius) <= radius * radius;
        }

        void draw(sf::RenderWindow& window) {
            circle.setPosition(Polygon<T>::getPosition());
            circle.setFillColor(Polygon<T>::getFillColor());
            circle.setOutlineColor(Polygon<T>::getOutlineColor());
            circle.setTexture(Polygon<T>::getTexture());
            circle.setOutlineThickness(Polygon<T>::getOutlineThickness());
            window.draw(circle);

            CentralisedText text(Button<T>::getString());
            text.setPosition(Polygon<T>::getPosition() + sf::Vector2f(circle.getRadius(), circle.getRadius()));
            text.setCharacterSize(Button<T>::getTextSize());
            text.centralise();
            if (is_correct_click(sf::Vector2f(sf::Mouse::getPosition(window)))) {
                auto texture = Polygon<T>::getTexture();
                circle.setTexture(Button<T>::getClickableTexture());
                window.draw(circle);
                circle.setTexture(texture);
            }
            window.draw(text);
        }
    };