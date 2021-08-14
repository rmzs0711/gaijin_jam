#pragma once

#include <functional>
#include <iostream>
#include <utility>
#include "SFML/Graphics.hpp"

struct CentralisedText : sf::Text {
    explicit CentralisedText(
        const std::string &str = "",
        const std::string &fontAddress = "data/fonts/mono.otf")
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
        : function(func), str(std::move(str_)) {
        text.setString(Button<T>::getString());
        text.setCharacterSize(Button<T>::getTextSize());
        text.centralise();
    }
    T handleClick() override {
        return function();
    }
    [[nodiscard]] const std::string &getString() const {
        return str;
    }

    virtual void setClickableTexture(const sf::Texture *newTexture) {
        clickableTexture = newTexture;
    }
    [[nodiscard]] const sf::Texture *getClickableTexture() const {
        return clickableTexture;
    }

    void setTextSize(const unsigned int &newSize) {
        textSize = newSize;
    }
    [[nodiscard]] const unsigned &getTextSize() const {
        return textSize;
    }
    virtual void drawButton(sf::RenderTarget &target) = 0;
    virtual bool isCorrectClick(const sf::Vector2f &) = 0;

private:
    std::function<T()> function;
    std::string str;
    unsigned textSize = 30;
    const sf::Texture *clickableTexture = nullptr;

protected:
    CentralisedText text;
};

template <typename T>
struct CircleButton : Button<T>, sf::CircleShape {
    explicit CircleButton(std::function<T()> func, const std::string &str = "")
        : Button<T>(func, str) {}
    bool isCorrectClick(const sf::Vector2f &pos) override {
        return this->getGlobalBounds().contains(pos);
    }

    void drawButton(sf::RenderTarget &target) override {
        if (!Button<T>::getClickableTexture()) {
            Button<T>::setClickableTexture(getTexture());
        }
        auto hitBox = getGlobalBounds();
        target.draw(*this);
        if (hitBox.contains(target.mapPixelToCoords(sf::Mouse::getPosition(
                dynamic_cast<sf::RenderWindow &>(target))))) {
            auto texture = getTexture();
            setTexture(Button<T>::getClickableTexture(), true);

            target.draw(*this);
            setTexture(texture, true);
        }
    }

private:
};

template <typename T>
struct RectangleButton : Button<T>, sf::RectangleShape {
    explicit RectangleButton(std::function<T()> func,
                             const std::string &str = "")
        : Button<T>(func, str) {}
    bool isCorrectClick(const sf::Vector2f &pos) override {
        return this->getGlobalBounds().contains(pos);
    }
    void drawButton(sf::RenderTarget &target) override {
        Button<T>::text.setPosition(getPosition() + getSize() / 2.f);
        Button<T>::text.setFillColor(sf::Color::White);
        target.draw(*this);
        if (getGlobalBounds().contains(
                target.mapPixelToCoords(sf::Mouse::getPosition()))) {
            auto texture = getTexture();
            setTexture(Button<T>::getClickableTexture());
            target.draw(*this);
            setTexture(texture);
            Button<T>::text.setFillColor(sf::Color::Red);
        }
        target.draw(Button<T>::text);
    }

private:
};