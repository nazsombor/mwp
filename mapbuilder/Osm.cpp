//
// Created by abris on 3/23/24.
//

#include "Osm.h"

Osm::Osm() : nwh(&nodes, &ways), rm(&nodes, &ways, &relations) {
    nwh.scale = scale;
    nwh.scaleY = scaleY;
}

void Osm::read(std::string filename) {
    osmium::io::File input_file(filename);
    osmium::io::Reader reader(input_file);
    osmium::io::Reader reader2(input_file);

    width = (reader.header().box().right() - reader.header().box().left()) * scale;
    height = -(reader.header().box().bottom() - reader.header().box().top()) * scale * scaleY;

    nwh.minLon = reader.header().box().left();
    nwh.minLat = reader.header().box().top();

    osmium::apply(reader, nwh);
    osmium::relations::read_relations(input_file, rm);
    osmium::apply(reader2, rm.handler());
    reader.close();
    reader2.close();
    rm.buildRelations();
}

void Osm::findAllRelationsWithTag(std::string key, std::string value, std::vector<Relation *> &result) {
    for (auto relation : relations) {
        if (relation.second->tags.has_tag(key, value)) {
            result.push_back(relation.second);
        }
    }
}

void Osm::findAllRelationsWithKey(const std::string &key, std::vector<Relation *> &result) {
    for (auto relation : relations) {
        if (relation.second->tags.has_key(key)) {
            result.push_back(relation.second);
        }
    }
}

void Osm::findAllWaysWithTag(const std::string key, const std::string value, std::vector<Way *> &result) {
    for (auto way : ways) {
        if (way.second->tags.has_tag(key, value)) {
            result.push_back(way.second);
        }
    }
}

void Osm::findAllWaysWithKey(std::string key, std::vector<Way *> &result) {
    for (auto way : ways) {
        if (way.second->tags.has_key(key)) {
            result.push_back(way.second);
        }
    }
}

NodeAndWayHandler::NodeAndWayHandler(std::map<unsigned long, Node *> *nodes, std::map<unsigned long, Way *> *ways) {
    this->nodes = nodes;
    this->ways = ways;
}

RelationsManager::RelationsManager(std::map<unsigned long, Node *> *nodes, std::map<unsigned long, Way *> *ways,
                                   std::map<unsigned long, Relation *> *relations) {
    this->nodes = nodes;
    this->ways = ways;
    this->relations = relations;
}

Node::Node(const osmium::Node &node, double minLat, double minLon, double scale, double scaleY) {
    id = node.id();
    x = (node.location().lon() - minLon) * scale;
    y = -(node.location().lat() - minLat) * scale * scaleY;
    for (auto &tag : node.tags()) {
        tags.tags[tag.key()] = tag.value();
    }
}

Way::Way(const osmium::Way &way, std::map<unsigned long, Node *> *nodes) {
    id = way.id();
    for (auto &tag : way.tags()) {
        tags.tags[tag.key()] = tag.value();
    }
    for (auto node : way.nodes()) {
        auto n = nodes->find(node.ref());
        if (n == nodes->end()) continue;
        this->nodes.push_back(n->second);
    }
}

void Way::box(double &x, double &y, double &x2, double &y2) {
    x = DBL_MAX;
    y = DBL_MAX;
    x2 = DBL_MIN;
    y2 = DBL_MIN;
    for (auto node : nodes) {
        if (node->x < x) {
            x = node->x;
        }
        if (node->y < y) {
            y = node->y;
        }
        if (node->x > x2) {
            x2 = node->x;
        }
        if (node->y > y2) {
            y2 = node->y;
        }
    }

}

Relation::Relation(const osmium::Relation &relation, std::map<unsigned long, Node*> *nodes, std::map<unsigned long, Way*> *ways) {
    id = relation.id();
    for (auto &tag : relation.tags()) {
        tags.tags[tag.key()] = tag.value();
    }
    for (auto &member : relation.members()) {
        switch (member.type()) {
            case osmium::item_type::node: {
                auto ni = nodes->find(member.ref());
                if (ni == nodes->end()) continue;
                this->nodes[ni->second] = member.role();
                break;
            }
            case osmium::item_type::way: {
                auto wi = ways->find(member.ref());
                if (wi == ways->end()) continue;
                this->ways[wi->second] = member.role();
                break;
            }
            case osmium::item_type::relation: {
                memberRelationRefs[member.ref()] = member.role();
            }
        }
    }

}

