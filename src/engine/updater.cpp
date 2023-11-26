#include "updater.hpp"

#include "updatable.hpp"

std::vector<Updatable*> Updater::_updatableList;

void Updater::update(const double& p_frameDeltaTime, const double& p_updateDeltaTime) {
    for (Updatable* updatable : Updater::_updatableList) {
        updatable->update(p_frameDeltaTime, p_updateDeltaTime);
    }
}