#include "updater.hpp"

#include "updatable.hpp"

std::vector<Updatable*> Updater::_updatableList;

void Updater::update(const double& p_deltaTime) {
    for (Updatable* updatable : Updater::_updatableList) {
        updatable->update(p_deltaTime);
    }
}