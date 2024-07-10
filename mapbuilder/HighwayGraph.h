//
// Created by abris on 5/11/24.
//

#ifndef MWP_HIGHWAYGRAPH_H
#define MWP_HIGHWAYGRAPH_H

#include "Osm.h"

struct Vertex {
    unsigned long index;
    bool building = false;
    float x, y;
};

class HighwayGraph {
    void connectHighways(std::vector<Way *> &highways);

    void connectCloseVertices(double width, double height);

    void connectBuildingsToGraph(std::vector<Way *> &buildingWays, std::vector<Relation *> &buildingRelations);

    static Vertex buildingOrigin(unsigned long index, Way *pWay);

    static Vertex buildingOrigin(unsigned long index, Relation *relation);

public:
    unsigned long buildingIndex;

    std::vector<Vertex> vertices;

    std::vector<std::pair<unsigned long, unsigned long>> edges;

    std::vector<std::vector<Vertex>> grid;

    unsigned long xLength, yLength;

    explicit HighwayGraph(Osm &osm);

    void write();
};


#endif //MWP_HIGHWAYGRAPH_H
