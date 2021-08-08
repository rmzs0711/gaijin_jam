#pragma once

#include <SFML/Graphics.hpp>
#include "moving_object.h"
#include <memory>
#include "button.h"

const std::size_t QUANTITY_PRODUCT = 3;

struct Product {
private:
	sf::Texture texture_money_icon;
	sf::Texture texture_product;
	sf::Sprite product;
	sf::Sprite money_icon;
	sf::Vector2i size_product;

	int cost;
	CentralisedText drawing_cost;

public:
	Product(sf::RenderWindow &window, int cost_, std::string file_name_product, sf::Vector2i size_product_ = sf::Vector2i(16, 16)) : cost(cost_), size_product(size_product_) {
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

	sf::Sprite getSprite() const {
		return product;
	}

	void move(sf::Vector2f position_) {
		product.move(position_);
		money_icon.move(position_);
		drawing_cost.move(position_);
	}

	void setScale(sf::RenderWindow& window, sf::Vector2f scale) {
		product.setScale(scale);

		money_icon.setPosition(window.mapPixelToCoords(sf::Vector2i(0, size_product.y)) + sf::Vector2f(0, 5));
		drawing_cost.setPosition(money_icon.getPosition() + sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(16, 0)).x * 1.7, 0) + sf::Vector2f(5, -3));

	}

	void drawProduct(sf::RenderWindow& window) {
		window.draw(product);
		window.draw(money_icon);
		window.draw(drawing_cost);
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
	}

	void drawStore(sf::RenderWindow& window) {
		window.draw(base);
		for (auto &i : products) {
			(*i).drawProduct(window);
		}
	}
};