#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct DragAndDropAndClick {
public:
    virtual bool is_correct_click(const sf::Vector2f& mouse) = 0;
    virtual void click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) = 0;
    virtual void click_mouse_right(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) = 0;
    virtual bool condition_for_move() = 0;
    virtual void change_position(const sf::Vector2f& position) = 0;
    virtual void move(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
        if (is_correct_click(mouse) && condition_for_move()) {
            change_position(mouse - old_mouse);
            old_mouse = mouse;
        }
    }
    virtual void draw(sf::RenderWindow& window) = 0;
};

enum TypeObject { JUST_OBJECT };

struct Object : DragAndDropAndClick {
private:
    TypeObject type;
    sf::Texture object_texture;
    sf::Sprite object;

public:
    Object(const std::string& name_file, const sf::Vector2f& position) {
        sf::Image object_image;
        object_image.loadFromFile(name_file);
        object_texture.loadFromImage(object_image);
        object.setTexture(object_texture);
        object.setPosition(position);
    }

    TypeObject getType() {
        return type;
    }

    virtual void draw(sf::RenderWindow& window) {
        window.draw(object);
    }

    virtual bool is_correct_click(const sf::Vector2f& mouse) {
        return object.getGlobalBounds().contains(mouse);
    }

    virtual void click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
        if (is_correct_click(mouse)) {
            old_mouse = mouse;
        }
    }

    virtual void click_mouse_right(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) { }

    virtual bool condition_for_move() {
        return sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }

    virtual void change_position(const sf::Vector2f& position) {
        object.move(position);
    }
};

enum TypePolygon { JUST_POLYGON };

struct Polygon : DragAndDropAndClick {
protected:
    TypePolygon type;
    sf::ConvexShape polygon;

    Polygon(const sf::Vector2f& position, TypePolygon type_, sf::Color color, float thickness) : type(type_) {
        polygon.setPosition(position);
        polygon.setFillColor(color);
        polygon.setOutlineColor(sf::Color::Transparent);
        polygon.setOutlineThickness(thickness);
    }

public:
    Polygon(const std::vector<sf::Vector2f>& point, const sf::Vector2f& position, TypePolygon type_, sf::Color color, float thickness) : type(type_) {
        polygon.setPointCount(point.size());
        for (int i = 0; i < point.size(); i++) {
            polygon.setPoint(i, point[i]);
        }
        polygon.setPosition(position);
        polygon.setFillColor(color);
        polygon.setOutlineColor(sf::Color::Transparent);
        polygon.setOutlineThickness(thickness);
    }

    TypePolygon getType() {
        return type;
    }

    virtual void draw(sf::RenderWindow& window) {
        window.draw(polygon);
    }

    virtual bool is_correct_click(const sf::Vector2f& mouse) {
        std::vector<sf::Vector2f> point;
        for (int i = 0; i < polygon.getPointCount(); i++) {
            point.push_back(polygon.getPoint(i));
        }
        bool result = false;
        int j = point.size() - 1;
        for (int i = 0; i < point.size(); i++) {
            float xi = point[i].x + polygon.getPosition().x;
            float yi = point[i].y + polygon.getPosition().y;
            float xj = point[j].x + polygon.getPosition().x;
            float yj = point[j].y + polygon.getPosition().y;
            if ((yi < mouse.y && yj >= mouse.y || yj < mouse.y && yi >= mouse.y) &&
                (xi + (mouse.y - yi) / (yj - yi) * (xj - xi) < mouse.x))
                result = !result;
            j = i;
        }
        return result;
    }

    void click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
        if (is_correct_click(mouse)) {
            polygon.setOutlineColor(sf::Color::Green);
            if (polygon.getFillColor() == sf::Color::Red) {
                polygon.setFillColor(sf::Color::Magenta);
            }
            else {
                polygon.setFillColor(sf::Color::Red);
            }
            old_mouse = mouse;
        }
        else {
            polygon.setOutlineColor(sf::Color::Transparent);
        }
    }

    void click_mouse_right(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
        if (is_correct_click(mouse)) {
            polygon.setOutlineColor(sf::Color::Green);
            if (polygon.getFillColor() == sf::Color::Red) {
                polygon.setFillColor(sf::Color::Blue);
            }
            else {
                polygon.setFillColor(sf::Color::Red);
            }
        }
        else {
            polygon.setOutlineColor(sf::Color::Transparent);
        }
    }

    bool condition_for_move() {
        return sf::Mouse::isButtonPressed(sf::Mouse::Left) && polygon.getOutlineColor() == sf::Color::Green;
    }

    void change_position(const sf::Vector2f& position) {
        polygon.move(position);
    }
};

struct Circle : Polygon {
private:
    sf::CircleShape circle;
public:
    Circle(float radius_, const sf::Vector2f& position, TypePolygon type_, sf::Color color, float thickness) : Polygon(position, type_, color, thickness) {
        circle.setRadius(radius_);
    }

    bool is_correct_click(const sf::Vector2f& mouse) {
        auto pos = polygon.getPosition();
        auto radius = circle.getRadius();
        return (mouse.x - pos.x - radius) * (mouse.x - pos.x - radius) +
            (mouse.y - pos.y - radius) * (mouse.y - pos.y - radius) <=
            radius * radius;
    }

    void draw(sf::RenderWindow& window) {
        circle.setPosition(polygon.getPosition());
        circle.setFillColor(polygon.getFillColor());
        circle.setOutlineColor(polygon.getOutlineColor());
        circle.setOutlineThickness(polygon.getOutlineThickness());
        window.draw(circle);
    }
};