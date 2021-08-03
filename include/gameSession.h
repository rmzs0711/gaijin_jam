#ifndef JAM_GAMESESSION_H
#define JAM_GAMESESSION_H
#include <SFML/Graphics.hpp>
#include <array>
#include <variant>
#include <vector>
#include "usefulFunctions.h"

namespace jam {

enum CellState { NORMAL, ON_FIRE, AFTER_FIRE };

enum CellObject {
    EMPTY,
    //                  GRASS, BUILD_TABLE, FIRE
};

inline const size_t cellSize = 16;
std::vector<sf::Texture *> texturePtrs;
std::vector<sf::Time> stateDurations;

struct Cell {
public:
    explicit Cell() {
        rect.setSize(sf::Vector2f(cellSize, cellSize));
    }

    void setState(const CellState &newState,
                  const sf::Time &newStateStartTime = sf::Time::Zero) {
        stateStartTime = newStateStartTime;
        switch (state) {
            case NORMAL:
                rect.setFillColor(sf::Color::White);
                break;
            case ON_FIRE:
                rect.setFillColor(sf::Color::Red);
                break;
            case AFTER_FIRE:
                rect.setFillColor(sf::Color::White);
                break;
        }
    }

    void setPosition(const sf::Vector2f &position) {
        rect.setPosition(position);
    }

    void setObject(const CellObject &newObject) {
        object = newObject;
        rect.setTexture(texturePtrs[object]);
    }
    void updateState(const sf::Time &currentTime) {
        if (stateDurations[state] < currentTime - stateStartTime) {
            switch (state) {
                case NORMAL:
                    break;
                case ON_FIRE:
                    setState(CellState::AFTER_FIRE);
                    break;
                case AFTER_FIRE:
                    setState(CellState::NORMAL);
                    break;
            }
        }
    }
    void draw(sf::RenderWindow &window) {
        window.draw(rect);
    }

private:
    sf::RectangleShape rect;
    CellState state = NORMAL;
    CellObject object = EMPTY;
    sf::Time stateStartTime;
    //    const size_t cellSize = 16;
};

struct Level {
    Level(const std::vector<std::vector<CellObject>> &mapObjects) {
        map.resize(mapObjects.size());
        for (std::size_t i = 0; i < mapObjects.size(); i++) {
            map[i].resize(mapObjects[i].size());
            for (std::size_t j = 0; j < mapObjects[i].size(); j++) {
                map[i][j].setPosition(sf::Vector2f(
                    sf::Vector2<size_t>(j * cellSize, i * cellSize)));
                map[i][j].setObject(mapObjects[i][j]);
            }
        }
    }
    void draw(sf::RenderWindow &window) {
        for (auto &i : map) {
            for (auto &j : i) {
                j.draw(window);
            }
        }
    }

private:
    std::vector<std::vector<Cell>> map;
};

struct GameSession {
    void startGame(sf::RenderWindow &window) {
        sf::Texture empty;
        checkLoad(empty, "data/images/black.png");
        texturePtrs = {&empty};
        stateDurations = {sf::Time::Zero};

        std::vector<std::vector<CellObject>> firstLevel = {
            {EMPTY, EMPTY, EMPTY}};
        levels.emplace_back(firstLevel);
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
            }
            levels.back().draw(window);
            window.display();
        }
    }

private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam
#endif  // JAM_GAMESESSION_H
