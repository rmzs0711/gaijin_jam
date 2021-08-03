#ifndef JAM_GAMESESSION_H
#define JAM_GAMESESSION_H
#include <SFML/Graphics.hpp>
#include <array>
#include <variant>
#include <vector>

namespace jam {

enum CellState { NORMAL, ON_FIRE, AFTER_FIRE };

enum CellObject { EMPTY, GRASS, BUILD_TABLE, FIRE };

struct Cell {
public:
    explicit Cell(std::vector<sf::Texture *> &texturePtrs_,
                  std::vector<sf::Time> &stateDurations_)
        : texturePtrs(texturePtrs_), stateDurations(stateDurations_) {}

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
    const size_t getCellSize() const {
        return cellSize;
    }

    void setSize(const sf::Vector2f &newSize) {
        rect.setSize(newSize);
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
    std::vector<sf::Texture *> &texturePtrs;
    std::vector<sf::Time> &stateDurations;
    sf::RectangleShape rect;
    CellState state = NORMAL;
    CellObject object = EMPTY;
    sf::Time stateStartTime;
    const size_t cellSize = 16;
};

struct Map {
    Map(const std::vector<std::vector<CellObject>>& mapObjects) {
        cells.resize(mapObjects.size());
        for (std::size_t i = 0; i < mapObjects.size(); i++) {
            cells[i].resize(mapObjects[i].size(), );
            for (std::size_t j = 0; j < mapObjects[i].size(); j++) {
                cells[i][j] =
                switch (mapObjects[i][j]) {
                    case EMPTY:
                        break;
                }
            }
        }
    }
private:
    std::vector<std::vector<Cell>> cells;
};

struct Level {
private:
    Map map;
};

struct GameSession {
private:
    std::size_t currentLevelNumber;
    std::vector<Level> levels;
};

}  // namespace jam
#endif  // JAM_GAMESESSION_H
