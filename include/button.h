#ifndef GAIJIN_JAM_BUTTON_H
#define GAIJIN_JAM_BUTTON_H
#include <functional>
#include <iostream>
#include <utility>
#include "SFML/Graphics.hpp"

struct CentralisedText : sf::Text {
    explicit CentralisedText(
        const std::string &str = "",
        const std::string &fontAddress = "../data/fonts/mono.otf")
        : sf::Text() {
        static sf::Font font;
        if (!font.loadFromFile(fontAddress)) {
            std::cerr << "Font didnt loaded" << std::endl;
            throw std::exception();
        }
        setFont(font);
        setString(str);
        centralise();
    }
    void centralise() {
        sf::FloatRect textRect = getLocalBounds();
        setOrigin(textRect.left + textRect.width / 2.f,
                  textRect.top + textRect.height / 2.f);
    }
};

template <typename T>
struct Clickable {
    virtual T handleClick() = 0;
    virtual ~Clickable() = default;
};


template <typename T>
struct Button : Clickable<T> {
    Button() = delete;
    explicit Button(std::function<T()> func, std::string str_ = "")
        : function(func), str(std::move(str_)) {}
    T handleClick() override {
        return function();
    }
    [[nodiscard]] const std::string &getString() const {
        return str;
    }

    virtual void setClickableTexture() {

    }
private:
    std::function<T()> function;
    std::string str;
};

template <typename T>
struct CircleButton : Button<T>, sf::CircleShape {
    explicit CircleButton(std::function<T()> func, const std::string &str = "")
        : Button<T>(func, str) {}
    void setTextSize(const unsigned int &newSize) {
        textSize = newSize;
    }
    unsigned getTextSize() {
        return textSize;
    }
    void draw_button(sf::RenderWindow &window) const {
        window.draw(*this);
        CentralisedText text(Button<T>::getString(), textSize);
        text.setPosition(getPosition());
        window.draw(text);
    }

private:
    int textSize = 30;
};

template <typename T>
struct RectangleButton : Button<T>, sf::RectangleShape {
    explicit RectangleButton(std::function<T()> func,
                             const std::string &str = "")
        : Button<T>(func, str) {}

    void drawButton(sf::RenderWindow &window) const {
        window.draw(*this);
        CentralisedText text;
        text.setString("go");
        text.setCharacterSize(30);
        text.centralise();
        text.setPosition(getPosition() + getSize() / 2.f);
        window.draw(text);
    }

private:
};

#endif  // GAIJIN_JAM_BUTTON_H
