#include "SFML/Graphics.hpp"
#include "include/Cell.h"

std::map<jam::CellBackground, std::string> mapa = {
    {jam::CellBackground::LIGHT_GREEN_GRASS, "LIGHT_GREEN_GRASS"},
    {jam::CellBackground::DARK_GREEN_GRASS, "DARK_GREEN_GRASS"},
    {jam::CellBackground::DEAD_GRASS, "DEAD_GRASS"},
    {jam::CellBackground::LAVA_FLOOR, "LAVA_FLOOR"},
    {jam::CellBackground::FROZEN_GRASS, "FROZEN_GRASS"},
    {jam::CellBackground::ROAD, "ROAD"}};

int main() {
    std::vector<sf::Texture> objectTextures(jam::NUMBER_OF_TEXTURES);
    jam::texturePtrs.resize(jam::NUMBER_OF_TEXTURES);
    jam::texturesNumberOfFrames.resize(jam::NUMBER_OF_TEXTURES);
    for (int i = 0; i < jam::NUMBER_OF_TEXTURES; i++) {
        checkLoadTexture(objectTextures[i], std::get<2>(jam::assetInfo[i]),
                         std::get<1>(jam::assetInfo[i]));
        auto &j = std::get<0>(jam::assetInfo[i]);
        auto &k = std::get<1>(jam::assetInfo[i]);
        jam::texturePtrs[std::get<0>(jam::assetInfo[i])] = &objectTextures[i];
        jam::texturesNumberOfFrames[std::get<0>(jam::assetInfo[i])] = {
            std::get<1>(jam::assetInfo[i]).width / jam::assetCellSize.x,
            std::get<1>(jam::assetInfo[i]).height / jam::assetCellSize.y};
    }

    sf::RenderWindow window(sf::VideoMode(), "SFML works!",
                            sf::Style::Fullscreen);
    sf::Vector2i boardSize(50, 50);
    sf::Vector2i cellSize(10, 10);
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(cellSize));
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2);
    std::vector<std::vector<sf::RectangleShape>> board(
        boardSize.y, std::vector<sf::RectangleShape>(boardSize.x, rect));
    window.setView(sf::View(sf::Vector2f(boardSize * cellSize.x) / 2.f,
                            sf::Vector2f(boardSize * cellSize.x)));
    int drugs = jam::CellBackground::LIGHT_GREEN_GRASS;

    std::vector<std::vector<std::string>> out(
        boardSize.y, std::vector<std::string>(boardSize.x, ""));
    std::vector<std::vector<int>> states(boardSize.y,
                                         std::vector<int>(boardSize.x, drugs));
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); ++j) {
            board[i][j].setPosition(cellSize.x * j, cellSize.y * i);
            states[i][j] = drugs + rand() % 2;
            board[i][j].setTexture(jam::texturePtrs[states[i][j]]);
            out[i][j] = mapa[static_cast<jam::CellBackground>(states[i][j])];
        }
    }
    bool drag = false;

    while (true) {
        window.clear();
        sf::Event event{};
        if (window.pollEvent(event)) {
            if (event.type == sf::Event::TextEntered) {
                if (std::isdigit(static_cast<char>(event.text.unicode))) {
                    drugs = jam::CellBackground::LIGHT_GREEN_GRASS +
                            static_cast<char>(event.text.unicode) - '0';
                    std::cout << drugs << std::endl;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    break;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                drag = false;
            }
            if (event.type == sf::Event::MouseMoved) {
                if (drag) {
                    for (int i = 0; i < boardSize.y; ++i) {
                        for (int j = 0; j < boardSize.x; ++j) {
                            if (board[i][j].getGlobalBounds().contains(
                                    window.mapPixelToCoords(
                                        sf::Mouse::getPosition(window)))) {
                                board[i][j].setTexture(jam::texturePtrs[drugs]);
                                board[i][j].setTextureRect({0, 0, 16, 16});
                                out[i][j] =
                                    mapa[static_cast<jam::CellBackground>(
                                        drugs)];
                            }
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                drag = true;
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < boardSize.y; ++i) {
                        for (int j = 0; j < boardSize.x; ++j) {
                            if (board[i][j].getGlobalBounds().contains(
                                    window.mapPixelToCoords(
                                        sf::Mouse::getPosition(window)))) {
                                board[i][j].setTexture(jam::texturePtrs[drugs]);
                                board[i][j].setTextureRect({0, 0, 16, 16});
                                out[i][j] =
                                    mapa[static_cast<jam::CellBackground>(
                                        drugs)];
                            }
                        }
                    }
                }
            }
        }
        for (auto &i : board) {
            for (auto &j : i) {
                window.draw(j);
            }
        }
        window.display();
    }
    std::cout << "{\n";
    for (auto &i : out) {
        std::cout << "{";
        for (auto &j : i) {
            std::cout << j << ", ";
        }
        std::cout << "}, " << std::endl;
    }

    std::cout << "}\n";
    window.close();
    return 0;
}
