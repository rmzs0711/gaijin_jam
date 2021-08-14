#ifdef _MSC_VER
#include "../include/Level.h"
#include "../include/game.h"
#include "../include/gameSession.h"
#include "../include/makeBuilding.h"
#include "../include/store.h"
#else
#include "Level.h"
#include "game.h"
#include "gameSession.h"
#include "makeBuilding.h"
#include "store.h"
#endif

extern const sf::Vector2f sizeBaseButton;

namespace {
bool isInView(const sf::View &view, const sf::FloatRect &objectRect) {
    return sf::FloatRect(view.getCenter() - view.getSize() / 2.f,
                         view.getSize())
        .intersects(objectRect);
}

template <typename T>
void checkDraw(const sf::View &view, T &object, sf::RenderTarget &window) {
    if (isInView(view, object.getGlobalBounds())) {
        object.draw(window);
    }
}
template <>
void checkDraw<Monster>(const sf::View &view,
                        Monster &object,
                        sf::RenderTarget &window) {
    if (isInView(view, object.getGlobalBounds())) {
        object.drawCharacter(window);
    }
}
template <>
void checkDraw<Hero>(const sf::View &view,
                     Hero &object,
                     sf::RenderTarget &window) {
    if (isInView(view, object.getGlobalBounds())) {
        object.drawCharacter(window);
    }
}

}  // namespace
bool jam::Level::addHero(const std::shared_ptr<Hero> &hero) {
    if ((*hero).isCorrectMove()) {
        heroes.emplace_back(hero);
        return true;
    }
    return false;
}
bool jam::Level::addMonster(const std::shared_ptr<Monster> &monster) {
    if ((*monster).isCorrectMove()) {
        monsters.emplace_back(monster);
        return true;
    }
    return false;
}

bool jam::Level::addAttackBuilding(AttackBuilding building) {
    auto hitBox = (*building.getSprite()).getGlobalBounds();
    {
        auto start = freeObjects.lower_bound(
            jam::makeTree({hitBox.left, hitBox.top - jam::cellSize}));
        auto end = freeObjects.upper_bound(
            jam::makeTree({hitBox.left, hitBox.top + jam::cellSize}));
        for (auto &i = start; i != end; i++) {
            if (i->getHitBox().intersects({hitBox.left,
                                           hitBox.top + hitBox.height / 2,
                                           hitBox.width, hitBox.height / 2})) {
                return false;
            }
        }
    }
    {
        auto start = attackBuildings.lower_bound(jam::makeEmptyAttackBuilding(
            *this, sf::Vector2i{(int)hitBox.left / jam::cellSize - 1,
                                (int)hitBox.top / jam::cellSize - 1}));
        auto end = attackBuildings.upper_bound(jam::makeEmptyAttackBuilding(
            *this, sf::Vector2i((int)hitBox.left / jam::cellSize + 1,
                                (int)hitBox.top / jam::cellSize + 1)));
        for (auto &i = start; i != end; i++) {
            if (i->getHitBox().intersects({hitBox.left,
                                           hitBox.top + hitBox.height / 2,
                                           hitBox.width, hitBox.height / 2})) {
                return false;
            }
        }
    }

    for (auto &i : home) {
        if (i.getHitBox().intersects({hitBox.left,
                                      hitBox.top + hitBox.height / 2,
                                      hitBox.width, hitBox.height / 2})) {
            return false;
        }
    }

    attackBuildings.insert(building);
    for (auto &i : heroes) {
        if (!(*i).isCorrectMove()) {
            attackBuildings.erase(building);
            return false;
        }
    }
    for (auto &i : monsters) {
        if (!(*i).isCorrectMove()) {
            attackBuildings.erase(building);
            return false;
        }
    }

    return true;
}

