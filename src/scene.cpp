#include "scene.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "game/controltester.hpp"
#include "ui/fpscounter.hpp"
#include "ui/textbutton.hpp"
#include "game/blocks.hpp"
#include "game/blocksview.hpp"
#include "game/blocksdataview.hpp"

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

    TextButton* createEmptyWindowBtn = new TextButton(nullptr, "Create empty window", [this](){ createWindow(); });
    createEmptyWindowBtn->setPosition(50, 50);

    TextButton* createTextureWindowBtn = new TextButton(nullptr, "Create texture window", [this](){ createWindow("mnc_arrow.png"); });
    createTextureWindowBtn->setPosition(50, 80);

    TextButton* createBlocksWindowBtn = new TextButton(nullptr, "Create Blocks(tm) window", [this](){ createBlocksWindow(0); });
    createBlocksWindowBtn->setPosition(50, 110);

    TextButton* createBlocksLvl500WindowBtn = new TextButton(nullptr, "Create Blocks(tm) window", [this](){ createBlocksWindow(500); });
    createBlocksLvl500WindowBtn->setPosition(50, 130);

    return true;
}

void Scene::createWindow(const std::string p_textureAssetName) {
    std::string windowId = "window" + to_string(_windows.size());
    int randWidth = 100 + rand() % 300;
    int randHeight = 100 + rand() % 300;
    int randX = rand() % (Scene::sceneWidth - randWidth);
    int randY = rand() % (Scene::sceneHeight - randHeight);

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

void Scene::createBlocksWindow(const int p_startLevel) {
    std::string windowId = "Blocks " + to_string(_windows.size());
    int width = 320;
    int height = 440;
    int randX = rand() % (Scene::sceneWidth - width);
    int randY = rand() % (Scene::sceneHeight - height);

    Window* window = new Window(windowId, width, height, [this, windowId](){
        deleteWindow(windowId);
    });

    Blocks* blocks = new Blocks(10, 20);
    blocks->setLevel(p_startLevel);

    BlocksView* blocksView = new BlocksView(window, blocks, 20);
    blocksView->setPosition(10, 30);
    BlocksDataView* blocksDataView = new BlocksDataView(window, blocks);
    blocksDataView->setPosition(220, 30);

    window->setPosition(randX, randY);

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