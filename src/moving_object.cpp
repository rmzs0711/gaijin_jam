#include "../include/moving_object.h"

namespace move {

        void DragAndDropAndClick::move_object(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
            if (is_correct_click(mouse) && condition_for_move()) {
                change_position(mouse - old_mouse);
                old_mouse = mouse;
            }
        }

        void Object::draw(sf::RenderWindow& window) {
            window.draw(*this);
        }

        bool Object::is_correct_click(const sf::Vector2f& mouse) {
            return getGlobalBounds().contains(mouse);
        }

        void Object::click_mouse_left(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) {
            if (is_correct_click(mouse)) {
                old_mouse = mouse;
            }
        }

        void Object::click_mouse_right(const sf::Vector2f& mouse, sf::Vector2f& old_mouse) { }

        bool Object::condition_for_move() {
            return sf::Mouse::isButtonPressed(sf::Mouse::Left);
        }

        void Object::change_position(const sf::Vector2f& position) {
            sf::Sprite::move(position);
        }
}