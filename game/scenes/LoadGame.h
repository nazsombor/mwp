//
// Created by abris on 3/24/24.
//

#ifndef MONITOR_LOADGAME_H
#define MONITOR_LOADGAME_H


#include "Scene.h"
#include "MapWithPeople.h"

class LoadGame : public Scene{
    sf::RectangleShape progressBarBackground;
    sf::RectangleShape progressBar;
    MapWithPeople *mwp;

public:
    bool loadFinished = false;

    explicit LoadGame(sf::Vector2u size);

    void input(sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;
};


namespace Game {

    class MetaTile {
    public:
        int xOffset, yOffset;
        std::string name;
    };

    void read(LoadGame *loadGame, MapWithPeople *map);

    void readSize(std::ifstream &file, size_t &size);

    void readMap(LoadGame *loadGame, MapWithPeople *mwp);

    void readTiles(LoadGame *loadGame, MapWithPeople *mwp);

    void readTilesHelper(LoadGame *loadGame, MapWithPeople * mwp, std::vector<MetaTile> *tiles, int from, int to);

}

#endif //MONITOR_LOADGAME_H
