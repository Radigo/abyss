#include "closebutton.hpp"

#include "engine/displayable.hpp"

CloseButton::CloseButton() {
    _display = new Displayable();
    _display->addRectangle(-CLOSE_BUTTON_SIZE * 0.5f, -CLOSE_BUTTON_SIZE * 0.5f, CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE, Displayable::Color(255, 255, 255, 255), Displayable::Color(255, 255, 255, 64));
    _display->addLine(-CLOSE_BUTTON_SIZE * 0.4f, -CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f);
    _display->addLine(-CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, -CLOSE_BUTTON_SIZE * 0.4f);
}

CloseButton::~CloseButton() {

}