void Relation::box(double &x, double &y, double &x2, double &y2) {
    x = DBL_MAX;
    y = DBL_MAX;
    x2 = DBL_MIN;
    y2 = DBL_MIN;
    for (auto node : nodes) {
        if (node.first->x < x) {
            x = node.first->x;
        }
        if (node.first->y < y) {
            y = node.first->y;
        }
        if (node.first->x > x2) {
            x2 = node.first->x;
        }
        if (node.first->y > y2) {
            y2 = node.first->y;
        }
    }
    for (auto way : ways) {
        for (auto node : way.first->nodes) {
            if (node->x < x) {
                x = node->x;
            }
            if (node->y < y) {
                y = node->y;
            }
            if (node->x > x2) {
                x2 = node->x;
            }
            if (node->y > y2) {
                y2 = node->y;
            }
        }
    }
    for (auto relation : relations) {
        double rx, ry, rx2, ry2;
        relation.first->box(rx, ry, rx2, ry2);
        if (rx < x) {
            x = rx;
        }
        if (ry < y) {
            y = ry;
        }
        if (rx2 > x2) {
            x2 = rx2;
        }
        if (ry2 > y2) {
            y2 = ry2;
        }
    }
}

void NodeAndWayHandler::node(const osmium::Node &node) {
    nodes->insert(std::make_pair(node.id(), new Node(node, minLat, minLon, scale, scaleY)));
}

void NodeAndWayHandler::way(const osmium::Way &way) {
    ways->insert(std::make_pair(way.id(), new Way(way, nodes)));
}

void RelationsManager::complete_relation(const osmium::Relation &relation) {
    relations->insert(std::make_pair(relation.id(), new Relation(relation, nodes, ways)));
}

void RelationsManager::buildRelations() {
    for (auto rel : *relations) {
        auto relation = rel.second;
        for (auto memberRel : relation->memberRelationRefs) {
            auto member = relations->find(memberRel.first);
            if (member == relations->end()) continue;
            relation->relations[member->second] = memberRel.second;
        }
    }
}

bool Tags::has_key(std::string key) {
    auto ti = tags.find(key);
    return ti != tags.end();
}

bool Tags::has_value(std::string value) {
    for (auto tag : tags) {
        if (strcmp(value.c_str(), tag.second.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

bool Tags::has_tag(std::string key, std::string value) {
    auto ti = tags.find(key);
    if (ti == tags.end()) return false;
    return strcmp(ti->second.c_str(), value.c_str()) == 0;
}

void sortPoints(std::map<Way *, std::string> ways, std::vector<Node *> &points) {
    if (ways.empty()) return;

    std::vector<Way *> originalBoundary;
    std::vector<std::pair<Way *, bool>> sortedBoundary;
    originalBoundary.reserve(ways.size());

    for (const auto &way: ways) {
        originalBoundary.push_back(way.first);
    }

    sortedBoundary.emplace_back(originalBoundary[0], false);
    Way *currentWay = originalBoundary[0];
    Node *lastNode = currentWay->nodes[currentWay->nodes.size() - 1];

    originalBoundary.erase(originalBoundary.begin());

    while (!originalBoundary.empty()) {
        double distance = 1000;
        bool reverseOrder = false;
        int i = 0, minIndex = 0;
        Node *nextLastNode;
        for (auto way: originalBoundary) {

            auto fN = way->nodes[0];
            auto lN = way->nodes[way->nodes.size() - 1];

            double df = (lastNode->x - fN->x) * (lastNode->x - fN->x) + (lastNode->y - fN->y) * (lastNode->y - fN->y);
            double dl = (lastNode->x - lN->x) * (lastNode->x - lN->x) + (lastNode->y - lN->y) * (lastNode->y - lN->y);

            if (dl < df) {
                if (dl < distance) {
                    currentWay = way;
                    nextLastNode = fN;
                    distance = dl;
                    minIndex = i;
                    reverseOrder = true;
                }
            } else {
                if (df < distance) {
                    currentWay = way;
                    nextLastNode = lN;
                    distance = df;
                    minIndex = i;
                    reverseOrder = false;
                }
            }
            i++;
        }
        lastNode = nextLastNode;
        originalBoundary.erase(originalBoundary.begin() + minIndex);
        sortedBoundary.emplace_back(currentWay, reverseOrder);
    }

    for (auto way: sortedBoundary) {
        if (way.second) {
            for (int i = way.first->nodes.size() - 1; i >= 0; i--) {
                points.push_back(way.first->nodes[i]);
            }
        } else {
            for (auto node: way.first->nodes) {
                points.push_back(node);
            }
        }
    }
}