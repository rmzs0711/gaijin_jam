#pragma once

#include <SFML/Graphics.hpp>
#include "moving_object.h"
#include <memory>
#include "button.h"
#include "Level.h"
#include "Cell.h"
#include "makeAttackBuilding.h"

struct AmountMoney {
private:
	sf::Texture texture_icon;
	sf::Sprite icon;
	int money;
	CentralisedText drawing_money;
public:
	AmountMoney(sf::RenderWindow& window) : money(100) {
		texture_icon.loadFromFile("data/images/MiniWorldSprites/Miscellaneous/Chests.png");
		icon.setTexture(texture_icon);
		icon.setTextureRect(sf::IntRect(16, 0, 16, 16));
		icon.setScale(1.7, 1.7);
		drawing_money.setFillColor(sf::Color(74, 53, 27));
		drawing_money.move(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(16, 0)).x * 1.7, 0) + sf::Vector2f(5, -3));
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

	void draw(sf::RenderWindow& window) {
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
	Product(sf::RenderWindow &window, int cost_, std::string file_name_product, sf::Vector2i size_product_ = sf::Vector2i(16, 16)) : cost(window), name_file(file_name_product), size_product(size_product_) {
		texture_product.loadFromFile(file_name_product);
		product.setTexture(texture_product);
		product.setTextureRect(sf::IntRect(0, 0, size_product.x, size_product.y));
		product.setScale(4.5, 4.5);
		cost.move(window.mapPixelToCoords(sf::Vector2i(0, size_product.y * 4.5)) + sf::Vector2f(0, 5));
		cost.setMoney(cost_);
	}


	int getCost() const {
		return cost.getMoney();
	}

	sf::Texture* getTexture() {
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

	void drawProduct(sf::RenderWindow& window) {
		window.draw(product);
		cost.draw(window);
	}

	bool isCorrectClick(const sf::Vector2f& mouse) {
		return product.getGlobalBounds().contains(mouse);
	}
};

struct Message {
private:
	std::string message;
	CentralisedText drawing_message;
	int count;
public:
	Message(sf::RenderWindow& window) : message("") {
		drawing_message.setFillColor(sf::Color(176, 0, 0));
		drawing_message.move(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(window.getSize())).x / 2,
			window.mapPixelToCoords(sf::Vector2i(window.getSize())).y * 5.4 / 7.4));
	}

	void setMessage(std::string message_) {
		count = 800;
		message = message_;
	}

	void draw(sf::RenderWindow& window) {
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
public:

	Store(sf::RenderWindow& window) : money(window), message(window) {
		texture_base.loadFromFile("data/images/textureBase.png");
		base.setTexture(&texture_base);
		base.setSize(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(window.getSize())).x, window.mapPixelToCoords(sf::Vector2i(window.getSize())).y / 7.4));
		base.setPosition(sf::Vector2f(0, window.mapPixelToCoords(sf::Vector2i(window.getSize())).y * 6.4 / 7.4));

		products.push_back(std::make_unique<Product>(window, 15, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png"));
		(*products[0]).move(base.getPosition() + sf::Vector2f(20, 12));
		products.push_back(std::make_unique<Product>(window, 20, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/AssasinLime.png"));
		products.push_back(std::make_unique<Product>(window, 25, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/AssasinCyan.png"));
		products.push_back(std::make_unique<Product>(window, 30, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/AssasinRed.png"));
		products.push_back(std::make_unique<Product>(window, 50, "data/images/MiniWorldSprites/Buildings/Lime/LimeTower.png"));
		
		for (int i = 1; i < products.size(); i++) {
			(*products[i]).move((*products[i - 1]).getPosition() + sf::Vector2f(40, 0));
		}
		money.move(sf::Vector2f(10, 10));
	}

	void event(const sf::Event& event, sf::RenderWindow& window, sf::Vector2f &mouse, jam::Level &level) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (!move_product.isValid()) {
				for (int i = 0; i < products.size(); i++) {
					if ((*products[i]).isCorrectClick(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
						if (money.getMoney() >= (*products[i]).getCost()) {
							move_product.loadFromFile((*products[i]).getFile(), (*products[i]).getSizeProduct());
							move_product.setPosition((*products[i]).getPosition() - sf::Vector2f(16 * 4.5, 0));
							move_product.setScale(4.5, 4.5);
							move_product.click_mouse_left(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
							money.changeMoney(-(*products[i]).getCost());
						}
						else {
							message.setMessage("Insufficient funds for the purchase");
						}
						break;
					}
				}
			} 
			else {
				move_product.click_mouse_left(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
			}
		}
		else if (event.type == sf::Event::MouseMoved) {
			move_product.move_object(window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)), mouse);
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			std::string file = move_product.getNameFile();
			if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png") {
				level.addHero(Hero::makeAssasinPurple(window, level,
                                                                  move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/AssasinLime.png") {
				level.addHero(Hero::makeAssasinLime(window, level,
                                                                move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/AssasinCyan.png") {
				level.addHero(Hero::makeAssasinCyan(window, level,
                                                                move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/AssasinRed.png") {
				level.addHero(Hero::makeAssasinRed(window, level,
                                                               move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Buildings/Lime/LimeTower.png") {
				level.addAttackBuilding(makeArcherBuilding(level, jam::toMapPosition(window, move_product.getPosition())));
			}
			move_product.loadFromFile("");
		}
	}

	void addMoney(int money_) {
		money.changeMoney(money_);
	}

	void drawStore(sf::RenderWindow& window) {
		window.draw(base);
		for (auto &i : products) {
			(*i).drawProduct(window);
		}
		move_product.draw(window);
		money.draw(window);
		message.draw(window);
	}
};