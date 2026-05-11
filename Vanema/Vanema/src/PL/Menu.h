#pragma once
#include "raylib.h"

class Menu
{
public:
    Menu();
    ~Menu();

    void Update();
    void Draw();

    bool IsStartPressed() const;
    bool IsExitPressed() const;

private:
    Texture2D background{};
    Texture2D logo{};
    Font gameFont;

    Rectangle startBounds{};
    Rectangle exitBounds{};

    bool startPressed = false;
    bool exitPressed = false;

    void DrawLogo(int screenWidth);
    void DrawButton(Rectangle bounds, const char* text, bool hover);
};