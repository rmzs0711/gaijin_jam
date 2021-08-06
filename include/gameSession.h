#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "Level.h"
#include "building.h"
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

        stateDurations.resize(NUMBER_OF_STATES, sf::seconds(2));
        {
            int cnt = 0;
            for (auto &i : fire) {
                checkLoad(i,
                          "data/images/fire/" + std::to_string(cnt) + ".png");
                cnt++;
            }
        }
        std::vector<std::vector<int>> firstLevel = {
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, EMPTY, EMPTY, EMPTY, EMPTY},
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, FROZEN_GRASS, TREE_1,
             DARK_GREEN_GRASS, EMPTY},
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, TREE_2, FROZEN_TREE_1,
             DEAD_GRASS, EMPTY},
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, TREE_3,
             DARK_GREEN_GRASS, EMPTY},
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS,
             DARK_GREEN_GRASS, DEAD_GRASS, EMPTY},
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS,
             DEAD_GRASS, DEAD_GRASS, EMPTY},
            {EMPTY, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, EMPTY, EMPTY, EMPTY, EMPTY},
        };

        levels.emplace_back(firstLevel);

        levels[0].heroSetPosition({150, 150});
        levels[0].heroSetScale({4, 4});
        levels[0].monsterSetPosition({60, 200});
        levels[0].monsterSetScale({3, 3});
        levels[0].monsterSetPosition({200, 290}, 1);
        levels[0].monsterSetScale({3, 3}, 1);

        sf::Clock clock1;
        while (window.isOpen()) {
            levels[0].updateStates(clock1.getElapsedTime());
            sf::Event event{};
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    default:
                        break;
                }
                levels[0].event(event, window, clock1.getElapsedTime());
            }
            levels.back().updateStates(clock1.getElapsedTime());
            levels.back().draw(window);
            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam
