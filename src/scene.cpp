#include "scene.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "game/controltester.hpp"
#include "ui/fpscounter.hpp"
#include "ui/textbutton.hpp"
#include "game/blocks.hpp"
#include "game/blocksview.hpp"

#include <time.h> // For rand only, try a fixed seed

#include "SDL2/SDL_log.h"

int Scene::sceneWidth = 0;
int Scene::sceneHeight = 0;

bool Scene::init(int p_sceneWidth, int p_sceneHeight) {
    Scene::sceneWidth = p_sceneWidth;
    Scene::sceneHeight = p_sceneHeight;

    /* initialize random seed: */
    srand(time(NULL));

    _fpsCounter = new FpsCounter(nullptr);
    _controlTester = new ControlTester(nullptr);

    TextButton* createEmptyWindow = new TextButton(nullptr, "Create empty window", [this](){ createWindow(); });
    createEmptyWindow->setPosition(50, 50);

    TextButton* createTextureWindow = new TextButton(nullptr, "Create texture window", [this](){ createWindow("mnc_arrow.png"); });
    createTextureWindow->setPosition(50, 100);

    Blocks* blocks = new Blocks(10, 20);
    BlocksView* blocksView = new BlocksView(this, blocks);
    blocksView->setPosition(400, 10);

    return true;
}

void Scene::createWindow(const std::string p_textureAssetName) {
    std::string windowId = "window" + to_string(_windows.size());
    int randWidth = 100 + rand() % 300;
    int randHeight = 100 + rand() % 300;
    int randX = rand() % Scene::sceneWidth;
    int randY = rand() % Scene::sceneHeight;

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