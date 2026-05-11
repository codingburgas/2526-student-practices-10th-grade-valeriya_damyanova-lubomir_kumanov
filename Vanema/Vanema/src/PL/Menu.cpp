#include "Menu.h"
#include "raylib.h"
Menu::Menu()
{
    background = LoadTexture("assets/menu.png");
    logo = LoadTexture("assets/logo.png");
    gameFont = LoadFontEx("assets/fonts/PlayfairDisplay-Medium.ttf", 154, 0, 0);
}

Menu::~Menu()
{
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadFont(gameFont);
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
        GRAY
    );

    DrawLogo(screenWidth);

    const char* title = "VANEMA";
    const char* slogan = "The Standar of Cinematic Experience";
    int fontSize = 154;
    Vector2 textSize = MeasureTextEx(gameFont, title, fontSize, 1);

    DrawTextEx(
        gameFont,
        title,
        {
            (screenWidth - textSize.x) / 2.05f,
            screenHeight * 0.35f
        },
        fontSize,
        1,
        BLACK
    );

    DrawTextEx(
        gameFont,
        slogan,
        {
            (screenWidth - textSize.x) / 2.30f,
            screenHeight * 0.50f
        },
        fontSize = 45,
        1,
        BLACK
    );

    float buttonWidth = 250;
    float buttonHeight = 60;

    float x = (screenWidth - buttonWidth) / 2.05f;
    float y = screenHeight * 0.64f;
    float spacing = 30.0f;

    startBounds = { x, y, buttonWidth, buttonHeight };
    exitBounds = { x, y + buttonHeight + spacing, buttonWidth, buttonHeight };

    Vector2 mouse = GetMousePosition();

    bool hoverStart = CheckCollisionPointRec(mouse, startBounds);
    bool hoverExit = CheckCollisionPointRec(mouse, exitBounds);

    DrawButton(startBounds, "BOOK", hoverStart);
    DrawButton(exitBounds, "EXIT", hoverExit);
}

void Menu::DrawLogo(int screenWidth)
{
    float logoX = (screenWidth - 350) / 2;

    Rectangle source = { 0, 0, (float)logo.width, (float)logo.height };
    Rectangle dest = { 0, 0, 320, 320 };
    Vector2 origin = { -logoX, -30 };

    DrawTexturePro(logo, source, dest, origin, 0, WHITE);
}

void Menu::DrawButton(Rectangle bounds, const char* text, bool hover)
{
    Color base = hover ? Color{ 208, 213, 233, 255 } : Color{ 208, 213, 233, 255 };
    Color border = hover ? Color{ 208, 213, 233, 255 } : Color{ 208, 213, 233, 255 };

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

    int fontSize = 40;
    Vector2 textSize = MeasureTextEx(gameFont, text, fontSize, 1);

    DrawTextEx(
        gameFont, 
        text,
        {
            bounds.x + (bounds.width - textSize.x) / 2,
            bounds.y + (bounds.height - fontSize) / 2
        },
        fontSize,
        1,
        BLACK
    );
}

bool Menu::IsStartPressed() const { return startPressed; }
bool Menu::IsExitPressed() const { return exitPressed; }