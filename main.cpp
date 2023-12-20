#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <string.h>

#include "engine/globals.hpp"
#include "engine/input.hpp"
#include "engine/renderer.hpp"
#include "engine/updater.hpp"
#include "scene.hpp"

std::string Globals::APP_PATH = "null";
std::string Globals::ASSETS_PATH = "null";
int Globals::WINDOW_WIDTH = -1;
int Globals::WINDOW_HEIGHT = -1;

int main(int /*argc*/, char** /*argv[]*/)
{
    std::cout << "Start\n- - -\n\n";
    
    // initiate SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("[ERROR] %s\n", SDL_GetError());
        return -1;
    }

    SDL_version compiled;
    SDL_VERSION(&compiled);
    std::ostringstream compiledVal;
    compiledVal << "Compiled with "
                << std::to_string(compiled.major)
                << "." << std::to_string(compiled.minor)
                << "." << std::to_string(compiled.patch);
    std::cout << compiledVal.str() << std::endl;

    SDL_version linked;
    SDL_GetVersion(&linked);
    std::ostringstream linkedVal;
    linkedVal << "Linked with "
              << std::to_string(linked.major)
              << "." << std::to_string(linked.minor)
              << "." << std::to_string(linked.patch);
    std::cout << linkedVal.str() << std::endl;

    // setup environment
    Globals::APP_PATH = SDL_GetBasePath();
    Globals::ASSETS_PATH = strcat(const_cast<char*>(Globals::APP_PATH.c_str()), "assets/");

    // setup SDL window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE
        );

    std::string glsl_version = "";
#ifdef __APPLE__
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute( // required on Mac OS
        SDL_GL_CONTEXT_FLAGS,
        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
        );
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_RESIZABLE
        );
#elif __linux__
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL
        | SDL_WINDOW_ALLOW_HIGHDPI
        | SDL_WINDOW_RESIZABLE
        );
#endif

    SDL_Window *window = SDL_CreateWindow(
        "OpenGL SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Globals::WINDOW_WIDTH_INIT,
        Globals::WINDOW_HEIGHT_INIT,
        window_flags
        );
    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(window, Globals::WINDOW_WIDTH_MIN, Globals::WINDOW_HEIGHT_MIN);
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == NULL)
    {
        std::cerr << "[ERROR] Failed to create a GL context: "
                  << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_GL_MakeCurrent(window, gl_context);
    
    // enable VSync
    //SDL_GL_SetSwapInterval(1);
    
    // Create renderer
    Renderer renderer;
    if (!renderer.init(window)) {
        std::cerr << "[ERROR] Failed to create the renderer" << std::endl;
        return -1;
    }

    // Create input controller
    Input input;

    //Initialisation de SDL_TTF 
	if (TTF_Init() == -1) { 
		return -1; 
	} 

    // Create Scene
    Scene scene;
    if (!scene.init(Globals::WINDOW_WIDTH_INIT, Globals::WINDOW_HEIGHT_INIT)) {
        std::cerr << "[ERROR] Failed to create the main scene" << std::endl;
        return -1;
    }

    // --- main loop
    bool loop = true;
    std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
    std::chrono::duration<long, std::micro> deltaTime = std::chrono::microseconds(1);
    const auto kFrameDuration = std::chrono::system_clock::duration(1000000 / Globals::FPS_TARGET);
    
    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    loop = false;
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                    case SDL_WINDOWEVENT_RESIZED:
                        Globals::WINDOW_WIDTH = event.window.data1;
                        Globals::WINDOW_HEIGHT = event.window.data2;
                        std::cout << "[INFO] Window size: "
                                  << Globals::WINDOW_WIDTH
                                  << "x"
                                  << Globals::WINDOW_HEIGHT
                                  << std::endl;
                        // Formerly GL window resize call
                        scene.sceneWidth = Globals::WINDOW_WIDTH;
                        scene.sceneHeight = Globals::WINDOW_HEIGHT;
                        break;
                    }
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        loop = false;
                        break;
                    default:
                        input.onKey(event.key);
                        break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    input.onMouseMove(event.motion);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    input.onMouseButtonDown(event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    input.onMouseButtonUp(event.button);
                    break;
            }
        }

        // Make sure we render only at FPS target
        double timeRatio = static_cast<double>(deltaTime.count()) / static_cast<double>(kFrameDuration.count());
        
        Updater::update(timeRatio);
        renderer.update(timeRatio);
        
        /*
        Windows (but try it on Mac too)
        */
        deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - currentTime);

        while (kFrameDuration > deltaTime) {
            deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - currentTime);
        }

        /*
        MacOs
        deltaTime = std::chrono::system_clock::now() - currentTime;

        while (kFrameDuration > deltaTime) {
            deltaTime = std::chrono::system_clock::now() - currentTime;
        }
        */

        currentTime = std::chrono::system_clock::now();
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "\n- - -\nQuit\n";
    
    return 0;
}