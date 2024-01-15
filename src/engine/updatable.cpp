#include "updatable.hpp"

#include <algorithm>
#include <string>

#include "engine/updater.hpp"

#include <SDL2/SDL_log.h>

Updatable::Updatable(GameObject* p_parent, std::function<void(const double&)> p_onUpdateCallback) :
GameObject(p_parent)
{
    size_t numUpdatables = Updater::addUpdatable(this);
    _id = "Updatable" + std::to_string(numUpdatables);
    _onUpdateCallback = p_onUpdateCallback;
}

Updatable::~Updatable() {
    Updater::removeUpdatable(this);
}

void Updatable::update(const double& p_deltaTime) {
    if (_onUpdateCallback) {
        _onUpdateCallback(p_deltaTime);
    }
}