//
// Created by abris on 5/16/24.
//

#include <iostream>
#include <queue>
#include "MapWithPeople.h"

MapWithPeople::MapWithPeople(sf::Vector2u size) : Scene(size) {

}

void MapWithPeople::input(sf::Event &event) {

}

void MapWithPeople::update() {

}

void MapWithPeople::render(sf::RenderWindow &window) {
    window.clear(sf::Color::White);
    std::vector<std::vector<sf::Vertex>> v;

    for (auto edge : edges) {
        std::vector<sf::Vertex> vv;
        vv.emplace_back(sf::Vector2f(vertices[edge.first].x, vertices[edge.first].y), sf::Color::Black);
        vv.emplace_back(sf::Vector2f(vertices[edge.second].x, vertices[edge.second].y), sf::Color::Black);
        v.push_back(vv);
    }

    for (auto vv : v) {
        window.draw(&vv[0], vv.size(), sf::LineStrip);
    }
}

void MapWithPeople::path(unsigned long a, unsigned long b, std::vector<unsigned long> &shortestPath) {
    if (a == b) return;

    std::vector<std::vector<unsigned long>> paths;
    std::vector<bool> visited(vertices.size(), false);
    visited[a] = true;

    for (auto edge: edges) {
        if (edge.first == a) {
            visited[edge.second] = true;
            std::vector<unsigned long> p1;
            p1.push_back(a);
            p1.push_back(edge.second);
            paths.push_back(p1);
            if (edge.second == b) {
                std::copy(p1.begin(), p1.end(), std::back_insert_iterator(shortestPath));
                return;
            }
        }
        if (edge.second == a) {
            visited[edge.first] = true;
            std::vector<unsigned long> p1;
            p1.push_back(a);
            p1.push_back(edge.first);
            paths.push_back(p1);
            if (edge.first == b) {
                std::copy(p1.begin(), p1.end(), std::back_inserter(shortestPath));
                return;
            }
        }
    }


    while (true) {
        std::vector<std::vector<unsigned long>> newPaths;
        for (auto &path : paths) {
            auto last = path[path.size() - 1];
            bool firstRound = true;
            for (auto edge: edges) {

                if (!visited[edge.second] && edge.first == last) {
                    visited[edge.second] = true;
                    if (firstRound) {
                        firstRound = false;
                        path.push_back(edge.second);
                    } else {
                        std::vector<unsigned long> path2;
                        path2.reserve(path.size());
                        std::copy(path.begin(), path.end() - 1, std::back_inserter(path2));
                        path2.push_back(edge.second);
                        newPaths.push_back(path2);
                    }
                }
                if (!visited[edge.first] && edge.second == last) {
                    visited[edge.first] = true;
                    if (firstRound) {
                        firstRound = false;
                        path.push_back(edge.first);
                    } else {
                        std::vector<unsigned long> path2;
                        path2.reserve(path.size());
                        std::copy(path.begin(), path.end() - 1, std::back_inserter(path2));
                        path2.push_back(edge.first);
                        newPaths.push_back(path2);
                    }
                }
            }
        }

        std::copy(newPaths.begin(), newPaths.end(), std::back_inserter(paths));

        for (auto path: paths) {
            if (path[path.size() - 1] == b) {
                std::copy(path.begin(), path.end(), std::back_inserter(shortestPath));
                return;
            }
        }
    }
}
