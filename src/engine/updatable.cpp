#include "updatable.hpp"

#include <algorithm>
#include <string>

#include "engine/updater.hpp"

#include <SDL2/SDL_log.h>

Updatable::Updatable(GameObject* p_parent, std::function<void(const double&, const double&)> p_onUpdateCallback) {
    size_t numUpdatables = Updater::addUpdatable(this);
    _id = "Updatable" + std::to_string(numUpdatables);
    _parent = p_parent;
    _onUpdateCallback = p_onUpdateCallback;
}

Updatable::~Updatable() {
    _parent = nullptr;
    
    Updater::removeUpdatable(this);
}

void Updatable::update(const double& p_frameDeltaTime, const double& p_updateDeltaTime) {
    if (_onUpdateCallback) {
        _onUpdateCallback(p_frameDeltaTime, p_updateDeltaTime);
    }
}