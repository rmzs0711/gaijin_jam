#ifndef JAM_TEXTBOX_H
#define JAM_TEXTBOX_H

#include <string>
#include "SFML/Graphics.hpp"

struct InputBox : sf::RectangleShape {
    const sf::Text &getInput() const {
        return input;
    }
    void setInput(const sf::Text &newInput) {
        InputBox::input = newInput;
    }

    const std::string &getVarName() const {
        return varName;
    }
    void setVarName(const std::string &newVarName) {
        InputBox::varName = newVarName;
    }

    const sf::Font &getFont() const {
        return font;
    }
    void setFont(const sf::Font &newFont) {
        InputBox::font = newFont;
    }

    unsigned int getTextSize() const {
        return textSize;
    }
    void setTextSize(unsigned int newTextSize) {
        InputBox::textSize = newTextSize;
    }

    void run(sf::RenderWindow &window, std::string &var) {
        //        std::string currentLine = getInput().getString();
        sf::Text varNameInBox;
        varNameInBox.setCharacterSize(getTextSize());
        varNameInBox.setFont(getFont());
        varNameInBox.setString(varName + ": ");
        varNameInBox.setOrigin(0, varNameInBox.getLocalBounds().height / 2);

        varNameInBox.setPosition(getPosition().x,
                                 getPosition().y + getLocalBounds().height / 2);

        input.setCharacterSize(getTextSize());
        input.setFont(getFont());
        input.setOrigin(varNameInBox.getOrigin());
        input.setPosition(
            varNameInBox.getPosition().x + varNameInBox.getGlobalBounds().width,
            varNameInBox.getPosition().y);
        while (true) {
            sf::Event event{};
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed: {
                        window.close();
                        return;
                    } break;
                    case sf::Event::TextEntered: {
                        if (getInput().getPosition().x +
                                getInput().getGlobalBounds().width +
                                (float)getTextSize() >=
                            getPosition().x + getGlobalBounds().width) {
                            break;
                        }
                        char enteredChar =
                            static_cast<char>(event.text.unicode);
                        if (!isalpha(enteredChar) && !isdigit(enteredChar)) {
                            break;
                        }
                        input.setString(std::string(getInput().getString()) +
                                        enteredChar);
                    } break;
                    case sf::Event::KeyPressed: {
                        switch (event.key.code) {
                            case sf::Keyboard::Escape: {
                                return;
                            }
                            case sf::Keyboard::BackSpace: {
                                if (!getInput().getString().getSize()) {
                                    break;
                                }
                                auto finalString =
                                    std::string(getInput().getString());
                                finalString.pop_back();
                                input.setString(finalString);
                            } break;
                            case sf::Keyboard::Enter: {
                                var = input.getString();
                                return;
                            }
                            default: {
                                break;
                            }
                        }
                    } break;
                    default: {
                        break;
                    } break;
                }
            }
            window.draw(*this);
            window.draw(input);
            window.draw(varNameInBox);
            window.display();
        }
    }

private:
    sf::Text input;
    std::string varName;
    sf::Font font;

    unsigned textSize = 30;
};

#endif  // JAM_TEXTBOX_H
