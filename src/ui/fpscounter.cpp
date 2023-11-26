#include "fpscounter.hpp"

#include <sstream>

#include "engine/displayable.hpp"
#include "engine/updatable.hpp"
#include "engine/renderer.hpp"

FpsCounter::FpsCounter(GameObject* p_parent) {
    _parent = p_parent;

    _display = new Displayable(this);
    _display->addRectangle(0, 0, 100, 20, Displayable::Color(0, 0, 0, 0), Displayable::Color(0, 0, 0, 64));

    _digits = new Displayable(this);
    _tfIndex = _digits->addTexture(Renderer::createDisplayableText("0000000000", Types::getFont(Types::Fonts::Regular)));
    
    _updater = new Updatable(this, [this](const double& p_frameDeltaTime, const double& p_updateDeltaTime){ _updateCounter(p_frameDeltaTime, p_updateDeltaTime); });
}

FpsCounter::~FpsCounter() {
    delete(_display);
}

void FpsCounter::_updateCounter(const double& p_frameDeltaTime, const double&) {
    std::string strFrameDelta = std::to_string(p_frameDeltaTime);
    _digits->updateTexture(_tfIndex, Renderer::createDisplayableText(strFrameDelta, Types::getFont(Types::Fonts::Regular)));
}