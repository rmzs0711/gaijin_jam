#ifndef JAM_MENU_H
#define JAM_MENU_H
#include <vector>
#include "SFML/Graphics.hpp"
#include "button.h"

struct Menu : sf::Drawable {
    Menu() {
        background.setPosition(0, 0);
    }
    void setSize(const sf::Vector2f& newSize) {
        background.setSize(newSize);
    }
    void setColor(const sf::Color& newColor) {
        background.setFillColor(newColor);
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

    void setBackground(const sf::Texture *background_) {
        Menu::background.setTexture(background_);
    }
    const sf::RectangleShape &getBackground() const {
        return background;
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
    mutable std::vector<std::unique_ptr<Button<void>>> buttons;
};

#endif  // JAM_MENU_H
