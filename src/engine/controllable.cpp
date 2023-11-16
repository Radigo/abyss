#include "controllable.hpp"

//#include <string>

std::vector<Controllable*> Controllable::controllableList;

Controllable::Controllable() {
    Controllable::controllableList.emplace_back(this);
    _id = "Displayable" + std::to_string(Controllable::controllableList.size());
}

Controllable::~Controllable() {
    
}

void Controllable::onMouseMove() {

}

void Controllable::onMouseButtonDown() {

}

void Controllable::onMouseButtonUp() {

}

void Controllable::onMouseWheel() {

}