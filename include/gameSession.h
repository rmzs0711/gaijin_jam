#include <SFML/Graphics.hpp>
#include <array>
#include <tuple>
#include <variant>
#include <vector>
#include "Level.h"
#include "building.h"
#include "makeCharacters.h"
#include "moving_object.h"
#include "usefulFunctions.h"

namespace jam {
struct GameSession {
    void startGame(sf::RenderWindow &window) {
        std::vector<sf::Texture> objectTextures(NUMBER_OF_TEXTURES);
        texturePtrs.resize(NUMBER_OF_TEXTURES);

        for (int i = 0; i < NUMBER_OF_TEXTURES; i++) {
            checkLoadTexture(objectTextures[i], std::get<1>(assetInfo[i]));
            texturePtrs[std::get<0>(assetInfo[i])] = &objectTextures[i];
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
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS,
             DARK_GREEN_GRASS, DEAD_GRASS},
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS,
             DARK_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS},
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS,
             DEAD_GRASS, DEAD_GRASS, DEAD_GRASS},
            {DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS, DEAD_GRASS,
             LIGHT_GREEN_GRASS, LIGHT_GREEN_GRASS, DEAD_GRASS, DEAD_GRASS,
             DEAD_GRASS, DEAD_GRASS},
        };

        levels.emplace_back(firstLevel);

        levels[0].heroSetPosition({150, 150});
        levels[0].monsterSetPosition({cellSize * 3, cellSize * 6});
        levels[0].monsterSetPosition({200, 290}, 1);

        sf::Clock clock1;
        std::vector<FlyingObject> flyingFireObjects;
        AttackBuilding archersTower(flyingFireObjects, clock1);
        sf::Texture archersTowerTexture;
        checkLoad(archersTowerTexture,
                  "data/images/MiniWorldSprites/Buildings/Lime/LimeTower"
                  ".png");
        archersTower.setAttackCooldown(sf::seconds(1));
        archersTower.setAttackRange(1000);
        archersTower.setTexture(archersTowerTexture);
        archersTower.setTextureRect({16, 16, 16, 32});
        archersTower.setPosInMap({3, 3});
        archersTower.setSizeInMap({1, 2});
        archersTower.setScale({(float)cellSize / 16, (float)cellSize / 16});
        archersTower.loadFlyingObjectTextureFromFile(
            "data/images/MiniWorldSprites/Objects/ArrowLong.png");
        archersTower.setAttackPosition({cellSize / 2, cellSize / 2});
        auto secondTower = archersTower;
        //        secondTower.setPosInMap({3, 2});

        FlyingObject arrow;
        arrow.setTextureRect({{5, 3}, {5, 11}});
        arrow.setOrigin(sf::Vector2f(2.5, 0));
        arrow.setScale(5, 5);
        arrow.setSpeed(1);
        arrow.setDamage(50);

        archersTower.setFlyingObject(arrow);

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
            secondTower.draw(window);
            archersTower.draw(window);
            archersTower.attack(levels[0].getMonsters());
            for (auto i = 0; i < flyingFireObjects.size(); i++) {
                flyingFireObjects[i].draw(window);

                if (flyingFireObjects[i].isFinished()) {
                    flyingFireObjects.erase(flyingFireObjects.begin() + i);
                }
            }
            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam