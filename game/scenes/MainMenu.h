//
// Created by abris on 3/24/24.
//

#ifndef MONITOR_MAINMENU_H
#define MONITOR_MAINMENU_H


#include "Scene.h"

class MainMenu : public Scene {
    sf::RectangleShape newGameButton;
    sf::RectangleShape loadGameButton;
    sf::RectangleShape exitButton;
    sf::Font font;
    sf::Text newGameText;
    sf::Text loadGameText;
    sf::Text exitText;

public:
    explicit MainMenu(sf::Vector2u size);

    void input(sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;
};


#endif //MONITOR_MAINMENU_H
