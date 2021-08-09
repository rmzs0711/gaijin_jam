#pragma once

#include <SFML/Graphics.hpp>
#include "moving_object.h"
#include <memory>
#include "button.h"

struct Product {
private:
	sf::Texture texture_money_icon;
	sf::Texture texture_product;
	sf::Sprite product;
	sf::Sprite money_icon;
	sf::Vector2i size_product;
	std::string name_file;

	int cost;
	CentralisedText drawing_cost;

public:
	Product(sf::RenderWindow &window, int cost_, std::string file_name_product, sf::Vector2i size_product_ = sf::Vector2i(16, 16)) : name_file(file_name_product), cost(cost_), size_product(size_product_) {
		texture_product.loadFromFile(file_name_product);
		product.setTexture(texture_product);
		product.setTextureRect(sf::IntRect(0, 0, size_product.x, size_product.y));
		product.setScale(4.5, 4.5);
		texture_money_icon.loadFromFile("data/images/MiniWorldSprites/Miscellaneous/Chests.png");
		money_icon.setTexture(texture_money_icon);
		money_icon.setTextureRect(sf::IntRect(16, 0, 16, 16));
		money_icon.setScale(1.7, 1.7);
		money_icon.move(window.mapPixelToCoords(sf::Vector2i(0, size_product.y * 4.5)) + sf::Vector2f(0, 5));

		drawing_cost.setString(std::to_string(cost));
		drawing_cost.setFillColor(sf::Color(74, 53, 27));
		drawing_cost.move(money_icon.getPosition() + sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(16, 0)).x * 1.7, 0) + sf::Vector2f(5, -3));
	}


	int getCost() const {
		return cost;
	}

	/*sf::Sprite getSprite() const {
		return product;
	}*/

	sf::Texture* getTexture() {
		return &texture_product;
	}

	sf::Vector2f getPosition() const {
		return product.getPosition() + sf::Vector2f(16 * 4.5, 0);
	}

	void move(sf::Vector2f position_) {
		product.move(position_);
		money_icon.move(position_);
		drawing_cost.move(position_);
	}

	/*void setScale(sf::RenderWindow& window, sf::Vector2f scale) {
		product.setScale(scale);

		money_icon.setPosition(window.mapPixelToCoords(sf::Vector2i(0, size_product.y)) + sf::Vector2f(0, 5));
		drawing_cost.setPosition(money_icon.getPosition() + sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(16, 0)).x * 1.7, 0) + sf::Vector2f(5, -3));

	}*/

	std::string getFile() {
		return name_file;
	}

	sf::Vector2i getSizeProduct() {
		return size_product;
	}

	void drawProduct(sf::RenderWindow& window) {
		window.draw(product);
		window.draw(money_icon);
		window.draw(drawing_cost);
	}

	bool isCorrectClick(const sf::Vector2f& mouse) {
		return product.getGlobalBounds().contains(mouse);
	}
};

struct Store {
private:
	sf::Texture texture_base;
	sf::RectangleShape base;
	Object move_product;
	std::vector<std::unique_ptr<Product>> products;
public:

	Store(sf::RenderWindow& window) {
		texture_base.loadFromFile("data/images/textureBase.png");
		base.setTexture(&texture_base);
		base.setSize(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(window.getSize())).x, window.mapPixelToCoords(sf::Vector2i(window.getSize())).y / 7.4));
		base.setPosition(sf::Vector2f(0, window.mapPixelToCoords(sf::Vector2i(window.getSize())).y * 6.4 / 7.4));

		products.push_back(std::make_unique<Product>(window, 10, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/PurpleMelee/AssasinPurple.png"));
		(*products[0]).move(base.getPosition() + sf::Vector2f(20, 12));
		products.push_back(std::make_unique<Product>(window, 10, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/AssasinLime.png"));
		products.push_back(std::make_unique<Product>(window, 10, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/AssasinCyan.png"));
		products.push_back(std::make_unique<Product>(window, 10, "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/AssasinRed.png"));
		
		for (int i = 1; i < products.size(); i++) {
			(*products[i]).move((*products[i - 1]).getPosition() + sf::Vector2f(40, 0));
		}
	
	}

	void event(const sf::Event& event, sf::RenderWindow& window, sf::Vector2f &mouse, jam::Level &level) {
		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (!move_product.isValid()) {
				for (int i = 0; i < products.size(); i++) {
					if ((*products[i]).isCorrectClick(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)))) {
						move_product.loadFromFile((*products[i]).getFile(), (*products[i]).getSizeProduct());
						move_product.setPosition((*products[i]).getPosition() - sf::Vector2f(16 * 4.5, 0));
						move_product.setScale(4.5, 4.5);
						move_product.click_mouse_left(window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouse);
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
				level.addHero(makeAssasinPurple(level.getMap(), level.getfreeObjects(), move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/LimeMelee/AssasinLime.png") {
				level.addHero(makeAssasinLime(level.getMap(), level.getfreeObjects(), move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/CyanMelee/AssasinCyan.png") {
				level.addHero(makeAssasinCyan(level.getMap(), level.getfreeObjects(), move_product.getPosition()));
			}
			else if (file == "data/images/MiniWorldSprites/Characters/Soldiers/Melee/RedMelee/AssasinRed.png") {
				level.addHero(makeAssasinRed(level.getMap(), level.getfreeObjects(), move_product.getPosition()));
			}
			move_product.loadFromFile("");
		}
	}

	void drawStore(sf::RenderWindow& window) {
		window.draw(base);
		for (auto &i : products) {
			(*i).drawProduct(window);
		}
		move_product.draw(window);
	}
};