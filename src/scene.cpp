#include "scene.hpp"

#include "engine/drawable.hpp"

bool Scene::init() {
    
    _draw1 = new Drawable();
    _draw1->addLine(2, 2, 18, 18);
    _draw1->addLine(2, 18, 18, 2);
    _draw1->addRectangle(0, 0, 22, 22);
    _draw1->setPosition(102, 102);

    _draw2 = new Drawable();
    _draw2->addRectangle(0, 0, 400, 300);
    _draw2->setPosition(100, 100);

    return true;
}