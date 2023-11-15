#pragma once

#include <vector>

class Controllable {
    public:
        static std::vector<Controllable*> controllableList;

        Controllable();
        ~Controllable();
};