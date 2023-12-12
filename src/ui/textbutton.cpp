#include "textbutton.hpp"

#include "engine/controllable.hpp"
#include "engine/displayable.hpp"
#include "engine/renderer.hpp"

TextButton::TextButton(GameObject* p_parent, const std::string& p_content, std::function<void()> p_callback) {
    _parent = p_parent;

    Displayable::DisplayableTexture label = Renderer::createDisplayableText(p_content, Types::getFont(Types::Fonts::Regular));

    _display = new Displayable(this);
    _display->addRectangle(0.0f, 0.0f, label.initWidth + (MARGIN * 2.0f), label.initHeight + (MARGIN * 2.0f), Displayable::Color(255, 255, 255, 255), Displayable::Color(255, 255, 255, 64));

    _label = new Displayable(this);
    _label->addTexture(label);
    _label->setPosition(MARGIN, MARGIN);

    _control = new Controllable(this);
    _control->setHitbox(Types::Rectangle(0.0f, 0.0f, label.initWidth + (MARGIN * 2.0f), label.initHeight + (MARGIN * 2.0f)));
    _control->setOnClick(p_callback);
}

TextButton::~TextButton() {
    delete(_display);
    delete(_control);
}