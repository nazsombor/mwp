//
// Created by abris on 4/30/24.
//

#include "MapTiles.h"

#include <utility>

LineStrip::LineStrip(Way *way, unsigned short red, unsigned short green, unsigned short blue, double lineWidth)
        : lineStrip(way), red(red), green(green), blue(blue), lineWidth(lineWidth) {}

Polygon::Polygon(Way *way, unsigned short r, unsigned short g, unsigned short b)
        : area(way), red(r), green(g), blue(b) {}

Polygon::Polygon(Way *way, unsigned short red, unsigned short green, unsigned short blue, unsigned short redOutline,
                 unsigned short greenOutline, unsigned short blueOutline, double lineWidth)
        : area(way), red(red), green(green), blue(blue), redOutline(redOutline), greenOutline(greenOutline),
          blueOutline(blueOutline), drawOutline(true), lineWidth(lineWidth) {}

Multipolygon::Multipolygon(Relation *relation, unsigned short red, unsigned short green, unsigned short blue)
        : red(red), green(green), blue(blue) {
    build(relation);
}

Multipolygon::Multipolygon(Relation *relation, unsigned short red, unsigned short green, unsigned short blue,
                           unsigned short redOutline, unsigned short greenOutline, unsigned short blueOutline,
                           int lineWidth)
        : red(red), green(green), blue(blue), redOutline(redOutline), greenOutline(greenOutline),
          blueOutline(blueOutline), drawOutline(true), lineWidth(lineWidth) {
    build(relation);
}

Tile::Tile(int xOffset, int yOffset, std::string name) : xOffset(xOffset), yOffset(yOffset), name(std::move(name)) {}

Tiles::Tiles(int tileWidth, int tileHeight, double width, double height) : tileWidth(tileWidth),
                                                                           tileHeight(tileHeight),
                                                                           tileXCount(width / tileWidth),
                                                                           tileYCount(height / tileHeight),
                                                                           width(width),
                                                                           height(height) {
    regions.reserve(tileXCount * tileYCount);
    for (int y = 0; y < tileYCount; y++) {
        for (int x = 0; x < tileXCount; x++) {
            regions.emplace_back(x * tileWidth, y * tileWidth, std::to_string(y * tileXCount + x));
        }
    }
}

MapTiles::MapTiles(Osm &osm) : regions(std::min((int) osm.width, 10000), std::min((int) osm.height, 10000), osm.width,
                                       osm.height) {
    this->osm = &osm;

    lineStrip("admin_level", "2", 0, 0, 0, 5);
    lineStrip("highway", 247, 250, 191, 10);

    area("landuse", "forest", 173, 209, 158);
    area("landuse", "residential", 224, 223, 223);
    area("landuse", "industrial", 224, 223, 223);
    area("natural", "water", 170, 211, 223);
    area("building", 217, 208, 201, 207, 196, 187);
    area("building:levels", 217, 208, 201, 207, 196, 187);
}

void MapTiles::lineStrip(const std::string &key, unsigned short red, unsigned short green, unsigned short blue,
                         double lineWidth) {
    std::vector<Way *> ways;
    osm->findAllWaysWithKey(key, ways);
    for (auto way: ways) {
        LineStrip l{way, red, green, blue, lineWidth};
        regions.addLineStrip(l);
    }

    std::vector<Relation *> relations;
    osm->findAllRelationsWithKey(key, relations);
    for (auto relation: relations) {
        for (const auto &way: relation->ways) {
            LineStrip l{way.first, red, green, blue, lineWidth};
            regions.addLineStrip(l);
        }
    }
}

void MapTiles::lineStrip(const std::string &key, const std::string &value, unsigned short red, unsigned short green,
                         unsigned short blue, double lineWidth) {
    std::vector<Way *> ways;
    osm->findAllWaysWithTag(key, value, ways);
    for (auto way: ways) {
        LineStrip l{way, red, green, blue, lineWidth};
        regions.addLineStrip(l);
    }

    std::vector<Relation *> relations;
    osm->findAllRelationsWithTag(key, value, relations);
    for (auto relation: relations) {
        for (const auto &way: relation->ways) {
            LineStrip l{way.first, red, green, blue, lineWidth};
            regions.addLineStrip(l);
        }
    }
}

