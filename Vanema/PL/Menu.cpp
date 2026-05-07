#include "Menu.h"
#include "raylib.h"
Menu::Menu()
{
    background = LoadTexture("../../assets/menu_background.png");
}

Menu::~Menu()
{
    UnloadTexture(background);
}

void Menu::Update()
{
    startPressed = false;
    exitPressed = false;

    Vector2 mouse = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mouse, startBounds))
            startPressed = true;

        if (CheckCollisionPointRec(mouse, exitBounds))
            exitPressed = true;
    }
}

void Menu::Draw()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    DrawTexturePro(
        background,
        { 0, 0, (float)background.width, (float)background.height },
        { 0, 0, (float)screenWidth, (float)screenHeight },
        { 0, 0 },
        0.0f,
        WHITE
    );

    const char* title = "VANEMA";
    int fontSize = 154;
    int textWidth = MeasureText(title, fontSize);

    DrawText(
        title,
        (screenWidth - textWidth) / 1.30,
        screenHeight * 0.25f,
        fontSize,
        BLACK
    );

    float buttonWidth = 250;
    float buttonHeight = 60;

    float x = (screenWidth - buttonWidth) / 1.45f;
    float y = screenHeight * 0.44f;
    float spacing = 30.0f;

    startBounds = { x, y, buttonWidth, buttonHeight };
    exitBounds = { x, y + buttonHeight + spacing, buttonWidth, buttonHeight };

    Vector2 mouse = GetMousePosition();

    bool hoverStart = CheckCollisionPointRec(mouse, startBounds);
    bool hoverExit = CheckCollisionPointRec(mouse, exitBounds);

    DrawButton(startBounds, "START", hoverStart);
    DrawButton(exitBounds, "EXIT", hoverExit);
}

void Menu::DrawButton(Rectangle bounds, const char* text, bool hover)
{
    Color base = hover ? Color{ 70, 130, 180, 255 } : Color{ 50, 50, 70, 255 };
    Color border = hover ? SKYBLUE : DARKGRAY;

    if (hover)
    {
        DrawRectangle(
            bounds.x + 4,
            bounds.y + 4,
            bounds.width,
            bounds.height,
            Fade(BLACK, 0.35f)
        );
    }

    DrawRectangleRec(bounds, base);
    DrawRectangleLinesEx(bounds, 2, border);

    int fontSize = 24;
    int textWidth = MeasureText(text, fontSize);

    DrawText(
        text,
        bounds.x + (bounds.width - textWidth) / 2,
        bounds.y + (bounds.height - fontSize) / 2,
        fontSize,
        RAYWHITE
    );
}

bool Menu::IsStartPressed() const { return startPressed; }
bool Menu::IsExitPressed() const { return exitPressed; }