//
// Created by abris on 3/23/24.
//

#ifndef MONITOR_OSM_H
#define MONITOR_OSM_H

//https://osmcode.org/libosmium/manual.html#handlers
//https://osmcode.org/libosmium/manual.html#working-with-relations

#include <iostream>
#include <float.h>
#include <map>
#include <osmium/relations/relations_manager.hpp>
#include <osmium/handler.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/visitor.hpp>

class Tags {
public:
    std::map<std::string, std::string> tags;

    bool has_key(std::string key);

    bool has_value(std::string value);

    bool has_tag(std::string key, std::string value);
};

class Node {
public:
    unsigned long id;
    Tags tags;
    double x, y;

    Node(const osmium::Node &node, double minLat, double minLon, double scale, double scaleY);
};

class Way {
public:
    unsigned long id;
    Tags tags;
    std::vector<Node *> nodes;

    explicit Way(const osmium::Way &way, std::map<unsigned long, Node *> *pMap);

    void box(double &x, double &y, double &x2, double &y2);
};

class Relation {
public:
    unsigned long id;
    Tags tags;
    std::map<unsigned long, std::string> memberRelationRefs;
    std::map<Node *, std::string> nodes;
    std::map<Way *, std::string> ways;
    std::map<Relation *, std::string> relations;

    explicit Relation(const osmium::Relation &relation, std::map<unsigned long, Node *> *nodes,
                      std::map<unsigned long, Way *> *ways);

    void box(double &x, double &y, double &x2, double &y2);

};

class NodeAndWayHandler : public osmium::handler::Handler {
public:
    std::map<unsigned long, Node *> *nodes;
    std::map<unsigned long, Way *> *ways;
    double scale{}, scaleY{}, minLat{}, minLon{};

    NodeAndWayHandler(std::map<unsigned long, Node *> *nodes, std::map<unsigned long, Way *> *ways);

    void node(const osmium::Node &node);

    void way(const osmium::Way &way);
};

class RelationsManager : public osmium::relations::RelationsManager<RelationsManager, true, true, true> {
public:
    std::map<unsigned long, Node *> *nodes;
    std::map<unsigned long, Way *> *ways;
    std::map<unsigned long, Relation *> *relations;

    RelationsManager(std::map<unsigned long, Node *> *nodes, std::map<unsigned long, Way *> *ways,
                     std::map<unsigned long, Relation *> *relations);

    void complete_relation(const osmium::Relation &relation);

    void buildRelations();
};

class Osm {

public:
    std::map<unsigned long, Node *> nodes;
    std::map<unsigned long, Way *> ways;
    std::map<unsigned long, Relation *> relations;
    NodeAndWayHandler nwh;
    RelationsManager rm;
    double scale = 77766.5448; //scale it, so 1 is 1 meter.
    double scaleY = 1.6;
    double width, height;

    Osm();

    void read(std::string filename);

    void findAllRelationsWithTag(std::string key, std::string value, std::vector<Relation *> &result);

    void findAllWaysWithTag(std::string key, std::string value, std::vector<Way *> &result);

    void findAllWaysWithKey(std::string key, std::vector<Way *> &ways);

    void findAllRelationsWithKey(const std::string &key, std::vector<Relation *> &result);
};

void sortPoints(std::map<Way*, std::string> ways, std::vector<Node*> &points);


#endif //MONITOR_OSM_H
