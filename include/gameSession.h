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
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
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

        levels[0].heroSetPosition({150, 150});
        levels[0].monsterSetPosition({cellSize * 3, cellSize * 6});
        levels[0].monsterSetPosition({200, 290}, 1);

        sf::Clock clock1;
        std::vector<FlyingObject> flyingFireObjects;
//        AttackBuilding archersTower(flyingFireObjects, clock1);
//        sf::Texture archersTowerTexture;
//        checkLoad(archersTowerTexture,
//                  "data/images/MiniWorldSprites/Buildings/Lime/LimeTower"
//                  ".png");
//        archersTower.setAttackCooldown(sf::seconds(1));
//        archersTower.setAttackRange(1000);
//        archersTower.setTexture(archersTowerTexture);
//        archersTower.setTextureRect({16, 16, 16, 32});
//        archersTower.setPosInMap({3, 3});
//        archersTower.setSizeInMap({1, 2});
//        archersTower.setScale({(float)cellSize / 16, (float)cellSize / 16});
//        archersTower.loadFlyingObjectTextureFromFile(
//            "data/images/MiniWorldSprites/Objects/ArrowLong.png");
//        archersTower.setAttackPosition({cellSize / 2, cellSize / 2});
//        auto secondTower = archersTower;
        //        secondTower.setPosInMap({3, 2});

        FlyingObject arrow;
        arrow.setTextureRect({{5, 3}, {5, 11}});
        arrow.setOrigin(sf::Vector2f(2.5, 0));
        arrow.setScale(5, 5);
        arrow.setSpeed(1);
        arrow.setDamage(3);

//        archersTower.setFlyingObject(arrow);

        Store store(window);
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
            store.drawStore(window);
            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam