//
// Created by abris on 3/24/24.
//

#include <iostream>
#include <thread>
#include <fstream>
#include "LoadGame.h"

LoadGame::LoadGame(sf::Vector2u size) : Scene(size), progressBarBackground(sf::Vector2f(size.x / 5 * 4, 100)),
                                        progressBar(sf::Vector2f()) {
    progressBarBackground.setPosition(size.x / 10, size.y / 3 * 2 - 50);
    progressBarBackground.setFillColor(sf::Color::White);
    progressBarBackground.setOutlineColor(sf::Color::Black);
    progressBarBackground.setOutlineThickness(5);
    progressBar.setPosition(size.x / 10, size.y / 2 - 50);
    progressBar.setFillColor(sf::Color::Green);
    mwp = new MapWithPeople(size);
    std::thread loadThread(Game::read, this, mwp);
    loadThread.detach();
}

void LoadGame::input(sf::Event &event) {

}

void LoadGame::update() {
    if (loadFinished) {
        nextScene = (Scene*) mwp;
    }
}

void LoadGame::render(sf::RenderWindow &window) {
    window.clear(sf::Color::White);
    window.draw(progressBarBackground);
    window.draw(progressBar);
}

void Game::read(LoadGame *loadGame, MapWithPeople *mwp) {
    std::ifstream input("graph.bin", std::ios::binary);

    size_t verticesSize;
    readSize(input, verticesSize);
    input.read(reinterpret_cast<char *>(&mwp->buildingIndex), sizeof mwp->buildingIndex);
    for (int i = 0; i < verticesSize; i++) {
        float x, y;
        input.read(reinterpret_cast<char *>(&x), sizeof x);
        input.read(reinterpret_cast<char *>(&y), sizeof y);
        mwp->vertices.emplace_back(x, y);
    }

    size_t edgesSize;
    readSize(input, edgesSize);
    for (int i = 0; i < edgesSize; i++) {
        unsigned long a, b;
        input.read(reinterpret_cast<char *>(&a), sizeof b);
        input.read(reinterpret_cast<char *>(&b), sizeof b);
        mwp->edges.emplace_back(a, b);
    }

    loadGame->loadFinished = true;
}

void Game::readSize(std::ifstream &file, size_t &size) {
    file.read(reinterpret_cast<char *>(&size), sizeof size);
}
