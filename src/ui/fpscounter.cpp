#include "fpscounter.hpp"

#include <sstream>
#include <iostream>

#include "engine/displayable.hpp"
#include "engine/updatable.hpp"
#include "engine/renderer.hpp"

FpsCounter::FpsCounter(GameObject* p_parent) :
GameObject(p_parent)
{
    _id = "fpsCounter";

    _display = new Displayable(this);
    _display->addRectangle(0, 0, 100, 20, Displayable::Color(0, 0, 0, 0), Displayable::Color(0, 0, 0, 64));

    _digits = new Displayable(this);
    _tfIndex = _digits->addTexture(Renderer::createDisplayableText("0000000000", Types::getFont(Types::Fonts::Regular)));
    
    _updater = new Updatable(this, [this](const double& p_deltaTime){ _updateCounter(p_deltaTime); });
}

FpsCounter::~FpsCounter() {
}

void FpsCounter::_updateCounter(const double& p_deltaTime) {
    std::string strFrameDelta = std::to_string(p_deltaTime);
    _digits->updateTexture(_tfIndex, Renderer::createDisplayableText(strFrameDelta, Types::getFont(Types::Fonts::Regular)));
}