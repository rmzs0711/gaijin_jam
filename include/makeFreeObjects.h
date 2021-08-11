#pragma once

#include "Cell.h"
namespace jam {
inline FreeObject makeEmptyObject(sf::Vector2f & pos) {
    FreeObject object(NONE);
    object.setPosition(pos);
    return object;
}
inline FreeObject makeTree(const sf::Vector2f &position) {
    static FreeObject tree(TREE);
    tree.setPosition(position);

    tree.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x * 0.9f});

    tree.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    tree.setHitBox({tree.getPosition().x - 3 * tree.getSprite().getScale().x,
                    tree.getPosition().y - 20,
                    5 * tree.getSprite().getScale().x, 20});
    return tree;
}

inline FreeObject makeRock(const sf::Vector2f &position) {
    static FreeObject rock(ROCK);
    rock.setPosition(position);

    rock.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x / 2});

    rock.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    auto hitBox = rock.getSprite().getGlobalBounds();
    rock.setHitBox({hitBox.left + 3, hitBox.top + hitBox.height / 2,
                    hitBox.width - 5, hitBox.height / 3});
    return rock;
}

inline FreeObject makeFire(const sf::Vector2f &position) {
    static FreeObject fire(FIRE);
    fire.setPosition(position);

    fire.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x / 2});

    fire.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    fire.setHitBox({0, 0, 0, 0});
    fire.setAnimation(true);
    return fire;
}

}  // namespace jam
