#include "scene.hpp"

#include "ui/closebutton.hpp"
#include "ui/window.hpp"

bool Scene::init() {
    _window = new Window(400, 600);
    _window->setPosition(100, 100);

    _closeButton = new CloseButton();
    _closeButton->setPosition(488, 112);
    
    return true;
}