#ifdef _MSC_VER
#include "../include/makeFreeObjects.h"
#else
#include "makeFreeObjects.h"
#endif

namespace jam {
FreeObject makeEmptyObject(const sf::Vector2f &pos) {
    FreeObject object(NONE);
    object.setPosition(pos);
    return object;
}
FreeObject makeTree(const sf::Vector2f &position) {
    FreeObject tree(TREE);
    tree.setPosition(position);

    tree.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x * 0.9f});

    tree.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    tree.setHitBox({tree.getPosition().x - 3 * tree.getSprite().getScale().x,
                    tree.getPosition().y - 20,
                    5 * tree.getSprite().getScale().x, 20});
    return tree;
}

FreeObject makeRock(const sf::Vector2f &position) {
    FreeObject rock(ROCK);
    rock.setPosition(position);

    rock.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x / 2});

    rock.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    auto hitBox = rock.getSprite().getGlobalBounds();
    rock.setHitBox({hitBox.left + 3, hitBox.top + hitBox.height / 2,
                    hitBox.width - 5, hitBox.height / 3});
    return rock;
}

FreeObject makeFire(const sf::Vector2f &position) {
    static FreeObject fire(FIRE);
    fire.setPosition(position);

    fire.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x / 2});

    fire.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    fire.setHitBox({0, 0, 0, 0});
    fire.setAnimation(true);
    return fire;
}
FreeObject makeBuildSign(const sf::Vector2f &position) {
    static FreeObject fire(BUILD_SIGN);
    fire.setPosition(position);

    fire.setOrigin({(float)assetCellSize.x / 2, (float)assetCellSize.x / 2});

    fire.setScale({(float)cellSize / (float)assetCellSize.x,
                   (float)cellSize / (float)assetCellSize.y});
    fire.setHitBox({position.x, position.y, cellSize, cellSize});
    fire.setAnimation(true);
    return fire;
}

}  // namespace jam