void jam::Level::heroSetPosition(const sf::Vector2f &newPos, std::size_t i) {
    heroes[i]->setPosition(newPos);
}
void jam::Level::monsterSetScale(const sf::Vector2f &newScale, std::size_t i) {
    (*monsters[i]).setScale(newScale);
}
void jam::Level::monsterSetPosition(const sf::Vector2f &newPos, std::size_t i) {
    (*monsters[i]).setPosition(newPos);
}
void jam::Level::heroSetScale(const sf::Vector2f &newScale, std::size_t i) {
    (*heroes[i]).setScale(newScale);
}

jam::Level::Level(sf::RenderWindow &window,
                  const std::vector<std::vector<int>> &mapObjects)
    : ability(FIRE_BLAST),
      elements(2, POWER_ELEMENT::FIRE),
      menuGameButton(
          [&]() {
              GameSession::closeGame(window,
                  [&]() { sf::View view_(sf::FloatRect{ sf::Vector2f(0, 0), sf::Vector2f(window.getSize()) });
                          window.setView(view_);
                          jam::Game::startGame(window); });
          },
          "Menu"), is_active_store(false), storeButton([&]() { is_active_store = !is_active_store; }, "Store") {
    menuGameButton.setSize({sizeBaseButton.x / 2, sizeBaseButton.y});
    menuGameButton.setFillColor(sf::Color(74, 53, 27));
    menuGameButton.setPosition(
        {window.mapPixelToCoords(sf::Vector2i(window.getSize())).x -
             sizeBaseButton.x / 2 - 20,
         20});
    storeButton.setSize({ sizeBaseButton.x / 2, sizeBaseButton.y });
    storeButton.setFillColor(sf::Color(74, 53, 27));
    storeButton.setPosition({ window.mapPixelToCoords(sf::Vector2i(window.getSize())).x -
             sizeBaseButton.x / 2 - 20, window.mapPixelToCoords(sf::Vector2i(window.getSize())).y -
             sizeBaseButton.y - 20 });

    freeObjects.insert(makeTree({200, 300}));
    map.resize(mapObjects.size());
    for (int i = 0; i < mapObjects.size(); i++) {
        map[i].resize(mapObjects[i].size());
        for (int j = 0; j < mapObjects[i].size(); j++) {
            map[i][j].setPosInMap({j, i});
            map[i][j].setBackgroundType(mapObjects[i][j]);
        }
    }
    supportBuildings.insert(makeMinerCave(*this, {7, 3}));
    home.push_back(makeHome(*this));
}

