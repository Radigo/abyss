#include "window.hpp"

#include "engine/controllable.hpp"
#include "engine/displayable.hpp"
#include "engine/renderer.hpp"

#include <SDL2/SDL_log.h>

Window::Window(std::string p_id, const int p_width, const int p_height, std::function<void()> p_onCloseCallback) :
GameObject(nullptr)
{
    _id = p_id;
    
    _display = new Displayable(this);
    _display->addRectangle(0, 0, p_width, p_height, Displayable::Color(255, 255, 255, 255), Displayable::Color(0, 0, 0, 0));
    _display->addRectangle(1, 1, p_width - 2, TOP_BAR_SIZE, Displayable::Color(0, 0, 0, 0), Displayable::Color(64, 64, 64, 255));
    _display->addRectangle(1, TOP_BAR_SIZE + 1, p_width - 2, p_height - TOP_BAR_SIZE + 2, Displayable::Color(0, 0, 0, 0), Displayable::Color(16, 16, 16, 196));

    // Font test
    Displayable* text = new Displayable(this);
    text->addTexture(Renderer::createDisplayableText(_id, Types::getFont(Types::Fonts::RegularDouble)));
    int textOffset = (TOP_BAR_SIZE - text->getTextures().at(0).initHeight) * 0.5f;
    text->setPosition(textOffset, textOffset);

    _closeButton = new CloseButton(this, CLOSE_BUTTON_SIZE, p_onCloseCallback);
    _closeButton->setPosition(p_width - (TOP_BAR_SIZE * 0.5f), (TOP_BAR_SIZE * 0.5f));

    _dragArea = new Controllable(this);
    _dragArea->setHitbox(Types::Rectangle(0, 0, (p_width - TOP_BAR_SIZE), TOP_BAR_SIZE));
    _dragArea->setOnMouseButtonDown([this](Types::Point p_position){
        _dragOrigin = p_position - Types::Point(getX(), getY());
    });
    _dragArea->setOnDragMove([this](Types::Point p_position){
        setPosition(p_position.x - _dragOrigin.x, p_position.y - _dragOrigin.y);
    });
}

Window::~Window() {
}