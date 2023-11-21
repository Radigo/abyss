#include "closebutton.hpp"

#include "engine/controllable.hpp"
#include "engine/displayable.hpp"

CloseButton::CloseButton(GameObject* p_parent, const int p_size, std::function<void()> p_callback) {
    _parent = p_parent;

    _display = new Displayable(this);
    _display->addRectangle(-p_size * 0.5f, -p_size * 0.5f, p_size, p_size, Displayable::Color(255, 255, 255, 255), Displayable::Color(255, 255, 255, 64));
    _display->addLine(-p_size * 0.4f, -p_size * 0.4f, p_size * 0.4f, p_size * 0.4f);
    _display->addLine(-p_size * 0.4f, p_size * 0.4f, p_size * 0.4f, -p_size * 0.4f);

    _control = new Controllable(this);
    _control->setHitbox(Types::Rectangle(-p_size * 0.5f, -p_size * 0.5f, p_size, p_size));
    _control->setOnClick(p_callback);
}

CloseButton::~CloseButton() {
    delete(_display);
    delete(_control);
}