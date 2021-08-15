#pragma once

#include <cmath>
#include <list>
#include <memory>
#include <set>
#include <vector>
#include "Cell.h"
#include "SFML/Graphics.hpp"
#include "building.h"
#include "makeFreeObjects.h"
#include "usefulFunctions.h"
inline const int maxHeroes = 200;
inline const int maxMonsters = 300;
inline const int maxMoneys = 100;
inline const int maxObjects = 1000;

int const UP = 1, DOWN = 0, LEFT = 3, RIGHT = 2, FIGHTING = 4,
          NOT_FIGHTING = -1;

inline sf::Vector2i nexCell(std::vector<std::vector<jam::Cell>> &map,
                            sf::Vector2i first,
                            sf::Vector2i second) {
    if (first == second) {
        return second;
    }
    sf::Vector2i ans;
    if (abs(first.x - second.x) > abs(first.y - second.y)) {
        if (first.x > second.x) {
            ans = first - sf::Vector2i(1, 0);
            if (map[ans.y][ans.x].getBackgroundType() !=
                jam::CellBackground::ROAD) {
                if (first.y > second.y) {
                    ans = first - sf::Vector2i(0, 1);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0) {
                            ans = first + sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first + sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(0, 1);
                            }
                        }
                    }
                } else {
                    ans = first + sf::Vector2i(0, 1);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0) {
                            ans = first - sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first + sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(0, 1);
                            }
                        }
                    }
                }
            }
        } else {
            ans = first + sf::Vector2i(1, 0);
            if (map[ans.y][ans.x].getBackgroundType() !=
                jam::CellBackground::ROAD) {
                if (first.y > second.y) {
                    ans = first - sf::Vector2i(0, 1);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0 &&
                            ans.y < 49) {
                            ans = first + sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first - sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                    jam::CellBackground::ROAD &&
                                ans.y < 49) {
                                ans = first + sf::Vector2i(0, 1);
                            }
                        }
                    }
                } else {
                    ans = first + sf::Vector2i(0, 1);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0 &&
                            ans.y > 0) {
                            ans = first - sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first - sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                    jam::CellBackground::ROAD &&
                                ans.y > 0) {
                                ans = first - sf::Vector2i(0, 1);
                            }
                        }
                    }
                }
            }
        }
    } else {
        if (first.y > second.y) {
            ans = first - sf::Vector2i(0, 1);
            if (map[ans.y][ans.x].getBackgroundType() !=
                jam::CellBackground::ROAD) {
                if (first.x > second.x) {
                    ans = first - sf::Vector2i(1, 0);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0) {
                            ans = first + sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(0, 1);
                            }
                        } else {
                            ans = first + sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(1, 0);
                            }
                        }
                    }
                } else {
                    ans = first + sf::Vector2i(1, 0);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0) {
                            ans = first + sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first - sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(0, 1);
                            }
                        }
                    }
                }
            }
        } else {
            ans = first + sf::Vector2i(0, 1);
            if (map[ans.y][ans.x].getBackgroundType() !=
                jam::CellBackground::ROAD) {
                if (first.x > second.x) {
                    ans = first - sf::Vector2i(1, 0);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0) {
                            ans = first - sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first + sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first + sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(0, 1);
                            }
                        }
                    }
                } else {
                    ans = first + sf::Vector2i(1, 0);
                    if (map[ans.y][ans.x].getBackgroundType() !=
                        jam::CellBackground::ROAD) {
                        if (rand() % 2 == 0) {
                            ans = first - sf::Vector2i(0, 1);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(1, 0);
                            }
                        } else {
                            ans = first - sf::Vector2i(1, 0);
                            if (map[ans.y][ans.x].getBackgroundType() !=
                                jam::CellBackground::ROAD) {
                                ans = first - sf::Vector2i(0, 1);
                            }
                        }
                    }
                }
            }
        }
    }
    return ans;
}