void MapTiles::area(const std::string &key, const std::string &value, unsigned short red, unsigned short green,
                    unsigned short blue) {
    std::vector<Way *> ways;
    osm->findAllWaysWithTag(key, value, ways);
    for (auto way: ways) {
        Polygon p{way, red, green, blue};
        regions.addArea(p);
    }

    std::vector<Relation *> relations;
    osm->findAllRelationsWithTag(key, value, relations);
    for (auto relation: relations) {
        Multipolygon m{relation, red, green, blue};
        regions.addArea(m);
    }
}

void MapTiles::area(const std::string &key, const std::string &value, unsigned short red, unsigned short green,
                    unsigned short blue, unsigned short redOutline, unsigned short greenOutline,
                    unsigned short blueOutline) {
    std::vector<Way *> ways;
    osm->findAllWaysWithTag(key, value, ways);
    for (auto way: ways) {
        Polygon p(way, red, green, blue, redOutline, greenOutline, blueOutline, 1);
        regions.addArea(p);
    }

    std::vector<Relation *> relations;
    osm->findAllRelationsWithTag(key, value, relations);
    for (auto relation: relations) {
        Multipolygon m{relation, red, green, blue, redOutline, greenOutline, blueOutline, 1};
        regions.addArea(m);
    }
}

void MapTiles::area(const std::string &key, unsigned short red, unsigned short green,
                    unsigned short blue, unsigned short redOutline, unsigned short greenOutline,
                    unsigned short blueOutline) {
    std::vector<Way *> ways;
    osm->findAllWaysWithKey(key, ways);
    for (auto way: ways) {
        Polygon p{way, red, green, blue, redOutline, greenOutline, blueOutline, 1};
        regions.addArea(p);
    }

    std::vector<Relation *> relations;
    osm->findAllRelationsWithKey(key, relations);
    for (auto relation: relations) {
        Multipolygon m{relation, red, green, blue, redOutline, greenOutline, blueOutline, 1};
        regions.addArea(m);
    }
}



void MapTiles::write() {
    int range = regions.regions.size() / 10;

    std::thread t1{writeTiles, &regions, 0, range};
    std::thread t2{writeTiles, &regions, range, 2 * range};
    std::thread t3{writeTiles, &regions, 2 * range, 3 *  range};
    std::thread t4{writeTiles, &regions, 3 * range, 4 *  range};
    std::thread t5{writeTiles, &regions, 4 * range, 5 * range};
    std::thread t6{writeTiles, &regions, 5 * range, 6 * range};
    std::thread t7{writeTiles, &regions, 6 * range, 7 * range};
    std::thread t8{writeTiles, &regions, 7 * range, 8 * range};
    std::thread t9{writeTiles, &regions, 8 * range, 9 * range};
    std::thread t10{writeTiles, &regions, 9 * range, regions.regions.size()};

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
}

void writeTiles(Tiles *tiles, int from, int to) {
    for (int i = from; i < to; i++) {
        writeTile(&tiles->regions[i], tiles->tileWidth, tiles->tileHeight);
    }
}

void writeTile(Tile *tile, int tileWidth, int tileHeight) {
    std::cout << "Writing " << tile->name << ".png" << std::endl;
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, tileWidth, tileHeight);
    cairo_t *cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, 0, 0, tileWidth, tileHeight);
    cairo_fill(cr);


    for (auto area: tile->areas) {
        area.draw(cr, tile->xOffset, tile->yOffset);
    }

    for (const auto &area: tile->areas2) {
        area.draw(cr, tile->xOffset, tile->yOffset);
    }

    for (auto line: tile->lineStrips) {
        line.draw(cr, tile->xOffset, tile->yOffset);
    }
    cairo_surface_write_to_png(surface, (tile->name + ".png").c_str());
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void LineStrip::draw(cairo_t *cr, int xOffset, int yOffset) const {
    cairo_set_line_width(cr, lineWidth);
    cairo_set_source_rgb(cr, red / 256.0, green / 256.0, blue / 256.0);
    cairo_move_to(cr, lineStrip->nodes[0]->x - xOffset, lineStrip->nodes[0]->y - yOffset);
    for (int i = 1; i < lineStrip->nodes.size(); i++) {
        cairo_line_to(cr, lineStrip->nodes[i]->x - xOffset, lineStrip->nodes[i]->y - yOffset);
    }
    cairo_stroke(cr);
}

