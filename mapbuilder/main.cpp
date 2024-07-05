//
// Created by abris on 4/30/24.
//

#include <iostream>
#include "Osm.h"
#include "Map.h"
#include "HighwayGraph.h"
#include "People.h"

int main(int argc, char **argv) {
    std::cout << "Reading " << argv[1] << std::endl;
    Osm osm;
    osm.read(argv[1]);

    std::cout << "Sorting osm" << std::endl;
    Map map{osm};

    std::cout << "Writing tile images" << std::endl;
    map.write();

    std::cout << "Creating highway graph" << std::endl;
    HighwayGraph graph{osm};
    graph.write();

    std::cout << "Creating population" << std::endl;
    People people{graph};
    people.write();

    return 0;
}