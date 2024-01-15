#include "controltester.hpp"

#include <sstream>

#include "scene.hpp"
#include "engine/controllable.hpp"
#include "engine/displayable.hpp"
#include "engine/updatable.hpp"
#include "engine/renderer.hpp"

ControlTester::ControlTester(GameObject* p_parent) :
GameObject(p_parent)
{
    _id = "ControlTester";
    _debugTf = new Displayable(this);
    _tfIndex = _debugTf->addTexture(Renderer::createDisplayableText("0000000000", Types::getFont(Types::Fonts::Regular)));
    
    _inputReader = new Controllable(this);
    _updater = new Updatable(this, [this](const double& p_deltaTime){ _updateDebugTf(p_deltaTime); });
}

ControlTester::~ControlTester() {

}


void ControlTester::_updateDebugTf(const double& p_deltaTime) {
    // Put TF in bottom left corner
    _debugTf->setPosition(0, Scene::sceneHeight - 20);

    int moveState = _inputReader->getMoveState();
    int btnState = _inputReader->getButtonState();

    // For the record, each line size is 36 x 8 (in case we want to wrap to get multiline)

    std::string flags = "";
    flags += " - UP:   ";
    flags += (moveState & Controllable::UP) ? "1" : "0";
    flags += " - DOWN: ";
    flags += (moveState & Controllable::DOWN) ? "1" : "0";
    flags += " - LEFT: ";
    flags += (moveState & Controllable::LEFT) ? "1" : "0";
    flags += " - RIGHT: ";
    flags += (moveState & Controllable::RIGHT) ? "1" : "0";
    flags += " - A:     ";
    flags += (btnState & Controllable::BTN_A) ? "1" : "0";
    flags += " - B:     ";
    flags += (btnState & Controllable::BTN_B) ? "1" : "0";
    flags += " - C:     ";
    flags += (btnState & Controllable::BTN_C) ? "1" : "0";
    flags += " - START: ";
    flags += (btnState & Controllable::BTN_START) ? "1" : "0";

    _debugTf->updateTexture(_tfIndex, Renderer::createDisplayableText(flags, Types::getFont(Types::Fonts::Regular)));
}