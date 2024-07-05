//
// Created by abris on 5/11/24.
//

#include <fstream>
#include "HighwayGraph.h"

HighwayGraph::HighwayGraph(Osm &osm) {
    std::vector<Way *> highways;
    std::vector<Way *> buildingWays;
    std::vector<Relation *> buildingRelations;

    osm.findAllWaysWithKey("highway", highways);
    osm.findAllWaysWithKey("building", buildingWays);
    osm.findAllRelationsWithKey("building", buildingRelations);

    unsigned long index = 0;
    for (auto h : highways) {
        for (auto n : h->nodes) {
            vertices.emplace_back(n->x, n->y);
        }

        index++;
        for (int i = 1; i < h->nodes.size(); i++) {
            edges.emplace_back(index, index-1);
            index++;
        }
    }

    for (int i = 0; i < vertices.size(); i++) {
        auto a = vertices[i];
        for (int j = 0; j < vertices.size(); j++) {
            auto b = vertices[j];
            auto abDistance = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
            if (abDistance < 100 && i != j) {
                edges.emplace_back(i, j);
            }
        }
    }

    std::vector<Vertex> buildingVertices;
    for (auto building : buildingWays) {
        buildingVertices.push_back(origin(building));
    }
    for (auto building : buildingRelations) {
        buildingVertices.push_back(origin(building));
    }

    buildingIndex = vertices.size() - 1;

    std::copy(buildingVertices.begin(), buildingVertices.end(), std::back_insert_iterator(vertices));

    for (unsigned long i = buildingIndex; i < vertices.size(); i++) {
        auto a = vertices[i];
        float minDistance = 10000;
        unsigned long minDistanceIndex = -1;
        for (unsigned long j = 0; j <= buildingIndex; j++) {
            auto b = vertices[j];
            auto abDistance = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
            if (abDistance < minDistance && i != j) {
                minDistanceIndex = j;
                minDistance = abDistance;
            }
        }
        edges.emplace_back(i, minDistanceIndex);
    }

}

void HighwayGraph::write() {
    std::ofstream fs("graph.bin", std::ios::binary);

    auto verticesSize = vertices.size();
    fs.write(reinterpret_cast<const char*>(&verticesSize), sizeof verticesSize);
    fs.write(reinterpret_cast<const char*>(&buildingIndex), sizeof buildingIndex);
    for (auto vertex : vertices) {
        fs.write(reinterpret_cast<const char*>(&vertex.x), sizeof vertex.x);
        fs.write(reinterpret_cast<const char*>(&vertex.y), sizeof vertex.y);
    }

    auto edgesSize = edges.size();
    fs.write(reinterpret_cast<const char*>(&edgesSize), sizeof edgesSize);
    for (auto edge : edges) {
        fs.write(reinterpret_cast<const char*>(&edge.first), sizeof edge.first);
        fs.write(reinterpret_cast<const char*>(&edge.second), sizeof edge.second);
    }

}

Vertex HighwayGraph::origin(Way *pWay) {
    float midX = 0, midY = 0;

    for (auto node : pWay->nodes) {
        midX += node->x;
        midY += node->y;
    }
    midX /= pWay->nodes.size();
    midY /= pWay->nodes.size();
    return Vertex(midX, midY);
}

Vertex HighwayGraph::origin(Relation *relation) {
    float midX = 0, midY = 0;
    int length = 0;

    for (auto way : relation->ways) {
        length += way.first->nodes.size();
        for (auto node : way.first->nodes) {
            midX += node->x;
            midY += node->y;
        }
    }

    midX /= length;
    midY /= length;

    return Vertex(midX, midY);
}
