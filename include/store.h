#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Cell.h"
#include "Level.h"
#include "button.h"
#include "makeBuilding.h"
#include "moving_object.h"

struct AmountMoney {
private:
    sf::Texture texture_icon;
    sf::Sprite icon;
    int money;
    CentralisedText drawing_money;

public:
    AmountMoney(sf::RenderTarget &window) : money(500) {
        texture_icon.loadFromFile(
            "data/images/MiniWorldSprites/Miscellaneous/Chests.png");
        icon.setTexture(texture_icon);
        icon.setTextureRect(sf::IntRect(16, 0, 16, 16));
        icon.setScale(1.7, 1.7);
        drawing_money.setFillColor(sf::Color(74, 53, 27));
        drawing_money.move(
            sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(16, 0)).x * 1.7,
                         0) +
            sf::Vector2f(5, -3));
    }

    void setScale(sf::Vector2f scale) {
        icon.setScale({ icon.getScale().x * scale.x, icon.getScale().y * scale.y });
        drawing_money.setScale({ drawing_money.getScale().x * scale.x, drawing_money.getScale().y * scale.y });
    }

    void setMoney(int new_money) {
        money = new_money;
    }

    int getMoney() const {
        return money;
    }

    void changeMoney(int money_) {
        money += money_;
    }

    void move(sf::Vector2f position_) {
        icon.move(position_);
        drawing_money.move(position_);
    }

    void draw(sf::RenderTarget &window) {
        drawing_money.setString(std::to_string(money));
        window.draw(icon);
        window.draw(drawing_money);
    }
};

struct Product {
private:
    sf::Texture texture_product;
    sf::Sprite product;
    sf::Vector2i size_product;
    std::string name_file;
    AmountMoney cost;

public:
    Product(sf::RenderTarget &window,
            int cost_,
            std::string file_name_product,
            sf::Vector2i size_product_ = sf::Vector2i(16, 16))
        : cost(window),
          name_file(file_name_product),
          size_product(size_product_) {
        texture_product.loadFromFile(file_name_product);
        product.setTexture(texture_product);
        product.setTextureRect(
            sf::IntRect(0, 0, size_product.x, size_product.y));
        product.setScale(4.5, 4.5);
        cost.move(
            window.mapPixelToCoords(sf::Vector2i(0, size_product.y * 4.5)) +
            sf::Vector2f(0, 5));
        cost.setMoney(cost_);
    }

    int getCost() const {
        return cost.getMoney();
    }
    void setTextureRect(int left, int top) {
        product.setTextureRect(
            sf::IntRect(left, top, size_product.x, size_product.y));
    }

    sf::IntRect getTextureRect() {
        return product.getTextureRect();
    }

    sf::Texture *getTexture() {
        return &texture_product;
    }

    sf::Vector2f getPosition() const {
        return product.getPosition() + sf::Vector2f(16 * 4.5, 0);
    }

    void move(sf::Vector2f position_) {
        product.move(position_);
        cost.move(position_);
    }

    std::string getFile() {
        return name_file;
    }

    sf::Vector2i getSizeProduct() {
        return size_product;
    }

    void drawProduct(sf::RenderTarget &window) {
        window.draw(product);
        cost.draw(window);
    }

    bool isCorrectClick(const sf::Vector2f &mouse) {
        return product.getGlobalBounds().contains(mouse);
    }
};

struct Message {
private:
    std::string message;
    CentralisedText drawing_message;
    int count;

public:
    void changeCount(int count_) {
        count += count_;
    }

    Message(sf::RenderTarget &window) : message(""), count(0) {
        drawing_message.setFillColor(sf::Color(176, 0, 0));
        drawing_message.move(sf::Vector2f(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2,
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).y * 5.4 /
                7.4));
    }

    void setMessage(std::string message_) {
        count = 800;
        message = message_;
    }

    void draw(sf::RenderTarget &window) {
        if (count > 0) {
            drawing_message.setString(message);
            drawing_message.centralise();
            window.draw(drawing_message);
            count--;
        }
    }
};

struct Store {
private:
    sf::Texture texture_base;
    sf::RectangleShape base;
    Object move_product;
    std::vector<std::unique_ptr<Product>> products;
    AmountMoney money;
    Message message;
    bool &is_active;
    int cost;

