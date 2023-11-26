#include "scene.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"

#include <time.h> // For rand only, try a fixed seed

#include "SDL2/SDL_log.h"

bool Scene::init(int p_sceneWidth, int p_sceneHeight) {
    sceneWidth = p_sceneWidth;
    sceneHeight = p_sceneHeight;

    /* initialize random seed: */
    srand(time(NULL));

    _fpsCounter = new FpsCounter(nullptr);

    CloseButton* createEmptyWindow = new CloseButton(nullptr, 30, [this](){ createWindow(); });
    createEmptyWindow->setPosition(50, 50);

    CloseButton* createTextureWindow = new CloseButton(nullptr, 30, [this](){ createWindow("mnc_arrow.png"); });
    createTextureWindow->setPosition(100, 50);

    return true;
}

void Scene::createWindow(const std::string p_textureAssetName) {
    std::string windowId = "window" + to_string(_windows.size());
    int randWidth = 100 + rand() % 300;
    int randHeight = 100 + rand() % 300;
    int randX = rand() % sceneWidth;
    int randY = rand() % sceneHeight;

    Window* window = new Window(windowId, randWidth, randHeight, [this, windowId](){
        deleteWindow(windowId);
    });
    window->setPosition(randX, randY);

    if (!p_textureAssetName.empty()) {
        Displayable* texture = new Displayable(window);
        texture->addTexture(Renderer::createDisplayableTexture(p_textureAssetName.c_str()));
        texture->setPosition(10, 30);
    }

    _windows.push_back(window);
}

void Scene::deleteWindow(const std::string p_id) {
    for (size_t i = 0; i < _windows.size(); i++) {
        Window* window = _windows.at(i);
        if (window->getId() == p_id) {
            _windows.erase(_windows.begin() + i);
            delete(window);
            break;
        }
    }
}