#pragma once

#include "engine/gameobject.hpp"

class Controllable;
class Displayable;
class Updatable;

class ControlTester : public GameObject {
    public:
        ControlTester(GameObject* p_parent);
        ~ControlTester();

    private:
        Displayable* _debugTf;
        size_t _tfIndex;

        Controllable* _inputReader;
        Updatable* _updater;

        void _updateDebugTf(const double& p_deltaTime);
};