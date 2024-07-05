//
// Created by abris on 3/24/24.
//

#include "MainMenu.h"
#include "LoadGame.h"

MainMenu::MainMenu(sf::Vector2u size) :
        Scene(size),
        newGameButton(sf::Vector2f(400, 100)),
        loadGameButton(sf::Vector2f(400, 100)),
        exitButton(sf::Vector2f(400, 100)) {

    newGameButton.setPosition(size.x / 2 - 200, size.y / 2 - 250);
    newGameButton.setOutlineColor(sf::Color::Black);
    newGameButton.setOutlineThickness(5);

    loadGameButton.setPosition(size.x / 2 - 200, size.y / 2 - 50);
    loadGameButton.setOutlineColor(sf::Color::Black);
    loadGameButton.setOutlineThickness(5);

    exitButton.setPosition(size.x / 2 - 200, size.y / 2 + 150);
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setOutlineThickness(5);

    font.loadFromFile("DejaVuSans-ExtraLight.ttf");

    newGameText.setString("New Game");
    newGameText.setFont(font);
    newGameText.setFillColor(sf::Color::Black);
    newGameText.setCharacterSize(70);
    newGameText.setPosition(newGameButton.getPosition());

    loadGameText.setString("Load Game");
    loadGameText.setFont(font);
    loadGameText.setFillColor(sf::Color::Black);
    loadGameText.setCharacterSize(70);
    loadGameText.setPosition(loadGameButton.getPosition());

    exitText.setString("Exit");
    exitText.setFont(font);
    exitText.setFillColor(sf::Color::Black);
    exitText.setCharacterSize(70);
    exitText.setPosition(exitButton.getPosition());
}

void MainMenu::input(sf::Event &event) {
    if (event.type == sf::Event::MouseMoved) {
        if (newGameButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
            newGameButton.setFillColor(sf::Color(200, 200, 200, 255));
        } else {
            newGameButton.setFillColor(sf::Color::White);
        }
        if (loadGameButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
            loadGameButton.setFillColor(sf::Color(200, 200, 200, 255));
        } else {
            loadGameButton.setFillColor(sf::Color::White);
        }
        if (exitButton.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
            exitButton.setFillColor(sf::Color(200, 200, 200, 255));
        } else {
            exitButton.setFillColor(sf::Color::White);
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (newGameButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            nextScene = new LoadGame(size);
        }
        if (loadGameButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {

        }
        if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            closeRequested = true;
        }
    }
    if (event.type == sf::Event::KeyReleased) {
        nextScene = new LoadGame(size);
    }
}

void MainMenu::update() {

}

void MainMenu::render(sf::RenderWindow &window) {
    window.clear(sf::Color::White);
    window.draw(newGameButton);
    window.draw(newGameText);
    window.draw(loadGameButton);
    window.draw(loadGameText);
    window.draw(exitButton);
    window.draw(exitText);
}
