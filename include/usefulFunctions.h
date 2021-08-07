#pragma once

#include <cassert>
#include <cstdlib>
#include <string>

int bounds(const int& x, const int& down, const int& up) {
    return std::min(std::max(x, down), up - 1);
}

void checkLoadTexture(sf::Texture &var,
                      const std::string &loadPath,
                      const sf::IntRect &area = {-1, -1, -1, -1}) {
    if (area == sf::IntRect(-1, -1, -1, -1)) {
        if (!var.loadFromFile(loadPath)) {
            assert(0);
        }
        return;
    }
    if (!var.loadFromFile(loadPath, area)) {
        std::cout << loadPath << std::endl;
        assert(0);
    }
}

template <typename T>
void checkLoad(T &var, const std::string &loadPath) {
    if (!var.loadFromFile(loadPath)) {
        assert(0);
    }
}

template <typename T>
bool isContains(const T &shape, const sf::Vector2i &pos) {
    return shape.getGlobalBounds().contains(pos);
}

template<typename T>
T quadraticDist(const sf::Vector2<T>& first, const sf::Vector2<T>& second) {
    T xDist = (first - second).x;
    T yDist = (first - second).y;
    return xDist * xDist + yDist * yDist;
}