inline std::vector<sf::Vector2f> getPathMonster(
    std::vector<std::vector<jam::Cell>> &map,
    sf::Vector2i home,
    sf::Vector2i home_monsters) {
    std::vector<sf::Vector2i> map_path;
    sf::Vector2i next;

    if (home_monsters.y < 49 &&
        map[home_monsters.y + 1][home_monsters.x].getBackgroundType() ==
            jam::CellBackground::ROAD) {
        next = home_monsters + sf::Vector2i(0, 1);
    } else if (home_monsters.y > 0 &&
               map[home_monsters.y - 1][home_monsters.x].getBackgroundType() ==
                   jam::CellBackground::ROAD) {
        next = home_monsters - sf::Vector2i(0, 1);
    } else if (home_monsters.x < 49 &&
               map[home_monsters.y][home_monsters.x + 1].getBackgroundType() ==
                   jam::CellBackground::ROAD) {
        next = home_monsters + sf::Vector2i(1, 0);
    } else {
        next = home_monsters - sf::Vector2i(1, 0);
    }

    int t = 0;
    while (next != home && t < 6000) {
        t++;
        map_path.push_back(next);
        next = nexCell(map, next, home);
    }
    if (t == 6000) {
        std::vector<sf::Vector2f> path;
        return path;
    }

    map_path.push_back(home);

    std::vector<sf::Vector2f> path;

    for (int i = map_path.size() - 1; i >= 0; i--) {
        path.push_back(jam::toPosition(map_path[i]));
    }

    return path;
}

struct LifeBarCharacter {
    sf::RectangleShape original;
    sf::RectangleShape current;
    float scale;
    float health;

    LifeBarCharacter(float health_) : scale(0), health(health_) {
        original.setFillColor(sf::Color(74, 53, 27));
        current.setFillColor(sf::Color::Green);
        original.setSize(sf::Vector2f(sf::Vector2i(32, 8)));
    }

    void changeCurrentHealth(sf::RenderTarget &window, float health_) {
        current.setSize(sf::Vector2f(sf::Vector2i(32 * health_ / health, 8)));
    }

    void draw(sf::RenderTarget &window, float health_, sf::Vector2f position) {
        setPosition(window, position);
        changeCurrentHealth(window, health_);
        window.draw(original);
        window.draw(current);
    }

    void setPosition(sf::RenderTarget &window, sf::Vector2f position) {
        original.setPosition(position -
                             sf::Vector2f(sf::Vector2i(16, scale * 16 + 5)));
        current.setPosition(original.getPosition());
    }

    void setScale(float scale_) {
        scale = scale_;
    }
};

int const BURNED = -2, FROZEN = -3, SLOWED = -4, STUNNED = -5;
enum POWER_ELEMENT { FIRE, ICE, EARTH, NUMBER_OF_POWER_ELEMENTS };
enum ABILITY {
    FIRE_BLAST,
    CLOUD,
    LAVA,
    FROZEN_BLAST,
    WALL,
    EARTHSHAKE,
    NUMBER_OF_ABILITIES
};

namespace jam {
struct FlyingObject;
struct Level;
};  // namespace jam

struct TemplateCharacter {
    friend jam::FlyingObject;

    float speedCoef = 1;
    float health, damage, current_health, current_damage, max_health;
protected:
    float speed;
    LifeBarCharacter life_bar;
    sf::Vector2f scale;
    int current_frame, quantity_frames;
    sf::Vector2i size_frame;
    sf::Texture character_texture, icon_texture;
    sf::Sprite character, icon;
    jam::Level &curLevel;

    TemplateCharacter(const std::string &file_name,
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
          scale(sf::Vector2f(0, 0)),
          life_bar(health_),
          max_health(health_) {
        if (file_name == "") {
            return;
        }
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
        icon.setTextureRect(sf::IntRect((rand() % 4) * size_frame.x,
            0, size_frame.x, size_frame.y));
    }
    void initializingCoordinates(float &dx, float &dy, int direction) const;

public:
    virtual void updateState() = 0;
    sf::FloatRect getGlobalBounds() const;
    virtual bool isCorrectMove();
    void takeDamage(float damage_);

    sf::Sprite *getSprite();

    virtual void setPosition(float x, float y);

    virtual void setPosition(sf::Vector2f position_);

    void setSpeed(float speed_);

    void setScale(float x, float y);

    void setScale(sf::Vector2f scale_);

    float getDamage();

    bool isLive();

    bool isDraw();

    virtual void drawCharacter(sf::RenderTarget &window) = 0;
};
bool charactersCompare(const std::shared_ptr<TemplateCharacter> &,
                       const std::shared_ptr<TemplateCharacter> &);

std::shared_ptr<TemplateCharacter> intersectionObjects(
    const sf::Sprite &character,
    const std::vector<std::shared_ptr<TemplateCharacter>> &objects,
    jam::Level &);

struct Monster : TemplateCharacter {
protected:
    int state;
    std::vector<sf::Vector2f> positions;
    int money;

