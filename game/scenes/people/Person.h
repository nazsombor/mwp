//
// Created by abris on 5/30/24.
//

#ifndef MWP_PERSON_H
#define MWP_PERSON_H


#include "../Scene.h"

class Event {
    unsigned long place;
    long timeToStay;
    std::string note;
};

class Person : public Scene {
    sf::RectangleShape shape;
public:
    unsigned long homeIndex;
    std::vector<Event> events;

    Person();

    void input(sf::Event &event) override;

    void update() override;

    void render(sf::RenderWindow &window) override;

};


#endif //MWP_PERSON_H
