#include "controllable.hpp"

std::vector<Controllable*> Controllable::controllableList;

Controllable::Controllable() {
    Controllable::controllableList.emplace_back(this);
}

Controllable::~Controllable() {
    
}