    void changeState(int state_, float damage_ = 0);

    void isFighting();

    void isEffected();
    void moveToPosition();

public:
    bool isCorrectMove() override;
    void death();
    void updateState() override;
    Monster(const std::string &file_name,
            float health_,
            float damage_,
            std::vector<sf::Vector2f> &positions_,
            jam::Level &curLevel_,
            int money_ = 0,
            int quantity_frames_ = 4,
            sf::Vector2i size_frame_ = sf::Vector2i(16, 16))
        : TemplateCharacter(file_name,
                            quantity_frames_,
                            size_frame_,
                            health_,
                            damage_,
                            curLevel_),
          state(DOWN),
          positions(positions_),
          money(money_) {}

    int getState() const;

    void drawCharacter(sf::RenderTarget &window) override;

    static std::shared_ptr<Monster> makeArmouredRedDemon(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeRedDemon(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePurpleDemon(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeMammoth(
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeWendigo(
        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeYeti(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeArcherGoblin(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeClubGoblin(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeFarmerGoblin(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeKamikazeGoblin(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeOrc(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeOrcMage(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeOrcShaman(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePirateCaptain(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePirateGrunt(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makePirateGunnern(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeNecromancer(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);

    static std::shared_ptr<Monster> makeSkeletonSoldier(

        jam::Level &level,
        std::vector<sf::Vector2f> &monster_path);
};

struct Hero : TemplateCharacter {
protected:
    int state;
    bool is_move, is_always_move;
    sf::Vector2f position;

    bool isCorrectClick(const sf::Vector2f &mouse);

    void isFighting();

    void keyPressed(const sf::Event &event);

    void death();

    void clickMouse(const sf::Event &event,
                    sf::RenderTarget &window,
                    const sf::Time &currentTime);

    void changeState(int state_, float damage_ = 0);

    void moveToPosition();

public:
    int getState() const;
    void updateState() override;

    Hero(const std::string &file_name,
         float health_,
         float damage_,
         jam::Level &curLevel,
         bool is_always_move_ = true,
         int quantity_frames_ = 4,
         sf::Vector2i size_frame_ = sf::Vector2i(16, 16))
        : TemplateCharacter(file_name,
                            quantity_frames_,
                            size_frame_,
                            health_,
                            damage_,
                            curLevel),
          state(DOWN),
          is_always_move(is_always_move_),
          is_move(is_always_move_),
          position(sf::Vector2f(0, 0)) {}

    void setPosition(float x, float y) override;

    void setPosition(sf::Vector2f position_) override;

    void event(const sf::Event &event,
               sf::RenderTarget &window,
               const sf::Time &currentTime);

    void drawCharacter(sf::RenderTarget &window) override;

    static std::shared_ptr<Hero> makeEmptyHero(
        jam::Level &level,
        sf::Vector2f position = sf::Vector2f(0, 0));
    static std::shared_ptr<Hero> makeAssasinPurple(
        jam::Level &level,
        sf::Vector2f position = sf::Vector2f(0, 0));

    static std::shared_ptr<Hero> makeAssasinLime(
        jam::Level &level,
        sf::Vector2f position = sf::Vector2f(0, 0));

    static std::shared_ptr<Hero> makeAssasinCyan(
        jam::Level &level,
        sf::Vector2f newPosition = sf::Vector2f(0, 0));

    static std::shared_ptr<Hero> makeAssasinRed(
        jam::Level &level,
        sf::Vector2f position = sf::Vector2f(0, 0));
};
