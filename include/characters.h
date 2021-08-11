#pragma once

#include <cmath>
#include <list>
#include <memory>
#include <set>
#include <vector>
#include "building.h"
#include "SFML/Graphics.hpp"
#include "makeFreeObjects.h"
#include "usefulFunctions.h"


struct LifeBarCharacter {
    sf::RectangleShape original;
    sf::RectangleShape current;
    float scale;
    float health;

    LifeBarCharacter(sf::RenderWindow& window, float health_) : scale(0), health(health_) {
        original.setSize(window.mapPixelToCoords(sf::Vector2i(32, 8)));
        current.setSize(window.mapPixelToCoords(sf::Vector2i(32, 8)));
        original.setFillColor(sf::Color(74, 53, 27));
        current.setFillColor(sf::Color::Green);
    }

    void changeCurrentHealth(sf::RenderWindow& window, float health_) {
        current.setSize(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(32, 0)).x * health_ / health,
            window.mapPixelToCoords(sf::Vector2i(0, 8)).y));
       // std::cout << health << " " << health_ << " " << current.getSize().x << " " << current.getSize().y << '\n';
    }

    void draw(sf::RenderWindow& window, float health_, sf::Vector2f position) {
        setPosition(window, position);
        changeCurrentHealth(window, health_);
        window.draw(original);
        window.draw(current);
    }

    void setPosition(sf::RenderWindow& window, sf::Vector2f position) {
        original.setPosition(position + sf::Vector2f(scale * window.mapPixelToCoords(sf::Vector2i(16, 0)).x / 2, -5));
        current.setPosition(position + sf::Vector2f(scale * window.mapPixelToCoords(sf::Vector2i(16, 0)).x / 2, -5));
    }

    void setScale(float scale_) {
        scale = scale_ - 2;
    }
};

int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2, FIGHTING = 4,
          NOT_FIGHTING = -1;
int const BURNED = -2, FROZEN = -3, SLOWED = -4, STUNNED = -5;
enum POWER_ELEMENT { FIRE, ICE, EARTH, NUMBER_OF_POWER_ELEMENTS };
enum ABILITY { FIRE_BLAST, CLOUD, LAVA, FROZEN_BLAST, WALL, EARTHSHAKE };

namespace jam {
struct FlyingObject;
struct Level;
};

struct TemplateCharacter {
    friend jam::FlyingObject;
protected:
    LifeBarCharacter life_bar;
    bool isCorrectMove();
    float speedCoef = 1;
    float speed;
    sf::Vector2f scale;
    int current_frame, quantity_frames;
    sf::Vector2i size_frame;
    float health, damage, current_health, current_damage;
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon; // icon position!
    jam::Level &curLevel;

    TemplateCharacter(sf::RenderWindow& window, const std::string &file_name,
                      int quantity_frames_,
                      sf::Vector2i size_frame_,
                      float health_,
                      float damage_,
                      jam::Level &curLevel_)
        : curLevel(curLevel_),
          current_frame(0),
          size_frame(size_frame_),
          quantity_frames(quantity_frames_),
          speed(0.2),
          current_health(health_),
          current_damage(damage_),
          health(health_),
          damage(damage_),
          scale(sf::Vector2f(0, 0)), life_bar(window, health_) {
        sf::Clock clock;
        sf::Image character_image;
        character_image.loadFromFile(file_name);
        character_texture.loadFromImage(character_image);
        character.setTexture(character_texture);
        character.setOrigin((float)size_frame.x / 2,
                            (float)size_frame.y * 0.9f);
        sf::Image icon_image;
        icon_image.loadFromFile(
            "data/images/MiniWorldSprites/Objects/FireballProjectile.png");
        icon_texture.loadFromImage(icon_image);
        icon.setTexture(icon_texture);
        icon.setOrigin(character.getOrigin());

        character.setTextureRect(sf::IntRect(0, 0, size_frame.x, size_frame.y));
        icon.setTextureRect(sf::IntRect(
            (static_cast<int>(clock.getElapsedTime().asMicroseconds()) % 4) *
                size_frame.x,
            0, size_frame.x, size_frame.y));
    }

    void initializingCoordinates(float &dx, float &dy, int direction) const;
public:
    sf::Sprite *getSprite();

    virtual void setPosition(float x, float y);

    virtual void setPosition(sf::Vector2f position_);

    void setSpeed(float speed_);

    void setScale(float x, float y);

    void setScale(sf::Vector2f scale_);

    float getDamage();

    bool isLive();

    bool isDraw();

    virtual void drawCharacter(sf::RenderWindow &window) = 0;
};
bool charactersCompare (const std::shared_ptr<TemplateCharacter> &,
                    const std::shared_ptr<TemplateCharacter> &);

