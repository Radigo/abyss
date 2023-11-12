#include "renderer.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_log.h>

vector<Drawable*> Renderer::_drawList = {};

bool Renderer::init(SDL_Window* p_window) {
    _renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RendererInfo infoRenderer;
    SDL_GetRendererInfo(_renderer, &infoRenderer);

    if (infoRenderer.flags & SDL_RENDERER_ACCELERATED)
    {
        SDL_Log("Accelerated render available");
    }

    if (infoRenderer.flags & SDL_RENDERER_SOFTWARE)
    {
        SDL_Log("Software render available");
    }

    if (infoRenderer.flags & SDL_RENDERER_TARGETTEXTURE)
    {
        SDL_Log("Texture render available");
    }

    return true;
}

void Renderer::addDraw(Drawable* p_drawable) {
    _drawList.emplace_back(p_drawable);
}

void Renderer::update() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    
    // Draw all drawables
    for (Drawable* drawable : _drawList) {
        int drawableX = drawable->getX();
        int drawableY = drawable->getY();

        for (Drawable::Point point : drawable->getDrawablePoints()) {
            SDL_SetRenderDrawColor(_renderer, point.color.r, point.color.g, point.color.b, point.color.a);
            SDL_RenderDrawPoint(_renderer, drawableX + point.x, drawableY + point.y);
        }

        for (Drawable::Line line : drawable->getDrawableLines()) {
            SDL_SetRenderDrawColor(_renderer, line.color.r, line.color.g, line.color.b, line.color.a);
            SDL_RenderDrawLine(_renderer, drawableX + line.x1, drawableX + line.x2, drawableY + line.y1, drawableY + line.y2);
        }

        for (Drawable::Rectangle rect : drawable->getDrawableRectangles()) {
            SDL_Rect sdlRect = {drawableX + rect.x, drawableY + rect.y, rect.w, rect.h};
            SDL_SetRenderDrawColor(_renderer, rect.outColor.r, rect.outColor.g, rect.outColor.b, rect.outColor.a);
            SDL_RenderDrawRect(_renderer, &sdlRect);
        }
    }

    SDL_RenderPresent(_renderer);
}