#include "displayable.hpp"

#include <algorithm>

#include "engine/globals.hpp"
#include "engine/renderer.hpp"
#include "engine/types.hpp"

#include <SDL2/SDL_log.h>

Displayable::Displayable(GameObject* p_parent) :
_drawablePoints(),
_drawableLines(),
_drawableRectangles()
{
    size_t numDisplayables = Renderer::addDisplayable(this);
    _id = "Displayable" + to_string(numDisplayables);
    _parent = p_parent;
}

Displayable::~Displayable() {
    _parent = nullptr;
    clear();
    Renderer::removeDisplayable(this);
}

void Displayable::clear() {
    _drawablePoints.clear();
    _drawableLines.clear();
    _drawableRectangles.clear();
    _textures.clear();
}

size_t Displayable::addPoint(const int p_x, const int p_y, const Color p_color) {
    _drawablePoints.push_back(DrawablePoint(Types::Point(p_x, p_y), p_color));
    return _drawablePoints.size() - 1;
}

size_t Displayable::addLine(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color) {
    _drawableLines.push_back(DrawableLine(Types::Line(p_x1, p_y1, p_x2, p_y2), p_color));
    return _drawableLines.size() - 1;
}

size_t Displayable::addRectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor, const Color p_inColor) {
    _drawableRectangles.push_back(DrawableRectangle(Types::Rectangle(p_x, p_y, p_w, p_h), p_outColor, p_inColor));
    return _drawableRectangles.size() - 1;
}

size_t Displayable::addTexture(SDL_Texture* p_texture, const int p_initWidth, const int p_initHeight) {
    _textures.push_back(DisplayableTexture(p_texture, p_initWidth, p_initHeight));
    return _textures.size() - 1;
}

size_t Displayable::addTexture(DisplayableTexture p_displayableTexture) {
    return addTexture(p_displayableTexture.texture, p_displayableTexture.initWidth, p_displayableTexture.initHeight);
}

void Displayable::updateTexture(size_t p_index, SDL_Texture* p_texture) {
    _textures.at(p_index).texture = p_texture;
}

void Displayable::updateTexture(size_t p_index, DisplayableTexture p_displayableTexture) {
    _textures.at(p_index).texture = p_displayableTexture.texture;
    _textures.at(p_index).initWidth = p_displayableTexture.initWidth;
    _textures.at(p_index).initHeight = p_displayableTexture.initHeight;
}