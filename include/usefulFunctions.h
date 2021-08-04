#ifndef JAM_USEFULFUNCTIONS_H
#define JAM_USEFULFUNCTIONS_H
#include <cassert>
#include <cstdlib>
#include <string>

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

#endif  // JAM_USEFULFUNCTIONS_H