void Polygon::draw(cairo_t *cr, int xOffset, int yOffset) const {
    if (area->nodes.empty()) return;

    cairo_set_source_rgb(cr, red / 256.0, green / 256.0, blue / 256.0);
    cairo_move_to(cr, area->nodes[0]->x - xOffset, area->nodes[0]->y - yOffset);
    for (int i = 1; i < area->nodes.size(); i++) {
        cairo_line_to(cr, area->nodes[i]->x - xOffset, area->nodes[i]->y - yOffset);
    }
    cairo_fill(cr);

    if (drawOutline) {
        cairo_set_line_width(cr, lineWidth);
        cairo_set_source_rgb(cr, redOutline / 256.0, greenOutline / 256.0, blueOutline / 256.0);
        cairo_move_to(cr, area->nodes[0]->x - xOffset, area->nodes[0]->y - yOffset);
        for (int i = 1; i < area->nodes.size(); i++) {
            cairo_line_to(cr, area->nodes[i]->x - xOffset, area->nodes[i]->y - yOffset);
        }
        cairo_stroke(cr);
    }
}

//https://lists.cairographics.org/archives/cairo/2008-November/015784.html
void Multipolygon::draw(cairo_t *cr, int xOffset, int yOffset) const {
    if (points.empty()) return;

    cairo_set_source_rgb(cr, red / 256.0, green / 256.0, blue / 256.0);
    cairo_move_to(cr, points[0]->x - xOffset, points[0]->y - yOffset);
    for (int i = 1; i < points.size(); i++) {
        cairo_line_to(cr, points[i]->x - xOffset, points[i]->y - yOffset);
    }

    for (auto hole: holes) {
        if (hole->nodes.empty()) continue;
        cairo_move_to(cr, hole->nodes[0]->x - xOffset, hole->nodes[0]->y - yOffset);
        for (int i = 1; i < hole->nodes.size(); i++) {
            cairo_line_to(cr, hole->nodes[i]->x - xOffset, hole->nodes[i]->y - yOffset);
        }
    }

    cairo_fill(cr);
}

void Tiles::addLineStrip(LineStrip &lineStrip) {
    double x, y, x2, y2;
    lineStrip.lineStrip->box(x, y, x2, y2);

    int ix, iy, ix2, iy2;
    calculateIndex(x, y, x2, y2, ix, iy, ix2, iy2);

    for (int j = iy; j <= iy2; j++) {
        for (int i = ix; i <= ix2; i++) {
            auto index = j * tileXCount + i;
            if (index >= regions.size()) continue;
            regions[index].lineStrips.push_back(lineStrip);
        }
    }
}

void Tiles::addArea(Polygon &area) {
    double x, y, x2, y2;
    area.area->box(x, y, x2, y2);

    int ix, iy, ix2, iy2;
    calculateIndex(x, y, x2, y2, ix, iy, ix2, iy2);

    for (int j = iy; j <= iy2; j++) {
        for (int i = ix; i <= ix2; i++) {
            auto index = j * tileXCount + i;
            if (index >= regions.size()) continue;
            regions[index].areas.push_back(area);
        }
    }
}

void Tiles::addArea(Multipolygon &area) {
    double x, y, x2, y2;
    area.relation->box(x, y, x2, y2);

    int ix, iy, ix2, iy2;
    calculateIndex(x, y, x2, y2, ix, iy, ix2, iy2);

    for (int j = iy; j <= iy2; j++) {
        for (int i = ix; i <= ix2; i++) {
            auto index = j * tileXCount + i;
            if (index >= regions.size()) continue;
            regions[index].areas2.push_back(area);
        }
    }
}

void Multipolygon::build(Relation *r) {
    relation = r;
    std::map<Way *, std::string> outer;
    for (const auto &way: relation->ways) {
        if (strcmp(way.second.c_str(), "outer") == 0) {
            outer[way.first] = "";
        } else if (strcmp(way.second.c_str(), "inner") == 0) {
            holes.push_back(way.first);
        }
    }
    sortPoints(outer, points);
}

void Tiles::calculateIndex(double x, double y, double x2, double y2, int &ix, int &iy, int &ix2, int &iy2) const {
    ix = (x / width) * tileXCount;
    iy = (y / height) * tileYCount;
    ix2 = (x2 / width) * tileXCount;
    iy2 = (y2 / height) * tileYCount;
}
