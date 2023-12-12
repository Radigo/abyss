#include <string.h>

class Globals {
    public:
        static std::string APP_PATH;
        static std::string ASSETS_PATH;

        static int WINDOW_WIDTH;
        static int WINDOW_HEIGHT;

        static const int FPS_TARGET = 60;
        static const int WINDOW_WIDTH_INIT = 1280;
        static const int WINDOW_HEIGHT_INIT = 720;
        static const int WINDOW_WIDTH_MIN = 320;
        static const int WINDOW_HEIGHT_MIN = 200;
};