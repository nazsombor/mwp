//
// Created by abris on 5/30/24.
//

#ifndef MWP_PEOPLE_H
#define MWP_PEOPLE_H


#include "HighwayGraph.h"

class Event {
    unsigned long location;
    long seconds;
    std::string note;
};

class Person {
public:
    unsigned long home;
    std::vector<unsigned long> events;
};

class People {
    std::vector<Person> people;
    std::vector<Event> events;
public:
    People(HighwayGraph &graph);

    void write();
};


#endif //MWP_PEOPLE_H
