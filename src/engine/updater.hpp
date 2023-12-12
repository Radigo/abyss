#pragma once

#include <algorithm>
#include <vector>

class Updatable;

class Updater {
    public:
        inline static size_t addUpdatable(Updatable* p_updatable) {
            _updatableList.push_back(p_updatable);
            return _updatableList.size();
        }
        inline static size_t removeUpdatable(Updatable* p_updatable) {
            auto it = std::find(_updatableList.begin(), _updatableList.end(), p_updatable);
            if (it == _updatableList.end())
                static_assert(true, "removing Updatable that is NOT in Updater::_updatableList");
            _updatableList.erase(it);
            return _updatableList.size();
        }

        static void update(const double& p_deltaTime);

    private:
        static std::vector<Updatable*> _updatableList;
};