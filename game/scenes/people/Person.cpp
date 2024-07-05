//
// Created by abris on 5/30/24.
//

#include "Person.h"

void Person::input(sf::Event &event) {

}

void Person::update() {

}

void Person::render(sf::RenderWindow &window) {
    window.draw(shape);
}

Person::Person() : Scene(), shape(sf::Vector2f{10, 10}){
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(100, 100);

}
