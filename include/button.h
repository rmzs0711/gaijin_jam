#ifndef GAIJIN_JAM_BUTTON_H
#define GAIJIN_JAM_BUTTON_H
#include <functional>
#include <iostream>
#include <utility>
#include "SFML/Graphics.hpp"

struct CentralisedText : sf::Text {
    explicit CentralisedText(
        const std::string &str,
        const unsigned int &size,
        const std::string &fontAddress = "../data/fonts/mono.otf")
        : sf::Text() {
        static sf::Font font;
        if (!font.loadFromFile(fontAddress)) {
            std::cerr << "Font didnt loaded" << std::endl;
            throw std::exception();
        }
        setFont(font);

        setString(str);
        setCharacterSize(size);
        setOrigin(sf::Vector2f(sf::Vector2u(0, 0)));

        sf::FloatRect textRect = getLocalBounds();
        setOrigin(textRect.left + textRect.width / 2.0f,
                  textRect.top + textRect.height / 2.0f);
    }
};

template <typename T>
struct Clickable {
    virtual T handle_click() = 0;
    virtual ~Clickable() = default;
};

template <typename T>
struct Button : Clickable<T> {
    Button() = delete;
    explicit Button(std::function<T()> func, std::string str_ = "")
        : function(func), str(std::move(str_)) {}
    T handle_click() override {
        return function();
    }
    [[nodiscard]] const std::string &getString() const {
        return str;
    }

private:
    std::function<T()> function;
    std::string str;
};

template <typename T>
struct CircleButton : Button<T>, sf::CircleShape {
    explicit CircleButton(std::function<T()> func,
                          const std::string &str = "")
        : Button<T>(func, str) {}

    void draw_button(sf::RenderWindow &window) const {
        window.draw(*this);
        CentralisedText text(Button<T>::getString(), 100);
        text.setPosition(getPosition());
        window.draw(text);
    }
};

template<typename T>
struct RectangleButton : Button<T>, sf::RectangleShape {
    explicit RectangleButton(std::function<T()> func,
                          const std::string &str = "")
        : Button<T>(func, str) {}

    void draw_button(sf::RenderWindow &window) const {
        window.draw(*this);
        CentralisedText text(Button<T>::getString(), 100);
        text.setPosition(getPosition() + getSize() / 2.f);
        window.draw(text);
    }
};

#endif  // GAIJIN_JAM_BUTTON_H
