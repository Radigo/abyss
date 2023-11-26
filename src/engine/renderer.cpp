#include "renderer.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>
#include <vector>

#include "engine/globals.hpp"
#include "engine/displayable.hpp"
#include "engine/types.hpp"

SDL_Renderer* Renderer::_renderer = nullptr;
std::vector<Displayable*> Renderer::_displayList;

bool Renderer::init(SDL_Window* p_window) {
    _renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RendererInfo infoRenderer;
    SDL_GetRendererInfo(_renderer, &infoRenderer);

    /*
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
    */

    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Renderer::update(const double&) {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
    SDL_RenderClear(_renderer);
    
    // Draw all displayables
    for (Displayable* displayable : Renderer::_displayList) {
        int drawableX = displayable->getX();
        int drawableY = displayable->getY();

        for (Displayable::DrawablePoint point : displayable->getDrawablePoints()) {
            SDL_SetRenderDrawColor(_renderer, point.color.r, point.color.g, point.color.b, point.color.a);
            SDL_RenderDrawPoint(_renderer, drawableX + point.point.x, drawableY + point.point.y);
        }

        for (Displayable::DrawableLine line : displayable->getDrawableLines()) {
            SDL_SetRenderDrawColor(_renderer, line.color.r, line.color.g, line.color.b, line.color.a);
            SDL_RenderDrawLine(_renderer, drawableX + line.line.x1, drawableY + line.line.y1, drawableX + line.line.x2, drawableY + line.line.y2);
        }

        for (Displayable::DrawableRectangle rect : displayable->getDrawableRectangles()) {
            SDL_Rect sdlRect = {drawableX + rect.rectangle.x, drawableY + rect.rectangle.y, rect.rectangle.w, rect.rectangle.h};

            SDL_SetRenderDrawColor(_renderer, rect.inColor.r, rect.inColor.g, rect.inColor.b, rect.inColor.a);
            SDL_RenderFillRect(_renderer, &sdlRect);

            SDL_SetRenderDrawColor(_renderer, rect.outColor.r, rect.outColor.g, rect.outColor.b, rect.outColor.a);
            SDL_RenderDrawRect(_renderer, &sdlRect);
        }

        for (Displayable::DisplayableTexture texture : displayable->getTextures()) {
            SDL_Rect dst = {drawableX, drawableY, texture.initWidth, texture.initHeight};
            SDL_RenderCopy(_renderer, texture.texture, NULL, &dst);
        }
    }

    SDL_RenderPresent(_renderer);
}

Displayable::DisplayableTexture Renderer::createDisplayableTexture(const std::string& p_assetName) {
    std::string image_path = Globals::ASSETS_PATH + p_assetName;
    SDL_Surface* surface = IMG_Load(image_path.c_str());
    if (!surface) {
        SDL_Log("Failed to load image at %s: %s\n", image_path.c_str(), SDL_GetError());
        return Displayable::DisplayableTexture();
    }

    Displayable::DisplayableTexture texture = Displayable::DisplayableTexture(SDL_CreateTextureFromSurface(_renderer, surface), surface->w, surface->h);
    
    SDL_FreeSurface(surface);

    return texture;
}

Displayable::DisplayableTexture Renderer::createDisplayableText(const std::string& p_content, const Types::DisplayableFont& p_font) {
	//Open the font
    std::string fontName = Globals::ASSETS_PATH + p_font.fontName;
	TTF_Font *font = TTF_OpenFont(fontName.c_str(), p_font.fontSize);
	if (!font) {
		SDL_Log("TTF_OpenFont failed");
        return Displayable::DisplayableTexture();
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surface = TTF_RenderText_Blended(font, p_content.c_str(), {255, 255, 255, 255});
    if (!surface) {
		TTF_CloseFont(font);
		SDL_Log("TTF_RenderText_Blended failed");
        return Displayable::DisplayableTexture();
    }
	
    Displayable::DisplayableTexture texture = Displayable::DisplayableTexture(SDL_CreateTextureFromSurface(_renderer, surface), surface->w, surface->h);
    
    SDL_FreeSurface(surface);
	TTF_CloseFont(font);

    return texture;
}
