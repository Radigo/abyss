#pragma once    

#include <functional>

#include "engine/gameobject.hpp"

class Updatable : public GameObject {
    public:
        Updatable(GameObject* p_parent, std::function<void(const double&, const double&)> p_onUpdateCallback);
        ~Updatable();

        void update(const double& p_frameDeltaTime, const double& p_updateDeltaTime);
    
    private:
        std::function<void(const double&, const double&)> _onUpdateCallback;
};