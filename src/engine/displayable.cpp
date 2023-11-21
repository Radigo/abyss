#include "displayable.hpp"

#include <algorithm>

#include "engine/globals.hpp"
#include "engine/types.hpp"

#include <SDL2/SDL_log.h>

std::vector<Displayable*> Displayable::displayList;

Displayable::Displayable(GameObject* p_parent) :
_drawablePoints(),
_drawableLines(),
_drawableRectangles()
{
    Displayable::displayList.emplace_back(this);
    _id = "Displayable" + to_string(Displayable::displayList.size());
    _parent = p_parent;
}

Displayable::~Displayable() {
    _parent = nullptr;
    clear();
    auto it = std::find(Displayable::displayList.begin(), Displayable::displayList.end(), this);
    if (it == Displayable::displayList.end())
        static_assert(true, "removing Displayable that is NOT in Displayable::displayList");
    Displayable::displayList.erase(it);
}

void Displayable::clear() {
    _drawablePoints.clear();
    _drawableLines.clear();
    _drawableRectangles.clear();
}

void Displayable::addPoint(const int p_x, const int p_y, const Color p_color) {
    _drawablePoints.emplace_back(DrawablePoint(Types::Point(p_x, p_y), p_color));
}

void Displayable::addLine(const int p_x1, const int p_y1, const int p_x2, const int p_y2, const Color p_color) {
    _drawableLines.emplace_back(DrawableLine(Types::Line(p_x1, p_y1, p_x2, p_y2), p_color));
}

void Displayable::addRectangle(const int p_x, const int p_y, const int p_w, const int p_h, const Color p_outColor, const Color p_inColor) {
    _drawableRectangles.emplace_back(DrawableRectangle(Types::Rectangle(p_x, p_y, p_w, p_h), p_outColor, p_inColor));
}

void Displayable::addTexture(SDL_Texture* p_texture, const int p_initWidth, const int p_initHeight) {
    _textures.push_back(DisplayableTexture(p_texture, p_initWidth, p_initHeight));
}

void Displayable::addTexture(DisplayableTexture p_displayableTexture) {
    addTexture(p_displayableTexture.texture, p_displayableTexture.initWidth, p_displayableTexture.initHeight);
}