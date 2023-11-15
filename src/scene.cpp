#include "scene.hpp"

#include "engine/displayable.hpp"

bool Scene::init() {
    _window = new Window(400, 600);
    _window->setPosition(100, 100);

    _closeButton = new CloseButton();
    _closeButton->setPosition(388, 12);

    _closeButton->setParent(_window);

    // Font test
    //Renderer::addText("This is a sample text", "assets/font/Smaco_Regular8.ttf", { 255, 255, 255, 255 }, 16);
    
    return true;
}