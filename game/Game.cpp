//
// Created by abris on 3/24/24.
//

#include "Game.h"
#include "scenes/MainMenu.h"

Game::Game() : window(sf::VideoMode(1920, 1080), "Monitor", sf::Style::Default){
    scene = new MainMenu(window.getSize());
}

void Game::run() {
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            scene->input(event);
        }
        if (scene->closeRequested) {
            window.close();
        }
        scene->update();
        scene->render(window);
        window.display();
        if (scene->nextScene) {
            auto oldScene = scene;
            scene = scene->nextScene;
            delete oldScene;
        }
    }
}
