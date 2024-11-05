#include "button.hpp"

button::button(const char *imagepath, Vector2 imposition)
{
    texture = LoadTexture(imagepath);
    position=imposition;
}

button::~button()
{
    UnloadTexture(texture);
}

void button::draw()
{
    DrawTextureV(texture,position,WHITE);
}

bool button::isPressed(Vector2 mousePos, bool mausepressed)
{
    Rectangle rec= {position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    if(CheckCollisionPointRec(mousePos, rec) && mausepressed){
        return true;
    }
    return false;
}

