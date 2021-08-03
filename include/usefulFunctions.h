#ifndef JAM_USEFULFUNCTIONS_H
#define JAM_USEFULFUNCTIONS_H
#include <cstdlib>
#include <string>

template<typename T>
void checkLoad(T& var, const std::string& loadPath) {
    if (!var.loadFromFile(loadPath)) {
        std::exit(3);
    }
}

template<typename T>
bool isContains(const T& shape, const sf::Vector2i& pos) {
    return shape.getGlobalBounds().contains(pos);
}

#endif  // JAM_USEFULFUNCTIONS_H
