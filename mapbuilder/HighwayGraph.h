//
// Created by abris on 5/11/24.
//

#ifndef MWP_HIGHWAYGRAPH_H
#define MWP_HIGHWAYGRAPH_H

#include "Osm.h"

struct Vertex {
    float x, y;
};

class HighwayGraph {
public:
    unsigned long buildingIndex;
    std::vector<Vertex> vertices;
    std::vector<std::pair<unsigned long, unsigned long>> edges;

    explicit HighwayGraph(Osm &osm);

    void write();
    Vertex origin(Way *pWay);

    Vertex origin(Relation *relation);
};


#endif //MWP_HIGHWAYGRAPH_H
