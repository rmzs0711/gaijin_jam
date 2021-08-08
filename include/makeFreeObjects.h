#pragma once

#include "Cell.h"
namespace jam {
FreeObject makeTree(const sf::Vector2f &position) {
    static FreeObject tree(TREE);
    tree.setPosition(position);

    tree.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x});

    tree.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    tree.setHitBox({tree.getPosition().x - 3 * tree.getSprite().getScale().x,
                    tree.getPosition().y - 20,
                    6 * tree.getSprite().getScale().x, 20});
    return tree;
}

FreeObject makeRock(const sf::Vector2f &position) {
    static FreeObject rock(ROCK);
    rock.setPosition(position);

    rock.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x});

    rock.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    rock.setHitBox({rock.getPosition().x - 3 * rock.getSprite().getScale().x,
                    rock.getPosition().y - 20,
                    6 * rock.getSprite().getScale().x, 20});
    return rock;
}
}  // namespace jam
