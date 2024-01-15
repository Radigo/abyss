#pragma once

#include <algorithm>
#include <vector>
#include <string>

#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>

#include "engine/displayable.hpp"
#include "engine/types.hpp"

class SDL_Color;
class SDL_Renderer;
class SDL_Texture;
class SDL_Window;

class Renderer {
    private:
        static SDL_Renderer* _renderer;
        static std::vector<Displayable*> _displayList;

    public:
        bool init(SDL_Window* p_window);

        inline static size_t addDisplayable(Displayable* p_displayable) {
            _displayList.push_back(p_displayable);
            return _displayList.size();
        }
        inline static size_t removeDisplayable(Displayable* p_displayable) {
            auto it = std::find(_displayList.begin(), _displayList.end(), p_displayable);
            if (it == _displayList.end())
                static_assert(true, "removing Displayable that is NOT in Renderer::_displayList");
            _displayList.erase(it);
            return _displayList.size();
        }

        static Displayable::DisplayableTexture createDisplayableTexture(const std::string& p_assetName);
        static Displayable::DisplayableTexture createDisplayableText(const std::string& p_content, const Types::DisplayableFont& p_font, const Displayable::Color& p_color = {255, 255, 255, 255});

        void update(const double& p_deltaTime);

    private:
};