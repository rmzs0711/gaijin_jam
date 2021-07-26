#ifndef GAIJIN_JAM_BUTTON_H
#define GAIJIN_JAM_BUTTON_H
#include <functional>
#include "SFML/Graphics.hpp"
template <typename T>
struct Clickable {
    virtual T handle_click() = 0;
    virtual ~Clickable() = default;
};

template <typename T>
struct Button : Clickable<T>, sf::Transformable {
    Button() = delete;
    explicit Button(std::function<T()> func, const std::string &str = "") {
        text.setString(str);
    }
    T handle_click() override {
        return function();
    }
    const sf::Text& getText() const {
        return text;
    }
    virtual void sync() {
        text.setPosition(getPosition());
    }
private:
    std::function<T()> function;
    sf::Text text;
};

template <typename T>
struct CircleButton : Button<T>, sf::Drawable {
    explicit CircleButton(std::function<T()> func, const std::string &str = "")
        : Button<T>(func, str) {
    }
    void setRadius(const float& new_radius) {
        circle.setRadius(new_radius);
    }
    void sync() {

    }

//    void draw(sf::RenderTarget &target,
//              sf::RenderStates states) const override {
//        sf::CircleShape circle(radius);
//        circle.setOrigin(getPosition());
//        circle.setPosition(getPosition());
//        target.draw(circle);
//    }
private:
    sf::CircleShape circle;
    void draw(sf::RenderTarget &target,
              sf::RenderStates states) const override {
        
    }
};

#endif  // GAIJIN_JAM_BUTTON_H
