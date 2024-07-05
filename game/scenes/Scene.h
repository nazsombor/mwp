//
// Created by abris on 3/24/24.
//

#ifndef MONITOR_SCENE_H
#define MONITOR_SCENE_H

#include "SFML/Graphics.hpp"


class Scene {
public:
    sf::Vector2u size;
    Scene *nextScene = nullptr;
    bool closeRequested = false;

    explicit Scene(sf::Vector2u size);

    Scene();

    virtual void input(sf::Event &event) = 0;

    virtual void update() = 0;

    virtual void render(sf::RenderWindow &window) = 0;
};


#endif //MONITOR_SCENE_H