std::shared_ptr<TemplateCharacter> intersectionObjects(
    const sf::Sprite &character,
    const std::vector<std::shared_ptr<TemplateCharacter>>
    &objects);

struct Monster : TemplateCharacter {
protected:
    int state;
    std::vector<sf::Vector2f> positions;
    int money;

    void changeState(int state_, float damage_ = 0);

    void isFighting();

    void death();
    void isEffected();

    void moveToPosition();

public:
    Monster(sf::RenderWindow& window, const std::string &file_name,
            float health_,
            float damage_,
            std::vector<sf::Vector2f> &positions_,
            jam::Level &curLevel_, int money_ = 0,
            int quantity_frames_ = 4,
            sf::Vector2i size_frame_ = sf::Vector2i(16, 16))
        : TemplateCharacter(window, file_name,
                            quantity_frames_,
                            size_frame_,
                            health_,
                            damage_,
                            curLevel_),
          state(DOWN),
          positions(positions_), money(money_) {}

    int getState() const;

    void takeDamage(float damage_);

    void drawCharacter(sf::RenderWindow &window) override;

    static std::shared_ptr<Monster> makeArmouredRedDemon(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeRedDemon(sf::RenderWindow& window, jam::Level &level,
                                          std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePurpleDemon(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    // makeFrostborn

    static std::shared_ptr<Monster> makeMammoth(sf::RenderWindow& window, jam::Level &level,
                                         std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeWendigo(sf::RenderWindow& window, jam::Level &level,
                                         std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeYeti(sf::RenderWindow& window, jam::Level& level,
                                      std::vector<sf::Vector2f> &monster_path);


    static std::shared_ptr<Monster> makeArcherGoblin(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeClubGoblin(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeFarmerGoblin(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeKamikazeGoblin(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeOrc(sf::RenderWindow& window, jam::Level& level,
                                     std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeOrcMage(sf::RenderWindow& window, jam::Level &level,
                                         std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeOrcShaman(sf::RenderWindow& window, jam::Level &level,
                                           std::vector<sf::Vector2f> &monster_path);

    // makePirates

    static std::shared_ptr<Monster> makePirateCaptain(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePirateGrunt(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePirateGunnern(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    // makeUndead

    static std::shared_ptr<Monster> makeNecromancer(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeSkeletonSoldier(sf::RenderWindow& window,
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);


};

struct Hero : TemplateCharacter {
protected:
    std::vector<POWER_ELEMENT> elements;
    ABILITY ability;
    bool readyToCast = false;

    int state;
    bool is_move, is_always_move;
    sf::Vector2f position;

    bool isCorrectClick(const sf::Vector2f &mouse);

    void isFighting();

    void keyPressed(const sf::Event &event);

    void death();

    void clickMouse(const sf::Event &event,
                    sf::RenderWindow &window,
                    const sf::Time &currentTime);

    void changeState(int state_, float damage_ = 0);

    void moveToPosition();

public:
    Hero(sf::RenderWindow& window, const std::string &file_name,
         float health_,
         float damage_,
         jam::Level &curLevel,
         bool is_always_move_ = true,
         int quantity_frames_ = 4,
         sf::Vector2i size_frame_ = sf::Vector2i(16, 16))
        : TemplateCharacter(window, file_name,
                            quantity_frames_,
                            size_frame_,
                            health_,
                            damage_,
                            curLevel),

          ability(CLOUD),
          elements(2, POWER_ELEMENT::FIRE),
          state(DOWN),
          is_always_move(is_always_move_),
          is_move(is_always_move_),
          position(sf::Vector2f(0, 0)) {}

    void setPosition(float x, float y) override;

    void setPosition(sf::Vector2f position_) override;

    void event(const sf::Event &event,
               sf::RenderWindow &window,
               const sf::Time &currentTime);

    void drawCharacter(sf::RenderWindow& window) override;
    static std::shared_ptr<Hero> makeAssasinPurple(sf::RenderWindow& window,
        jam::Level &level,
        sf::Vector2f position = sf::Vector2f(0, 0));

    static std::shared_ptr<Hero> makeAssasinLime(sf::RenderWindow& window, jam::Level &level,
                                          sf::Vector2f position = sf::Vector2f(0,
                                                                               0));

    static std::shared_ptr<Hero> makeAssasinCyan(sf::RenderWindow& window, jam::Level &level,
                                          sf::Vector2f newPosition = sf::Vector2f(0,
                                                                               0));

    static std::shared_ptr<Hero> makeAssasinRed(sf::RenderWindow& window, jam::Level &level,
                                         sf::Vector2f position = sf::Vector2f(0,
                                                                              0));

};
// makeHero


