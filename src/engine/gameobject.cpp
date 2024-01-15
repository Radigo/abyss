#include "gameobject.hpp"

std::vector<std::pair<GameObject*, GameObject*>> GameObject::_hierarchy = {};

GameObject::GameObject(GameObject* p_parent) :
_id(""),
_x(0),
_y(0)
{
    setParent(p_parent);
}

GameObject::~GameObject() {
    // Delete all children as well
    for (GameObject* child : getChildren()) {
        delete(child);
    }
    setParent(nullptr);
}

void GameObject::setPosition(const int p_x, const int p_y) {
    _x = p_x;
    _y = p_y;
}

void GameObject::setParent(GameObject* p_gameobject) {
    auto it = std::find_if(_hierarchy.begin(), _hierarchy.end(), [this](const std::pair<GameObject*, GameObject*>& item){
        return (item.second == this);
    });

    if (p_gameobject == nullptr) {
        // Setting no parent
        if (it != _hierarchy.end()) {
            // We have a parent
            _hierarchy.erase(it);
        }
    } else {
        if (it == _hierarchy.end()) {
            // Setting a parent
            _hierarchy.push_back(std::make_pair(p_gameobject, this));
        } else {
            // Changing parent
            (*it).first = p_gameobject;
        }
    }
}

GameObject* GameObject::getParent() {
    auto it = std::find_if(_hierarchy.begin(), _hierarchy.end(), [this](const std::pair<GameObject*, GameObject*>& item){
        return (item.second == this);
    });
    if (it != _hierarchy.end())
        return (*it).first;
    return nullptr;
}

std::vector<GameObject*> GameObject::getChildren() {
    std::vector<GameObject*> children = {};

    for (auto it = _hierarchy.begin(); it != _hierarchy.end(); it++) {
        if ((*it).first == this) {
            children.push_back((*it).second);
        }
    }

    return children;
}