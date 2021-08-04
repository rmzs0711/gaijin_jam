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

        stateDurations = {sf::seconds(10)};

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

//        character2.setPosition(100, 100);
        levels[0].characterSetPosition({100, 100});
//        character2.setScale(4, 4);
        levels[0].characterSetScale({4, 4});
        sf::Clock clock1;
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
                levels[0].event(event, window);
//                character2.event(event, window);
            }
            levels.back().updateStates(clock1.getElapsedTime());
            levels.back().draw(window);
//            character2.drawCharacter(window);

            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam
