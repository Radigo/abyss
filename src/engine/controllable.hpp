#pragma once

#include "engine/gameobject.hpp"

#include <vector>

class Controllable : public GameObject {
    public:
        static std::vector<Controllable*> controllableList;

        Controllable();
        ~Controllable();

        virtual void onMouseMove();
        virtual void onMouseButtonDown();
        virtual void onMouseButtonUp();
        virtual void onMouseWheel();

    private:

};