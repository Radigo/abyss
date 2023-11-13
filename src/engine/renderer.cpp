#include "renderer.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_ttf.h>

#include <string>

SDL_Renderer* Renderer::_renderer = nullptr;
vector<Drawable*> Renderer::_drawList = vector<Drawable*>();
vector<SDL_Texture*> Renderer::_textures;

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

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Renderer::addDraw(Drawable* p_drawable) {
    _drawList.emplace_back(p_drawable);
}

void Renderer::addText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		SDL_Log("TTF_OpenFont failed");
		return;
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		SDL_Log("TTF_RenderText_Blended failed");
		return;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surf);
	if (texture == nullptr) {
		SDL_Log("SDL_CreateTextureFromSurface failed");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

    _textures.emplace_back(texture);
}

void Renderer::update() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
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
            SDL_RenderDrawLine(_renderer, drawableX + line.x1, drawableY + line.y1, drawableX + line.x2, drawableY + line.y2);
        }

        for (Drawable::Rectangle rect : drawable->getDrawableRectangles()) {
            SDL_Rect sdlRect = {drawableX + rect.x, drawableY + rect.y, rect.w, rect.h};

            SDL_SetRenderDrawColor(_renderer, rect.inColor.r, rect.inColor.g, rect.inColor.b, rect.inColor.a);
            SDL_RenderFillRect(_renderer, &sdlRect);

            SDL_SetRenderDrawColor(_renderer, rect.outColor.r, rect.outColor.g, rect.outColor.b, rect.outColor.a);
            SDL_RenderDrawRect(_renderer, &sdlRect);
        }

        for (SDL_Texture* texture : _textures) {
            SDL_RenderCopy(_renderer, texture, NULL, NULL);
        }
    }

    SDL_RenderPresent(_renderer);
}