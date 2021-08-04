#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "Level.h"
#include "characters.h"
#include "moving_object.h"
#include "usefulFunctions.h"


std::vector<jam::Cell> currentMap;

namespace jam {
struct GameSession {
    void startGame(sf::RenderWindow &window) {
        std::vector<sf::Texture> objectTextures(NUMBER_OF_OBJECTS);
        texturePtrs.resize(NUMBER_OF_OBJECTS);

        for (int i = 0; i < NUMBER_OF_OBJECTS; i++) {
            checkLoadTexture(
                objectTextures[i], std::get<2>(assetInfo[i]),
                sf::IntRect(std::get<1>(assetInfo[i]), assetCellSize));
            texturePtrs[std::get<0>(assetInfo[i])] = &objectTextures[i];
        }

        stateDurations = {sf::Time::Zero};

        std::vector<std::vector<CellObject>> firstLevel = {
            {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
            {EMPTY, DEAD_GRASS, LIGHT_GREEN_GRASS, DARK_GREEN_GRASS, EMPTY},
            {EMPTY, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, EMPTY},
            {EMPTY, DEAD_GRASS, DARK_GREEN_GRASS, DARK_GREEN_GRASS, EMPTY},
            {EMPTY, LIGHT_GREEN_GRASS, DARK_GREEN_GRASS, DEAD_GRASS, EMPTY},
            {EMPTY, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS, EMPTY},
            {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        };

        levels.emplace_back(firstLevel);

        for (auto &i : levels.back().getMap()) {
            for (auto &j : i) {
                currentMap.push_back(j);
            }
        }

        CharacterMouse character2(
            "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
            "PurpleMelee/AssasinPurple.png",
            true);  // убери false, если хочешь, чтоб он всегда ходил, без
                    // выделений
        character2.setPosition(100, 100);
        character2.setScale(4, 4);

        while (window.isOpen()) {
            sf::Event event{};

            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    default:
                        break;
                }
                character2.event(event, window);
            }
            levels.back().draw(window);
            character2.drawCharacter(window);

            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam
