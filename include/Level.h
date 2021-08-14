#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <limits>
#include <memory>
#include <set>
#include <tuple>
#include <variant>
#include <vector>
#include "Cell.h"
#include "building.h"
#include "button.h"
#include "characters.h"
#include "makeFreeObjects.h"
#include "usefulFunctions.h"

namespace {}

struct Money {
private:
    sf::Texture texture_icon;
    sf::Sprite icon;
    int money;
    bool is_valid;

public:
    Money(int money_, sf::Vector2f position) : money(money_), is_valid(true) {
        texture_icon.loadFromFile(
            "data/images/MiniWorldSprites/Miscellaneous/Chests.png");
        icon.setTexture(texture_icon);
        icon.setTextureRect(sf::IntRect(16, 0, 16, 16));
        icon.setScale(4, 4);
        icon.setPosition(position);
    }
    sf::FloatRect getGlobalBounds() const {
        return icon.getGlobalBounds();
    }

    static std::shared_ptr<Money> makeMoney(int money_, sf::Vector2f position) {
        return std::make_unique<Money>(money_, position);
    }

    bool isCorrectClick(const sf::Vector2f &mouse) {
        return icon.getGlobalBounds().contains(mouse);
    }

    int event(const sf::Event &event, sf::RenderTarget &window) {
        if (is_valid && event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            isCorrectClick(window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
            is_valid = false;
            return money;
        }
        return 0;
    }

    void draw(sf::RenderTarget &window) {
        if (is_valid) {
            window.draw(icon);
        }
    }

    sf::Sprite *getSprite() {
        return &icon;
    }

    bool isValid() {
        return is_valid;
    }
};

struct TemplateCharacter;
struct Monster;
struct Hero;
namespace jam {

struct Level {
    friend TemplateCharacter;
    friend Monster;
    friend Hero;

    friend Building;
    friend Home;
    friend AttackBuilding;
    friend SupportBuilding;

    explicit Level(sf::RenderWindow &window,
                   const std::vector<std::vector<int>> &mapObjects);

    bool addHero(const std::shared_ptr<Hero> &hero);

    bool addMonster(const std::shared_ptr<Monster> &monster);
    void addMoney(const std::shared_ptr<Money> &money_);

    bool addAttackBuilding(AttackBuilding building);

    void heroSetPosition(const sf::Vector2f &newPos, std::size_t i = 0);
    void monsterSetScale(const sf::Vector2f &newScale, std::size_t i = 0);
    void monsterSetPosition(const sf::Vector2f &newPos, std::size_t i = 0);
    void heroSetScale(const sf::Vector2f &newScale, std::size_t i = 0);

    void updateStates();

    void draw(sf::RenderWindow &window);

    [[nodiscard]] std::vector<std::vector<Cell>> &getMap();

    const std::set<FreeObject> &getFreeObjects() const;

    const std::vector<std::shared_ptr<TemplateCharacter>> &getHeroes() const;
    const std::vector<std::shared_ptr<TemplateCharacter>> &getMonsters() const;
    const sf::Vector2f &getShift() const;
    const std::set<SupportBuilding> &getSupportBuildings() const;

    std::set<SupportBuilding> supportBuildings;
    std::vector<std::vector<Cell>> map;
    std::set<AttackBuilding> attackBuildings;
    std::vector<std::shared_ptr<TemplateCharacter>> heroes;
    std::vector<std::shared_ptr<TemplateCharacter>> monsters;
    std::set<FreeObject> freeObjects;
    std::vector<std::shared_ptr<Money>> money;
    std::list<FlyingObject> flyingObjects;

private:
    sf::Clock clock1;
    std::vector<Home> home;

    RectangleButton<void> menuGameButton;

    // skills
    std::vector<POWER_ELEMENT> elements;
    ABILITY ability;
    bool readyToCast = false;
    float maxMana = 100;
    mutable float mana = maxMana;
    float manaRegen = 100;
    sf::Time regenCooldown = sf::seconds(1);
    sf::Time lastRegenTime;
    float combineCost = 10;
    const std::map<ABILITY, float> abilityCost = {
        {ABILITY::FIRE_BLAST, 40},   {ABILITY::LAVA, 50},
        {ABILITY::FROZEN_BLAST, 80}, {ABILITY::EARTHSHAKE, 50},
        {ABILITY::WALL, 80},         {ABILITY::CLOUD, 30},
    };

    // Random trees
    sf::Time treeCooldown = sf::seconds(5);
    sf::Time lastTreeTime = sf::Time::Zero;
    // View
    float viewMoveSpeed = 20;
    sf::Vector2f shift = {0, 0};
};
}  // namespace jam