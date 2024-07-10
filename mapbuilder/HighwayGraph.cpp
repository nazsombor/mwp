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
    osm.findAllWaysWithKey("building:levels", buildingWays);
    osm.findAllRelationsWithKey("building", buildingRelations);
    osm.findAllRelationsWithKey("building:levels", buildingRelations);

    connectHighways(highways);
    connectCloseVertices(osm.width, osm.height);
    connectBuildingsToGraph(buildingWays, buildingRelations);
}

void HighwayGraph::connectHighways(std::vector<Way *> &highways) {
    unsigned long edgeIndex = 0;
    unsigned long vertexIndex = 0;
    for (auto h : highways) {
        for (auto n : h->nodes) {
            vertices.emplace_back(vertexIndex++, false, n->x, n->y);
        }

        edgeIndex++;
        for (int i = 1; i < h->nodes.size(); i++) {
            edges.emplace_back(edgeIndex, edgeIndex - 1);
            edgeIndex++;
        }
    }
}

void HighwayGraph::connectCloseVertices(double width, double height) {
    xLength = width / 100;
    yLength = height / 100;
    grid.reserve(xLength * yLength);
    for (int i = 0; i < xLength * yLength; i++) {
        grid.emplace_back();
    }

    for (auto vertex : vertices) {
        unsigned long x = vertex.x / 100;
        unsigned long y = vertex.y / 100;

        if (vertex.x < 0) {
            x = 0;
        }
        if (x > xLength - 1) {
            x = xLength - 1;
        }
        if (vertex.y < 0) {
            y = 0;
        }
        if (y > yLength - 1) {
            y = yLength - 1;
        }

        grid[y * xLength + x].push_back(vertex);
    }


    for (unsigned long y = 0; y < yLength; y++) {
        for (unsigned long x = 0; x < xLength; x++) {
            for (auto a : grid[y * xLength + x]) {
                for (int i = -1; i < 2; i++) {
                    if (y + i < 0 || y + i >= yLength) continue;
                    for (int j = -1; j < 2; j++) {
                        if (x + j < 0 || x + j >= xLength) continue;
                        for (auto b : grid[(y + i) * xLength + (x + j)]) {
                            auto abDistance = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
                            if (abDistance < 100 && a.index != b.index) {
                                edges.emplace_back(a.index, b.index);
                            }
                        }
                    }
                }
            }
        }
    }
}

void
HighwayGraph::connectBuildingsToGraph(std::vector<Way *> &buildingWays, std::vector<Relation *> &buildingRelations) {
    buildingIndex = vertices.size() - 1;
    unsigned long index = buildingIndex + 1;
    for (auto building : buildingWays) {
        vertices.push_back(buildingOrigin(index, building));
        index++;
    }
    for (auto building : buildingRelations) {
        vertices.push_back(buildingOrigin(index, building));
        index++;
    }

    for (unsigned long i = buildingIndex; i < index; i++) {
        auto vertex = vertices[i];
        unsigned long x = vertex.x / 100;
        unsigned long y = vertex.y / 100;

        if (vertex.x < 0) {
            x = 0;
        }
        if (x > xLength - 1) {
            x = xLength - 1;
        }
        if (vertex.y < 0) {
            y = 0;
        }
        if (y > yLength - 1) {
            y = yLength - 1;
        }

        grid[y * xLength + x].push_back(vertex);
    }

    for (int y = 0; y < yLength; y++) {
        for (int x = 0; x < xLength; x++) {
            for (auto a : grid[y * xLength + x]) {
                if (!a.building) continue;
                float minDistance = 10000;
                unsigned long minDistanceIndex = -1;
                for (int i = -1; i < 2; i++) {
                    if (y + i < 0 || y + i >= yLength) continue;
                    for (int j = -1; j < 2; j++) {
                        if (x + j < 0 || x + j >= xLength) continue;
                        for (auto b : grid[(y + i) * xLength + (x + j)]) {
                            if (b.building) continue;
                            auto abDistance = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
                            if (abDistance < minDistance) {
                                minDistanceIndex = b.index;
                                minDistance = abDistance;
                            }
                        }
                    }
                }
                edges.emplace_back(a.index, minDistanceIndex);
            }
        }
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

Vertex HighwayGraph::buildingOrigin(unsigned long index, Way *pWay) {
    float midX = 0, midY = 0;

    for (auto node : pWay->nodes) {
        midX += node->x;
        midY += node->y;
    }
    midX /= pWay->nodes.size();
    midY /= pWay->nodes.size();
    return Vertex(index, true, midX, midY);
}

Vertex HighwayGraph::buildingOrigin(unsigned long index, Relation *relation) {
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

    return Vertex(index, true, midX, midY);
}
