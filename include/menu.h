#ifndef JAM_MENU_H
#define JAM_MENU_H
#include <vector>
#include "SFML/Graphics.hpp"
#include "button.h"

struct Menu : sf::Drawable {
    void scale(const sf::Vector2f &windowSize,
               const sf::Vector2f &textureSize) {
        background.scale(windowSize.x / textureSize.x,
                         windowSize.y / textureSize.y);
    }
    const std::vector<std::unique_ptr<Button<void>>> &getButtons() const {
        return buttons;
    }
    void setButtons(std::vector<std::unique_ptr<Button<void>>>& newButtons) {
        buttons.resize(newButtons.size());
        for (int i = 0; i < newButtons.size(); i++) {
            buttons[i] = std::move(newButtons[i]);
        }
    }

    void setBackground(const sf::Sprite &background_) {
        Menu::background = background_;
    }
    const sf::Sprite &getBackground() const {
        return background;
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const
        override {
        target.draw(background);
        for (auto & button : buttons) {
            button->drawButton(target);
        }
    }
    sf::Sprite background;
    mutable std::vector<std::unique_ptr<Button<void>>> buttons;
};

#endif  // JAM_MENU_H
