#include "drawable.hpp"

#include <SDL2/SDL_log.h>

#include "renderer.hpp"

Drawable::Drawable() :
_drawablePoints(),
_drawableLines(),
_drawableRectangles()
{
    SDL_Log("<Draw constructor>");
    Renderer::addDraw(this);
}

Drawable::~Drawable() {
    SDL_Log("<Draw destructor>");
}

void Drawable::setPosition(const int p_x, const int p_y) {
    _x = p_x;
    _y = p_y;
}

void Drawable::clear() {
    _drawablePoints.clear();
    _drawableLines.clear();
    _drawableRectangles.clear();
}

void Drawable::addPoint(const int p_x, const int p_y, const Color p_color) {
    _drawablePoints.emplace_back(Point(p_x, p_y, p_color));
}

void Drawable::addLine(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color) {
    _drawableLines.emplace_back(Line(p_x1, p_x2, p_y1, p_y2, p_color));
}

void Drawable::addRectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor, const Color p_inColor) {
    _drawableRectangles.emplace_back(Rectangle(p_x, p_y, p_w, p_h, p_outColor, p_inColor));
}