#include "Login.h"
#include <string>
#include <cmath>

using namespace std;

void DrawRoundedRectangleRec(Rectangle rect, float radius, Color color)
{
    if (radius <= 0)
    {
        DrawRectangleRec(rect, color);
        return;
    }

    if (radius > rect.width / 2) radius = rect.width / 2;
    if (radius > rect.height / 2) radius = rect.height / 2;

    DrawRectangle(rect.x + radius, rect.y, rect.width - 2 * radius, rect.height, color);
    DrawRectangle(rect.x, rect.y + radius, rect.width, rect.height - 2 * radius, color);

    DrawCircle(rect.x + radius, rect.y + radius, radius, color);
    DrawCircle(rect.x + rect.width - radius, rect.y + radius, radius, color);
    DrawCircle(rect.x + radius, rect.y + rect.height - radius, radius, color);
    DrawCircle(rect.x + rect.width - radius, rect.y + rect.height - radius, radius, color);
}

void DrawRoundedRectangleLines(Rectangle rect, float radius, float thickness, Color color)
{
    if (radius <= 0)
    {
        DrawRectangleLinesEx(rect, thickness, color);
        return;
    }

    if (radius > rect.width / 2) radius = rect.width / 2;
    if (radius > rect.height / 2) radius = rect.height / 2;

    DrawLineEx({ rect.x + radius, rect.y }, { rect.x + rect.width - radius, rect.y }, thickness, color);
    DrawLineEx({ rect.x + rect.width, rect.y + radius }, { rect.x + rect.width, rect.y + rect.height - radius }, thickness, color);
    DrawLineEx({ rect.x + radius, rect.y + rect.height }, { rect.x + rect.width - radius, rect.y + rect.height }, thickness, color);
    DrawLineEx({ rect.x, rect.y + radius }, { rect.x, rect.y + rect.height - radius }, thickness, color);

    float radiusOffset = thickness / 2;
    DrawCircleLines(rect.x + radius, rect.y + radius, radius - radiusOffset, color);
    DrawCircleLines(rect.x + rect.width - radius, rect.y + radius, radius - radiusOffset, color);
    DrawCircleLines(rect.x + radius, rect.y + rect.height - radius, radius - radiusOffset, color);
    DrawCircleLines(rect.x + rect.width - radius, rect.y + rect.height - radius, radius - radiusOffset, color);
}

void Login::Init()
{
    background = LoadTexture("../../assets/login.png");
    logo = LoadTexture("../../assets/logo.png");

    username.clear();
    password.clear();

    activeField = -1;
    loginError = false;
    openSignup = false;
    showErrorPopup = false;
    errorPopupTimer = 0.0f;
    completed = false;
}

void Login::Unload()
{
    UnloadTexture(background);
    UnloadTexture(logo);
}

void Login::Update()
{
    Vector2 mouse = GetMousePosition();

    Rectangle formRect = { 200, 150, 600, 650 };  // Moved to the left
    Rectangle usernameBox = { formRect.x + 50, formRect.y + 260, 500, 40 };
    Rectangle passBox = { formRect.x + 50, formRect.y + 320, 500, 40 };
    Rectangle loginBtn = { formRect.x + 200, formRect.y + 450, 200, 50 };
    Rectangle signupLink = { formRect.x + 390, formRect.y + 380, 120, 30 };

    if (showErrorPopup)
    {
        errorPopupTimer -= GetFrameTime();
        if (errorPopupTimer <= 0)
            showErrorPopup = false;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        activeField = -1;

        if (CheckCollisionPointRec(mouse, usernameBox)) activeField = 0;
        else if (CheckCollisionPointRec(mouse, passBox)) activeField = 1;

        if (CheckCollisionPointRec(mouse, loginBtn))
        {
            if (username.empty() || password.empty())
            {
                showErrorPopup = true;
                errorPopupTimer = 2.0f;
                return;
            }
            bool success = false;

            // TODO: Add actual login validation here
            // For example: if (username == "user" && password == "pass") success = true;

            if (!success)
            {
                loginError = true;
                showErrorPopup = true;
                errorPopupTimer = 2.5f;
                completed = false;
            }
            else
            {
                loginError = false;
                completed = true;
            }
        }

        if (CheckCollisionPointRec(mouse, signupLink))
        {
            openSignup = true;
        }
    }

    int key = GetCharPressed();
    while (key > 0)
    {
        if (activeField == 0 && username.length() < 30)
            username += (char)key;
        else if (activeField == 1 && password.length() < 20)
            password += (char)key;

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (activeField == 0 && !username.empty())
            username.pop_back();
        if (activeField == 1 && !password.empty())
            password.pop_back();
    }
}

