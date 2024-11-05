#pragma once
#include <raylib.h>
class button{
    private:
        Texture2D texture;
        Vector2 position;
    public:
        button(const char* imagepath, Vector2 imposition);
        ~button();
        void draw();
        bool isPressed(Vector2 mousePos, bool mausepressed);
};