    void add(bool condition) {
        if (condition) {
            money.changeMoney(-cost);
        } else if (cost != 0){
            message.setMessage("You can't put an object here");
        }
        cost = 0;
    }

public:
    Store(sf::RenderTarget &window, bool &is_active_)
        : is_active(is_active_), money(window), message(window), cost(0) {
        texture_base.loadFromFile("data/images/textureBase.png");
        base.setTexture(&texture_base);
        base.setSize(sf::Vector2f(
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).x,
            window.mapPixelToCoords(sf::Vector2i(window.getSize())).y / 7.4));
        base.setPosition(sf::Vector2f(
            0, window.mapPixelToCoords(sf::Vector2i(window.getSize())).y * 6.4 /
                   7.4));

        products.push_back(std::make_unique<Product>(
            window, 15,
            "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
            "PurpleMelee/AssasinPurple.png"));
        products[0]->move(base.getPosition() + sf::Vector2f(20, 15));
        products.push_back(std::make_unique<Product>(
            window, 20,
            "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/"
            "AssasinLime.png"));
        products.push_back(std::make_unique<Product>(
            window, 25,
            "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/"
            "AssasinCyan.png"));
        products.push_back(std::make_unique<Product>(
            window, 30,
            "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/"
            "AssasinRed.png"));
        products.push_back(std::make_unique<Product>(
            window, 150,
            "data/images/MiniWorldSprites/Buildings/Lime/LimeTower.png"));
        products[products.size() - 1]->setTextureRect(0, 16);
        products.push_back(std::make_unique<Product>(
            window, 150,
            "data/images/MiniWorldSprites/Buildings/Red/RedTower.png"));
        products[products.size() - 1]->setTextureRect(0, 16);
        products.push_back(std::make_unique<Product>(
            window, 150,
            "data/images/MiniWorldSprites/Buildings/Cyan/CyanTower.png"));
        products[products.size() - 1]->setTextureRect(0, 16);
        products.push_back(std::make_unique<Product>(
            window, 125,
            "data/images/MiniWorldSprites/Miscellaneous/Well"
            ".png"));
        products[products.size() - 1]->setTextureRect(0, 16);
        products.push_back(std::make_unique<Product>(
            window, 125,
            "data/images/MiniWorldSprites/Buildings/Wood/CaveV2.png"));
        products[products.size() - 1]->setTextureRect(0, 0);
        products.push_back(std::make_unique<Product>(
            window, 200,
            "data/images/MiniWorldSprites/Buildings/Cyan/CyanChapels.png"));
        products[products.size() - 1]->setTextureRect(16, 16);
        products.push_back(std::make_unique<Product>(
            window, 200,
            "data/images/MiniWorldSprites/Buildings/Wood/Barracks.png"));
        products[products.size() - 1]->setTextureRect(0, 0);