void Login::Draw()
{
    DrawTexturePro(
        background,
        { 0, 0, (float)background.width, (float)background.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 }, 0, WHITE
    );

    Rectangle formRect = { 200, 150, 600, 650 };  // Moved to the left

    // Draw rounded rectangle with transparency
    DrawRoundedRectangleRec(formRect, 20.0f, ColorAlpha(WHITE, 0.85f));
    DrawRoundedRectangleLines(formRect, 20.0f, 3, ColorAlpha(BLACK, 0.5f));

    float logoScale = 0.5f;
    float logoWidth = logo.width * logoScale;
    float logoHeight = logo.height * logoScale;

    Vector2 logoPos = {
        formRect.x + (formRect.width / 2) - (logoWidth / 2),
        formRect.y + 20
    };

    DrawTextureEx(logo, logoPos, 0.0f, logoScale, WHITE);

    DrawText("LOG IN", formRect.x + 240, formRect.y + 185, 50, BLACK);  // Adjusted position

    Rectangle usernameBox = { formRect.x + 50, formRect.y + 260, 500, 40 };
    Rectangle passBox = { formRect.x + 50, formRect.y + 320, 500, 40 };
    Rectangle loginBtn = { formRect.x + 200, formRect.y + 450, 200, 50 };
    Rectangle signupLink = { formRect.x + 390, formRect.y + 380, 120, 30 };

    DrawRectangleLinesEx(usernameBox, 2, activeField == 0 ? BLUE : BLACK);
    DrawRectangleLinesEx(passBox, 2, activeField == 1 ? BLUE : BLACK);

    if (username.empty())
        DrawText("Enter username or email", usernameBox.x + 10, usernameBox.y + 10, 20, GRAY);
    else
        DrawText(username.c_str(), usernameBox.x + 10, usernameBox.y + 10, 20, BLACK);

    if (password.empty())
        DrawText("Enter password", passBox.x + 10, passBox.y + 10, 20, GRAY);
    else
    {
        string masked(password.length(), '*');
        DrawText(masked.c_str(), passBox.x + 10, passBox.y + 10, 20, BLACK);
    }

    DrawRectangleRec(loginBtn, Color{ 16, 29, 66, 255 });
    DrawText("LOG IN", loginBtn.x + 60, loginBtn.y + 15, 20, WHITE);

    DrawText("Don't have an account?", formRect.x + 130, formRect.y + 380, 20, BLACK);

    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, signupLink);

    DrawText("Sign up",
        signupLink.x,
        signupLink.y,
        20,
        hover ? DARKBLUE : BLUE
    );

    if (hover)
        DrawLine(
            signupLink.x - 3,
            signupLink.y + 22,
            signupLink.x + 76,
            signupLink.y + 22,
            DARKBLUE
        );

    if (showErrorPopup)
    {
        float w = 400;
        float h = 120;
        float x = (GetScreenWidth() - w) / 2 - 10;
        float y = 400;

        DrawRectangle(x, y + 4, w, h, Fade(LIGHTGRAY, 0.3f));
        DrawRectangle(x, y, w, h, LIGHTGRAY);
        DrawRectangleLinesEx({ x, y, w, h }, 3, LIGHTGRAY);

        DrawText("Wrong username or password",
            x + 25, y + 50, 24, BLACK);
    }
}

bool Login::IsCompleted() const
{
    return completed;
}

bool Login::ShouldOpenSignup() const
{
    return openSignup;
}

void Login::Reset()
{
    username.clear();
    password.clear();
    activeField = -1;
    loginError = false;
    openSignup = false;
    showErrorPopup = false;
    errorPopupTimer = 0.0f;
    completed = false;
}