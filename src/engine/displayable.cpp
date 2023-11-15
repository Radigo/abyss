#include "displayable.hpp"

#include "engine/renderer.hpp"

Displayable::Displayable() :
_x(0),
_y(0),
_drawablePoints(),
_drawableLines(),
_drawableRectangles()
{
    Renderer::add(this);
    /*
    OK, let's get rid of renderer in here.
    Try this:
    keep a static list of displayables in displayable
    Let renderer get access to this list when it updates
    */
}

Displayable::~Displayable() {

}

void Displayable::setPosition(const int p_x, const int p_y) {
    _x = p_x;
    _y = p_y;
}

void Displayable::clear() {
    _drawablePoints.clear();
    _drawableLines.clear();
    _drawableRectangles.clear();
}

void Displayable::addPoint(const int p_x, const int p_y, const Color p_color) {
    _drawablePoints.emplace_back(Point(p_x, p_y, p_color));
}

void Displayable::addLine(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color) {
    _drawableLines.emplace_back(Line(p_x1, p_y1, p_x2, p_y2, p_color));
}

void Displayable::addRectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor, const Color p_inColor) {
    _drawableRectangles.emplace_back(Rectangle(p_x, p_y, p_w, p_h, p_outColor, p_inColor));
}