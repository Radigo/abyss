#include "scene.hpp"

#include "engine/displayable.hpp"

#include "SDL2/SDL_log.h"

bool Scene::init() {
    _window = new Window(400, 600);
    _window->setPosition(100, 100);

    _closeButton = new CloseButton();
    _closeButton->setPosition(388, 12);

    _closeButton->setParent(_window);

    // We didn't change the position of displayable inside 'window' and 'button' with setPosition, just the GameObject position.
    // The renderer gets x and y from displayable only, not 'window'
    // - We want to keep position in GameObject for logic
    // - Displayable should probably not be a GameObject
    SDL_Log("scene: %s, %d, %d", _window->getId().c_str(), _window->getX(), _window->getY());
    SDL_Log("scene: %s, %d, %d", _closeButton->getId().c_str(), _closeButton->getX(), _closeButton->getY());

    // Font test
    //Renderer::addText("This is a sample text", "assets/font/Smaco_Regular8.ttf", { 255, 255, 255, 255 }, 16);
    
    return true;
}