#pragma once    

#include <functional>

#include "engine/gameobject.hpp"

class Updatable : public GameObject {
    public:
        Updatable(GameObject* p_parent, std::function<void(const double&)> p_onUpdateCallback);
        ~Updatable();

        void update(const double& p_deltaTime);
    
    private:
        std::function<void(const double&)> _onUpdateCallback;
};