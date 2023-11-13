#include "closebutton.hpp"

CloseButton::CloseButton() {
    addRectangle(-CLOSE_BUTTON_SIZE * 0.5f, -CLOSE_BUTTON_SIZE * 0.5f, CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE, Color(255, 255, 255, 255), Color(255, 255, 255, 64));
    addLine(-CLOSE_BUTTON_SIZE * 0.4f, -CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f);
    addLine(-CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, CLOSE_BUTTON_SIZE * 0.4f, -CLOSE_BUTTON_SIZE * 0.4f);
}

CloseButton::~CloseButton() {

}