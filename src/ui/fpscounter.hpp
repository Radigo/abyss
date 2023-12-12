#pragma once

#include <vector>
#include "engine/gameobject.hpp"

class Displayable;
class Updatable;

class FpsCounter : public GameObject {
    public:
        FpsCounter(GameObject* p_parent);
        ~FpsCounter();

    private:
        Displayable* _display;
        Displayable* _digits;
        Updatable* _updater;

        size_t _tfIndex;

        void _updateCounter(const double& p_deltaTime);
};