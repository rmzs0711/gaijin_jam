#ifndef JAM_GAMESESSION_H
#define JAM_GAMESESSION_H
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>

struct MapCell {
private:
    const size_t cellSize = 16;
    sf::RectangleShape rect;
};

struct Map {
private:
    std::vector<MapCell> cells;
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

#endif  // JAM_GAMESESSION_H
