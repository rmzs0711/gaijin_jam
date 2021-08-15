#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "button.h"

struct Menu : sf::Drawable {
    void setBackground(const sf::Texture *background_) {
        Menu::background.setTexture(background_);
    }

    /*const sf::RectangleShape &getBackground() const {
        return background;
    }*/

    void setSize(const sf::Vector2f &newSize) {
        background.setSize(newSize);
    }

    sf::Vector2f getSize() {
        return background.getSize();
    }

    void setColor(const sf::Color &newColor) {
        background.setFillColor(newColor);
    }

    void setPosition(const sf::Vector2f &newPosition) {
        background.setPosition(newPosition);
    }

    void setOutlineThickness(float size) {
        background.setOutlineThickness(size);
        background.setOutlineColor(sf::Color(112, 80, 40));
    }

    Menu(sf::RenderWindow &window) {
        background.setPosition(0, 0);
        background.setSize(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())));
        texture_background.loadFromFile("data/images/base.png");
        background.setTexture(&texture_background);
    }

    const std::vector<std::unique_ptr<Button<void>>> &getButtons() const {
        return buttons;
    }

    void setButtons(std::vector<std::unique_ptr<Button<void>>> &newButtons) {
        buttons.resize(newButtons.size());
        for (int i = 0; i < newButtons.size(); i++) {
            buttons[i] = std::move(newButtons[i]);
        }
    }

    void addButton(std::unique_ptr<Button<void>> newButton) {
        buttons.push_back(std::move(newButton));
    }

private:
    void draw(sf::RenderTarget &target,
              sf::RenderStates states) const override {
        target.draw(background);
        for (auto &button : buttons) {
            button->drawButton(target);
        }
    }
    sf::RectangleShape background;
    sf::Texture texture_background;
    mutable std::vector<std::unique_ptr<Button<void>>> buttons;
};