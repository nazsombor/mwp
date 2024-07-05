//
// Created by abris on 3/24/24.
//

#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <SFML/Graphics.hpp>
#include "scenes/Scene.h"


class Game {
    sf::RenderWindow window;
    Scene *scene;
public:
    explicit Game();
    void run();
};


#endif //MONITOR_GAME_H