void jam::Level::updateStates() {
    for (auto &i : map) {
        for (auto &j : i) {
            j.updateState(clock1.getElapsedTime());
        }
    }
    for (auto &i : freeObjects) {
        i.updateAnimation();
    }
    for (auto &i : attackBuildings) {
        i.attack(clock1.getElapsedTime());
    }
    for (auto &i : supportBuildings) {
        i.doMagic(clock1.getElapsedTime());
    }
    for (auto &i : heroes) {
        i->updateState();
    }
    for (auto &i : monsters) {
        i->updateState();
    }
}
void jam::Level::draw(sf::RenderWindow &window) {
    clock1.restart();
    Store store(window, is_active_store);
    sf::Vector2f mouse;
    sf::View view(
        sf::FloatRect{sf::Vector2f(0, 0), sf::Vector2f(window.getSize())});

    sf::View miniMapView({0, 0, (float)(map[0].size() * cellSize),
                          (float)(map.size() * cellSize)});
    sf::RenderTexture minimap;
    minimap.create(map[0].size() * cellSize, map.size() * cellSize);
    minimap.setView(miniMapView);
    for (auto &i : map) {
        for (auto &j : i) {
            j.draw(minimap);
        }
    }
    minimap.display();
    sf::Texture text = minimap.getTexture();
    sf::Sprite fullSizeMinimapSprite(text);

    sf::RectangleShape miniMapArea(sf::Vector2f(window.getSize()));
    sf::RectangleShape minimapOutline(
        {(float)(map[0].size() * cellSize - 160), (float)(map.size() *
                                                        cellSize - 160)});
    minimapOutline.setPosition(80, 80);
    minimapOutline.setFillColor(sf::Color::Transparent);
    minimapOutline.setOutlineThickness(80);
    minimapOutline.setOutlineColor(sf::Color::Black);
    miniMapArea.setFillColor(sf::Color::Transparent);
    miniMapArea.setOutlineThickness(50);
    miniMapArea.setOutlineColor(sf::Color::Blue);

    minimap.create(window.getSize().x, window.getSize().y);
    minimap.clear(sf::Color::Transparent);
    miniMapView.setViewport(
        {1 - (float)window.getSize().y * 0.25f / (float)window.getSize().x,
         0.60, (float)window.getSize().y * 0.25f / (float)window.getSize().x,
         0.25});
    minimap.setView(miniMapView);

    window.setView(view);

    sf::View object_view(
        sf::FloatRect{sf::Vector2f(0, 0), sf::Vector2f(window.getSize())});
    sf::RenderTexture object_bar;
    object_bar.create(window.getSize().x, window.getSize().y);
    object_bar.setView(object_view);

    while (window.isOpen()) {
        minimap.clear(sf::Color::Transparent);
        miniMapArea.setPosition(shift);
        minimap.draw(fullSizeMinimapSprite);
        minimap.draw(minimapOutline);
        minimap.draw(miniMapArea);

        minimap.display();
        sf::Sprite minimapSprite(minimap.getTexture());
        minimapSprite.setColor(sf::Color(200, 200, 200));
        if (clock1.getElapsedTime() - lastRegenTime > regenCooldown) {
            mana += manaRegen;
            mana = std::min(mana, maxMana);
            lastRegenTime = clock1.getElapsedTime();
        }

        updateStates();
        sf::Event event{};
        while (window.pollEvent(event)) {
            for (auto &i : heroes) {
                dynamic_cast<Hero &>(*i).event(event, window,
                                               clock1.getElapsedTime());
            }
            for (auto &i : money) {
                store.addMoney((*i).event(event, window));
            }
            store.event(event, object_bar, mouse, *this);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (menuGameButton.isCorrectClick(
                            object_bar.mapPixelToCoords(
                                { event.mouseButton.x,
                                 event.mouseButton.y }))) {
                            menuGameButton.handleClick();
                        }
                        else if (storeButton.isCorrectClick(
                            object_bar.mapPixelToCoords(
                                { event.mouseButton.x,
                                 event.mouseButton.y }))) {
                            storeButton.handleClick();
                        }
                        sf::FloatRect minimapZoneOnScreen = {
                            0, 0, (float)(map[0].size() * cellSize),
                            (float)(map.size() * cellSize)};
                        sf::Vector2f mouseOnMinimap = minimap.mapPixelToCoords(
                            {event.mouseButton.x, event.mouseButton.y});
                        if (minimapZoneOnScreen.contains(mouseOnMinimap)) {
                            shift = mouseOnMinimap -
                                    sf::Vector2f(window.getSize()) / 2.f;
                        }
                    }
                    if (!readyToCast) {
                        continue;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left &&
                        mana > abilityCost.at(ability)) {
                        mana -= abilityCost.at(ability);
                        auto selectedCell = sf::Vector2i{
                            window.mapPixelToCoords(
                                {event.mouseButton.x, event.mouseButton.y}) /
                            (float)cellSize};
                        std::swap(selectedCell.x, selectedCell.y);
                        switch (ability) {
                            case ABILITY::FIRE_BLAST:
                                for (int i = -1; i < 2; i++) {
                                    for (int j = -1; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::BLAST,
                                                   clock1.getElapsedTime());
                                        freeObjects.insert(
                                            jam::makeFire(sf::Vector2f(
                                                (float)bounds(
                                                    selectedCell.y + j, 0,
                                                    (int)map[0].size()) *
                                                        jam::cellSize +
                                                    (float)jam::cellSize / 2,
                                                (float)bounds(
                                                    selectedCell.x + i, 0,
                                                    (int)map.size()) *
                                                        jam::cellSize +
                                                    (float)jam::cellSize / 2)));
                                    }
                                }
                                break;
                            case ABILITY::CLOUD:
                                for (int i = -1; i < 2; i++) {
                                    for (int j = -1; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::CLOUD,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::LAVA:
                                for (int i = 0; i < 2; i++) {
                                    for (int j = 0; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::LAVA,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::FROZEN_BLAST:
                                for (int i = -2; i < 3; i++) {
                                    for (int j = -2; j < 3; j++) {
                                        if (std::abs(j) + std::abs(i) >= 3) {
                                            continue;
                                        }
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::FROZEN_BLAST,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::EARTHSHAKE:
                                for (int i = 0; i < 2; i++) {
                                    for (int j = 0; j < 2; j++) {
                                        map[bounds(selectedCell.x + i, 0,
                                                   (int)map.size())]
                                           [bounds(selectedCell.y + j, 0,
                                                   (int)map[0].size())]
                                               .setState(
                                                   jam::EARTHSHAKE,
                                                   clock1.getElapsedTime());
                                    }
                                }
                                break;
                            case ABILITY::WALL:
                                map[selectedCell.x][selectedCell.y].setState(
                                    jam::WALL, clock1.getElapsedTime());
                                freeObjects.insert(jam::makeRock(sf::Vector2f(
                                    (float)selectedCell.y * jam::cellSize +
                                        (float)jam::cellSize / 2,
                                    (float)selectedCell.x * jam::cellSize +
                                        (float)jam::cellSize / 2)));
                                break;
                        }
                    }
                    readyToCast = false;
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::R &&
                        mana > combineCost) {
                        mana -= combineCost;
                        int whatAbility =
                            (1 << elements[0]) | (1 << elements[1]);
                        if (whatAbility == 4) {  // 100
                            ability = ABILITY::EARTHSHAKE;
                        } else if (whatAbility == 5) {  // 101
                            ability = ABILITY::LAVA;
                        } else if (whatAbility == 6) {  // 110
                            ability = ABILITY::WALL;
                        } else if (whatAbility == 2) {  // 010
                            ability = ABILITY::FROZEN_BLAST;
                        } else if (whatAbility == 3) {  // 011
                            ability = ABILITY::CLOUD;
                        } else if (whatAbility == 1) {  // 001
                            ability = ABILITY::FIRE_BLAST;
                        } else {
                            assert(0);
                        }
                        continue;
                    }
                    if (event.key.code == sf::Keyboard::E) {
                        readyToCast = true;
                        continue;
                    }
                    elements[0] = elements[1];
                    if (event.key.code == sf::Keyboard::Z) {
                        elements[1] = POWER_ELEMENT::FIRE;
                    }
                    if (event.key.code == sf::Keyboard::X) {
                        elements[1] = POWER_ELEMENT::ICE;
                    }
                    if (event.key.code == sf::Keyboard::C) {
                        elements[1] = POWER_ELEMENT::EARTH;
                    }
                    if (event.key.code == sf::Keyboard::W) {
                        shift.y -= viewMoveSpeed;
                    }
                    if (event.key.code == sf::Keyboard::S) {
                        shift.y += viewMoveSpeed;
                    }
                    if (event.key.code == sf::Keyboard::A) {
                        shift.x -= viewMoveSpeed;
                    }
                    if (event.key.code == sf::Keyboard::D) {
                        shift.x += viewMoveSpeed;
                    }
                    shift.x = bounds(
                        shift.x, 0.f,
                        (float)(cellSize * map[0].size()) - view.getSize().x);
                    shift.y = bounds(
                        shift.y, 0.f,
                        (float)(cellSize * map.size()) - view.getSize().y);
                    break;
                default:
                    break;
            }
        }

        window.clear();
        object_bar.clear(sf::Color::Transparent);


        for (auto &i : map) {
            for (auto &j : i) {
                checkDraw(view, j, window);
                if (((clock1.getElapsedTime() - lastTreeTime) > treeCooldown) &&
                    !rand() &&
                    (j.getBackgroundType() == DARK_GREEN_GRASS ||
                     j.getBackgroundType() == LIGHT_GREEN_GRASS)) {
                    auto x = makeTree(
                        {j.getGlobalBounds().left + (float)(rand() % cellSize),
                         j.getGlobalBounds().top + (float)(rand() % cellSize)});
                    lastTreeTime = clock1.getElapsedTime();
                    if (!intersectionObjects(x.getSprite(), heroes)) {
                        freeObjects.emplace(x);
                    }
                }
            }
        }
        for (auto &i : home) {
            i.draw(window);
        }
        std::sort(monsters.begin(), monsters.end(), charactersCompare);
        std::sort(heroes.begin(), heroes.end(), charactersCompare);
        auto freeObject = freeObjects.begin();
        auto monster = monsters.begin();
        auto hero = heroes.begin();
        auto flyingObject = flyingObjects.begin();
        auto attackBuilding = attackBuildings.begin();
        auto supportBuilding = supportBuildings.begin();
        for (; freeObject != freeObjects.end() || monster != monsters.end() ||
               hero != heroes.end() || flyingObject != flyingObjects.end() ||
               attackBuilding != attackBuildings.end() ||
               supportBuilding != supportBuildings.end();) {
            auto objectPos = freeObject != freeObjects.end()
                                 ? freeObject->getPosition().y
                                 : std::numeric_limits<float>::max();
            auto monsterPos = monster != monsters.end()
                                  ? (*monster)->getSprite()->getPosition().y
                                  : std::numeric_limits<float>::max();
            auto heroPos = hero != heroes.end()
                               ? (*hero)->getSprite()->getPosition().y
                               : std::numeric_limits<float>::max();
            auto flyingObjectPos = flyingObject != flyingObjects.end()
                                       ? flyingObject->getPosition().y
                                       : std::numeric_limits<float>::max();
            auto attackBuildingPos = attackBuilding != attackBuildings.end()
                                         ? attackBuilding->getHitBox().top
                                         : std::numeric_limits<float>::max();
            auto supportBuildingPos = supportBuilding != supportBuildings.end()
                                          ? supportBuilding->getHitBox().top
                                          : std::numeric_limits<float>::max();

            float poses[6] = {objectPos,         monsterPos,
                              heroPos,           flyingObjectPos,
                              attackBuildingPos, supportBuildingPos};
            std::sort(std::begin(poses), std::end(poses));
            if (poses[0] == objectPos) {
                freeObject->draw(window);
                checkDraw(view, *freeObject, window);
                auto cell =
                    sf::Vector2i(freeObject->getPosition() / (float)cellSize);
                if (freeObject->getObjectType() == ROCK &&
                        (map[cell.y][cell.x].getState() != WALL &&
                         map[cell.y][cell.x].getState() != FROZEN_BLAST) ||
                    freeObject->getObjectType() == FIRE &&
                        map[cell.y][cell.x].getState() != BLAST ||
                    freeObject->getObjectType() != ROCK &&
                        map[cell.y][cell.x].getState() == WALL ||
                    map[cell.y][cell.x].getState() == EARTHSHAKE ||
                    map[cell.y][cell.x].getState() == LAVA) {
                    freeObject = freeObjects.erase(freeObject);
                    continue;
                } else if (map[cell.y][cell.x].getState() == FROZEN_BLAST) {
                    switch (freeObject->getObjectType()) {
                        case TREE:
                            freeObject->changeObjectType(FROZEN_TREE);
                            break;
                        case DEAD_TREE:
                            freeObject->changeObjectType(FROZEN_DEAD_TREE);
                            break;
                        case ROCK:
                            freeObject->changeObjectType(FROZEN_ROCK);
                            break;
                        case BUILD_SIGN:
                        case FIRE:
                        case NONE:
                        case FROZEN_TREE:
                        case FROZEN_DEAD_TREE:
                        case FROZEN_ROCK:
                        case NUMBER_OF_OBJECTS:
                            break;
                    }
                } else if (map[cell.y][cell.x].getState() == NORMAL) {
                    switch (freeObject->getObjectType()) {
                        case TREE:
                        case DEAD_TREE:
                        case ROCK:
                        case BUILD_SIGN:
                        case FIRE:
                        case NONE:
                        case NUMBER_OF_OBJECTS:
                            break;
                        case FROZEN_TREE:
                            freeObject->changeObjectType(TREE);
                            break;
                        case FROZEN_DEAD_TREE:
                            freeObject->changeObjectType(DEAD_TREE);
                            break;
                        case FROZEN_ROCK:
                            freeObject->changeObjectType(ROCK);
                            break;
                    }
                } else if (map[cell.y][cell.x].getState() == BLAST) {
                    switch (freeObject->getObjectType()) {
                        case TREE:
                        case DEAD_TREE:
                        case FROZEN_TREE:
                        case FROZEN_DEAD_TREE:
                            freeObject->changeObjectType(DEAD_TREE);
                            break;
                        case FROZEN_ROCK:
                            freeObject->changeObjectType(ROCK);
                            break;
                        default:
                            break;
                    }
                }
                freeObject++;
            } else if (poses[0] == monsterPos) {
                checkDraw(view, dynamic_cast<Monster &>(**monster), window);
                if (!(*monster)->isDraw()) {
                    monster = monsters.erase(monster);
                } else {
                    monster++;
                }
            } else if (poses[0] == heroPos) {
                checkDraw(view, dynamic_cast<Hero &>(**hero), window);
                if (!(*hero)->isDraw()) {
                    hero = heroes.erase(hero);
                } else {
                    hero++;
                }
            } else if (poses[0] == flyingObjectPos) {
                checkDraw(view, *flyingObject, window);
                if (flyingObject->isFinished()) {
                    flyingObject = flyingObjects.erase(flyingObject);
                } else {
                    flyingObject++;
                }
            } else if (poses[0] == attackBuildingPos) {
                checkDraw(view, *attackBuilding, window);
                attackBuilding++;
            } else if (poses[0] == supportBuildingPos) {
                checkDraw(view, *supportBuilding, window);
                supportBuilding++;
            }
        }
        for (auto &i : money) {
            checkDraw(view, *i, window);
        }
        view.setCenter(shift + view.getSize() / 2.f);
        minimapSprite.setPosition(shift);
        store.drawStore(object_bar);
        menuGameButton.drawButton(object_bar);
        storeButton.drawButton(object_bar);
        object_bar.display();

        sf::Sprite storeSprite(object_bar.getTexture());
        storeSprite.setPosition(minimapSprite.getPosition());

        window.draw(storeSprite);
        window.draw(minimapSprite);
        window.setView(view);
        window.display();
    }
}
std::vector<std::vector<jam::Cell>> &jam::Level::getMap() {
    return map;
}
const std::set<jam::FreeObject> &jam::Level::getFreeObjects() const {
    return freeObjects;
}
const std::vector<std::shared_ptr<TemplateCharacter>> &jam::Level::getHeroes()
    const {
    return heroes;
}
const std::vector<std::shared_ptr<TemplateCharacter>> &jam::Level::getMonsters()
    const {
    return monsters;
}
void jam::Level::addMoney(const std::shared_ptr<Money> &money_) {
    money.push_back(money_);
}
const sf::Vector2f &jam::Level::getShift() const {
    return shift;
}
const std::set<jam::SupportBuilding> &jam::Level::getSupportBuildings() const {
    return supportBuildings;
}
