#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "Level.h"
#include "building.h"
#include "moving_object.h"
#include "usefulFunctions.h"
#include "store.h"

namespace jam {
struct GameSession {
    void startGame(sf::RenderWindow &window) {
        std::vector<sf::Texture> objectTextures(NUMBER_OF_TEXTURES);
        texturePtrs.resize(NUMBER_OF_TEXTURES);
        texturesNumberOfFrames.resize(NUMBER_OF_TEXTURES);
        for (int i = 0; i < NUMBER_OF_TEXTURES; i++) {
            checkLoadTexture(objectTextures[i], std::get<2>(assetInfo[i]),
                             std::get<1>(assetInfo[i]));
            auto & j = std::get<0>(assetInfo[i]);
            auto & k = std::get<1>(assetInfo[i]);
            texturePtrs[std::get<0>(assetInfo[i])] = &objectTextures[i];
            texturesNumberOfFrames[std::get<0>(assetInfo[i])] = {
                std::get<1>(assetInfo[i]).width / assetCellSize.x,
                std::get<1>(assetInfo[i]).height / assetCellSize.y};
        }

        stateDurations.resize(NUMBER_OF_STATES, sf::seconds(2));

        std::vector<std::vector<int>> firstLevel = {
            {ROAD, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS,
             DEAD_GRASS, DEAD_GRASS},
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, FROZEN_GRASS, DEAD_GRASS,
             DARK_GREEN_GRASS, DEAD_GRASS},
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, FROZEN_GRASS,
             DEAD_GRASS, DEAD_GRASS},
             {DEAD_GRASS, DEAD_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS,
             DARK_GREEN_GRASS, DEAD_GRASS},
             {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS,
             DARK_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS},
             {LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS,
             DEAD_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS},
             {DEAD_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS,
             DEAD_GRASS, DEAD_GRASS},
        };

        levels.emplace_back(firstLevel);
        std::vector<sf::Vector2f> monster_path;
        monster_path.emplace_back(200, 200);
        monster_path.emplace_back(220, 280);
        monster_path.emplace_back(260, 340);
        levels[0].addMonster(Monster::makeArmouredRedDemon(window, levels[0], monster_path));
        levels[0].addMonster(Monster::makeMammoth(window, levels[0], monster_path));
        levels[0].monsterSetPosition({cellSize * 3, cellSize * 6});
        levels[0].monsterSetPosition({ cellSize * 6, cellSize * 2}, 1);

        sf::Clock clock1;
        std::vector<FlyingObject> flyingFireObjects;

        sf::Vector2f mouse;
        while (window.isOpen()) {
            window.clear();
            sf::Event event{};
            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        break;
                    default:

                        break;
                }
            }
            levels[0].draw(window);
            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam