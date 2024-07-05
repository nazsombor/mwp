//
// Created by abris on 5/16/24.
//

#ifndef MWP_MAPWITHPEOPLE_H
#define MWP_MAPWITHPEOPLE_H


#include "Scene.h"

struct Vertex {
    float x, y;
};

class Tile {
public:
    sf::Texture texture;
    sf::Sprite sprite;
};

class MapWithPeople : public Scene {

public:
    unsigned long buildingIndex;
    std::vector<Vertex> vertices;
    std::vector<std::pair<unsigned long, unsigned long>> edges;
    std::deque<Tile> tiles;
    sf::View view;

    explicit MapWithPeople(sf::Vector2u size);

    void input(sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

    void path(unsigned long a, unsigned long b, std::vector<unsigned long> &shortestPath);
};


#endif //MWP_MAPWITHPEOPLE_H