        for (int i = 1; i < products.size(); i++) {
            products[i]->move(products[i - 1]->getPosition() +
                              sf::Vector2f(50, 0));
        }
        money.move(sf::Vector2f(jam::cellSize + 10, jam::cellSize / 3));
        money.setScale(sf::Vector2f(1.4, 1.4));
    }

    void event(const sf::Event &event,
               sf::RenderTarget &window,
               sf::Vector2f &mouse,
               jam::Level &level) {
        if (!is_active) {
            return;
        }
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            if (!move_product.isValid()) {
                for (int i = 0; i < products.size(); i++) {
                    if ((*products[i])
                            .isCorrectClick(window.mapPixelToCoords(
                                sf::Vector2i(event.mouseButton.x,
                                             event.mouseButton.y)))) {
                        if (money.getMoney() >= (*products[i]).getCost()) {
                            move_product.loadFromFile(
                                (*products[i]).getFile(),
                                (*products[i]).getTextureRect(),
                                (*products[i]).getSizeProduct());
                            move_product.setPosition(
                                (*products[i]).getPosition() -
                                sf::Vector2f(16 * 4.5, 0));
                            move_product.setScale(4.5, 4.5);
                            move_product.click_mouse_left(
                                window.mapPixelToCoords(sf::Vector2i(
                                    event.mouseButton.x, event.mouseButton.y)),
                                mouse);
                            cost = (*products[i]).getCost();
                        } else {
                            message.setMessage(
                                "Insufficient funds for the purchase");
                        }
                        break;
                    }
                }
            } else {
                move_product.click_mouse_left(
                    window.mapPixelToCoords(
                        sf::Vector2i(event.mouseButton.x, event.mouseButton.y)),
                    mouse);
            }
        } else if (event.type == sf::Event::MouseMoved) {
            move_product.move_object(window.mapPixelToCoords(sf::Vector2i(
                                         event.mouseMove.x, event.mouseMove.y)),
                                     mouse);
        } else if (event.type == sf::Event::MouseButtonReleased) {
            std::string file = move_product.getNameFile();
            auto pos = jam::toMapPosition(
                window, level.getShift() + move_product.getPosition());
            pos.x = bounds(pos.x, 0, (int)level.map[0].size());
            pos.y = bounds(pos.y, 0, (int)level.map.size());
            if (file ==
                "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
                "PurpleMelee/AssasinPurple.png") {
                add(level.addHero(Hero::makeAssasinPurple(
                    level, level.getShift() + move_product.getPosition())));
            } else if (file ==
                       "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
                       "LimeMelee/AssasinLime.png") {
                add(level.addHero(Hero::makeAssasinLime(
                    level, level.getShift() + move_product.getPosition())));
            } else if (file ==
                       "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
                       "CyanMelee/AssasinCyan.png") {
                add(level.addHero(Hero::makeAssasinCyan(
                    level, level.getShift() + move_product.getPosition())));
            } else if (file ==
                       "data/images/MiniWorldSprites/Characters/Soldiers/Melee/"
                       "RedMelee/AssasinRed.png") {
                add(level.addHero(Hero::makeAssasinRed(
                    level, level.getShift() + move_product.getPosition())));
            } else if (level.getMap()[pos.y][pos.x].getBackgroundType() ==
                     jam::ROAD) {
                add(false);
            } else if (file ==
                       "data/images/MiniWorldSprites/Buildings/Lime/"
                       "LimeTower.png") {
                add(level.addAttackBuilding(makeArcherBuilding(level, pos)));
            } else if (file ==
                       "data/images/MiniWorldSprites/Buildings/Red/"
                       "RedTower.png") {
                add(level.addAttackBuilding(makeSniperBuilding(level, pos)));
            } else if (file ==
                       "data/images/MiniWorldSprites/Buildings/Cyan/"
                       "CyanTower.png") {
                add(level.addAttackBuilding(makeWizardTower(level, pos)));
            } else if (file ==
                       "data/images/MiniWorldSprites/Miscellaneous/Well.png") {
                if (level.map[pos.y][pos.x].getState() == jam::EARTHSHAKE) {
                    add(level.addSupportBuilding(jam::makeWell(level, pos)));
                } else {
                    add(false);
                }
            } else if (file ==
                       "data/images/MiniWorldSprites/Buildings/Cyan"
                       "/CyanChapels.png") {
                if (level.getMap()[pos.y][pos.x].getBackgroundType() ==
                        jam::DARK_GREEN_GRASS ||
                    level.getMap()[pos.y][pos.x].getBackgroundType() ==
                        jam::LIGHT_GREEN_GRASS) {
                    add(level.addSupportBuilding(jam::makeHospital(
                        level, jam::toMapPosition(
                                   window, level.getShift() +
                                               move_product.getPosition()))));
                } else {
                    add(false);
                }

            } else if (file ==
                       "data/images/MiniWorldSprites/Buildings/Wood/"
                       "CaveV2.png") {
                if (level.map[pos.y][pos.x].getState() == jam::WALL) {
                    level.map[pos.y][pos.x].setState(jam::NORMAL);
                    level.supportBuildings.insert(
                        jam::makeMinerCave(level, pos));
                    add(true);
                } else {
                    add(false);
                }
            } else if (file ==
                       "data/images/MiniWorldSprites/Buildings/Wood/"
                       "Barracks.png") {
                add(level.addSupportBuilding(jam::makeBarrack(level, pos)));
            }
            move_product.loadFromFile("");
        }
    }

    void addMoney(int money_) {
        money.changeMoney(money_);
    }

    void drawStore(sf::RenderTarget &window) {
        if (!is_active) {
            message.changeCount(-1);
            return;
        }
        window.draw(base);
        for (auto &i : products) {
            (*i).drawProduct(window);
        }
        move_product.draw(window);
        money.draw(window);
        message.draw(window);
    }
};