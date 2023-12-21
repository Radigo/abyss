#pragma once

#include <vector>
#include <string>

//#include "ui/fpscounter.hpp"
#include "ui/window.hpp"

class SDL_Texture;
class SDL_Color;
class SDL_Renderer;
class FpsCounter;
class ControlTester;

using namespace std;

class Scene : public GameObject {
    public:
        static int sceneWidth;
        static int sceneHeight;

    private:
        FpsCounter* _fpsCounter;
        ControlTester* _controlTester;
        std::vector<Window*> _windows;

        void createWindow(const std::string p_textureAssetName = "");
        void deleteWindow(const std::string p_id);

    public:
        bool init(int p_sceneWidth, int p_sceneHeight);
};