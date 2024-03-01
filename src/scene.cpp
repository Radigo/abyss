#include "scene.hpp"

#include "engine/displayable.hpp"
#include "engine/renderer.hpp"
#include "game/controltester.hpp"
#include "game/blocks.hpp"
#include "game/blocksview.hpp"
#include "game/blocksdataview.hpp"
#include "ui/fpscounter.hpp"
#include "ui/textbutton.hpp"
#include "bot/bot.hpp"
#include "bot/botdataview.hpp"

#include <time.h> // For rand only, try a fixed seed

#include "SDL2/SDL_log.h"

int Scene::sceneWidth = 0;
int Scene::sceneHeight = 0;

Scene::Scene() :
GameObject(nullptr)
{
    
}

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

    TextButton* createBlocksTGMWindowBtn = new TextButton(nullptr, "Blocks(tm) TGM", [this](){ createBlocksWindow(1, 0, 10, 20, false); });
    createBlocksTGMWindowBtn->setPosition(50, 110);

    TextButton* createBlocksTAPWindowBtn = new TextButton(nullptr, "Blocks(tm) TAP", [this](){ createBlocksWindow(2, 0, 10, 20, false); });
    createBlocksTAPWindowBtn->setPosition(50, 130);

    TextButton* createBlocksLvl500WindowBtn = new TextButton(nullptr, "20G Blocks(tm) window", [this](){ createBlocksWindow(2, 500, 10, 20, false); });
    createBlocksLvl500WindowBtn->setPosition(50, 150);

    TextButton* createBlocksRandWindowBtn = new TextButton(nullptr, "Random size Blocks(tm) window", [this](){
        int version = 1 + rand() % 2;
        int numColumns = 5 + rand() % 30;
        int numRows = 10 + rand() % 60;
        createBlocksWindow(version, 0, numColumns, numRows, false);
    });
    createBlocksRandWindowBtn->setPosition(50, 170);

    TextButton* createBlocksBotWindowBtn = new TextButton(nullptr, "Bot Blocks(tm) window", [this](){
        createBlocksWindow(1, 0, 10, 20, true);
    });
    createBlocksBotWindowBtn->setPosition(50, 200);

    TextButton* createBlocksBotRandWindowBtn = new TextButton(nullptr, "Bot Random size Blocks(tm) window", [this](){
        int version = 1 + rand() % 2;
        int numColumns = 5 + rand() % 30;
        int numRows = 10 + rand() % 60;
        createBlocksWindow(version, 0, numColumns, numRows, true);
    });
    createBlocksBotRandWindowBtn->setPosition(50, 220);

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

void Scene::createBlocksWindow(const int p_version, const int p_startLevel, const int p_numColumns, const int p_numRows, const bool p_isPlayedByBot) {
    std::string windowId = "Blocks " + to_string(_windows.size());
    int cellSize = 16;
    int width = cellSize * p_numColumns + 100;
    int height = cellSize * (p_numRows + Blocks::BUFFER_ROWS) + 36;
    int dataViewX = cellSize * p_numColumns + 20;
    int randX = rand() % (Scene::sceneWidth - width);
    int randY = rand() % (Scene::sceneHeight - height);

    Window* window = new Window(windowId, width, height, [this, windowId](){
        deleteWindow(windowId);
    });

    Blocks* blocks = new Blocks(p_version, p_numColumns, p_numRows);
    blocks->setLevel(p_startLevel);

    BlocksView* blocksView = new BlocksView(window, blocks, cellSize);
    blocksView->setPosition(10, 30);
    BlocksDataView* blocksDataView = new BlocksDataView(window, blocks);
    blocksDataView->setPosition(dataViewX, 30);

    if (p_isPlayedByBot) {
        Bot* bot = new Bot(blocks);
        /*BotDataView* botDataView = */new BotDataView(blocksView, blocks, bot, cellSize);
    }

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