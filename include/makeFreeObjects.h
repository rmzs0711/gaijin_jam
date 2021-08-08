#pragma once

#include "Cell.h"
namespace jam {
FreeObject makeTree(const sf::Vector2f &position) {
    static FreeObject tree(TREE);
    tree.setPosition(position);

    tree.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x});

    tree.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});

    return tree;
}
}  // namespace jam
