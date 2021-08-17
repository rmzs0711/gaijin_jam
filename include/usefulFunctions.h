#pragma once

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>

template <typename T>
inline T bounds(const T &x, const T &down, const T &up) {
    return std::min(std::max(x, down), up - 1);
}

inline void checkLoadTexture(sf::Texture &var,
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
inline void checkLoad(T &var, const std::string &loadPath) {
    if (!var.loadFromFile(loadPath)) {
        assert(0);
    }
}

template <typename T>
inline bool isContains(const T &shape, const sf::Vector2i &pos) {
    return shape.getGlobalBounds().contains(pos);
}

template <typename T>
inline T quadraticDist(const sf::Vector2<T> &first,
                       const sf::Vector2<T> &second) {
    T xDist = (first - second).x;
    T yDist = (first - second).y;
    return xDist * xDist + yDist * yDist;
}

template <typename T>
inline T sign(T x) {
    return x > 0 ? 1 : -1;
}
