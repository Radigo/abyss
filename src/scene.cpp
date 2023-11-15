#include "scene.hpp"

#include "engine/displayable.hpp"

bool Scene::init() {
    int CLOSE_BUTTON_SIZE = 20;
    Displayable* sprite = new Displayable();
    sprite->addRectangle(-CLOSE_BUTTON_SIZE * 0.5f, -CLOSE_BUTTON_SIZE * 0.5f, CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE, Displayable::Color(255, 255, 255, 255), Displayable::Color(255, 255, 255, 64));
    sprite->addLine(-CLOSE_BUTTON_SIZE * 0.4f, -CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f);
    sprite->addLine(-CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, -CLOSE_BUTTON_SIZE * 0.4f);
    sprite->setPosition(100, 100);

    _window = new Window(400, 600);
    _window->setPosition(100, 100);

    _closeButton = new CloseButton();
    _closeButton->setPosition(488, 112);

    // Font test
    //Renderer::addText("This is a sample text", "assets/font/Smaco_Regular8.ttf", { 255, 255, 255, 255 }, 16);
    
    return true;
}