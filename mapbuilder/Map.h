//
// Created by abris on 4/30/24.
//

#ifndef MWP_MAP_H
#define MWP_MAP_H


#include "Osm.h"
#include <cairo/cairo.h>
#include <thread>

class LineStrip {
public:
    Way *lineStrip;
    unsigned short red, green, blue;
    double lineWidth = 1;

    explicit LineStrip(Way *way, unsigned short red, unsigned short green, unsigned short blue, double lineWidth);

    void draw(cairo_t *cr, int xOffset, int yOffset) const;

};

class Polygon {
public:
    Way *area;
    unsigned short red, green, blue, redOutline, greenOutline, blueOutline;
    bool drawOutline = false;
    double lineWidth = 1;

    explicit Polygon(Way *way, unsigned short red, unsigned short green, unsigned short blue);

    explicit Polygon(Way *way, unsigned short red, unsigned short green, unsigned short blue, unsigned short redOutline,
                     unsigned short greenOutline, unsigned short blueOutline, double lineWidth);

    void draw(cairo_t *cr, int xOffset, int yOffset) const;
};

class Multipolygon {
public:
    Relation *relation;
    std::vector<Node *> points;
    std::vector<Way *> holes;
    unsigned short red, green, blue, redOutline, greenOutline, blueOutline;
    bool drawOutline = false;
    int lineWidth;

    explicit Multipolygon(Relation *relation, unsigned short red, unsigned short green, unsigned short blue);

    explicit Multipolygon(Relation *relation, unsigned short red, unsigned short green, unsigned short blue,
                          unsigned short redOutline, unsigned short greenOutline, unsigned short blueOutline,
                          int lineWidth);

    void draw(cairo_t *cr, int xOffset, int yOffset) const;

    void build(Relation *relation);
};

class Tile {
public:
    std::string name;
    int xOffset, yOffset;
    std::vector<LineStrip> lineStrips;
    std::vector<Polygon> areas;
    std::vector<Multipolygon> areas2;

    explicit Tile(int xOffset, int yOffset, std::string name);
};

class Tiles {
    void calculateIndex(double x, double y, double x2, double y2, int &ix, int &iy, int &ix2, int &iy2) const;

public:
    int tileWidth, tileHeight, tileXCount, tileYCount;
    double width, height;
    std::vector<Tile> regions;

    explicit Tiles(int tileWidth, int tileHeight, double width, double height);

    void addLineStrip(LineStrip &lineStrip);

    void addArea(Polygon &area);

    void addArea(Multipolygon &area);
};

class Map {
public:
    Osm *osm;
    Tiles regions;

    explicit Map(Osm &osm);

    void write();

    void area(const std::string &key, const std::string &value, unsigned short red, unsigned short green,
              unsigned short blue);

    void area(const std::string &key, const std::string &value, unsigned short red, unsigned short green,
              unsigned short blue, unsigned short redOutline, unsigned short greenOutline, unsigned short blueOutline);

    void area(const std::string &key, unsigned short red, unsigned short green,
              unsigned short blue, unsigned short redOutline, unsigned short greenOutline,
              unsigned short blueOutline);

    void lineStrip(const std::string &key, const std::string &value, unsigned short red, unsigned short green,
                   unsigned short blue, double lineWidth);

    void
    lineStrip(const std::string &key, unsigned short red, unsigned short green, unsigned short blue, double lineWidth);
};

void writeTiles(Tiles *tiles, int from, int to);

void writeTile(Tile *tile, int tileWidth, int tileHeight);

#endif //MWP_MAP_H
