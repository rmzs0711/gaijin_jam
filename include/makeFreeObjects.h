#pragma once

#include "Cell.h"
namespace jam {
FreeObject makeEmptyObject(sf::Vector2f &pos);
FreeObject makeTree(const sf::Vector2f &position);

FreeObject makeRock(const sf::Vector2f &position);

FreeObject makeFire(const sf::Vector2f &position);

FreeObject makeBuildSign(const sf::Vector2f &position);

}  // namespace jam
