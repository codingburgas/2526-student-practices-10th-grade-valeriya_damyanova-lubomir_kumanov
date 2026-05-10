#include "Login.h"
#include <string>

using namespace std;

void Login::Init()
{
    background = LoadTexture("../../assets/login.png");
    logo = LoadTexture("../../assets/logo.png");
    headerFont = LoadFontEx("../../assets/fonts/PlayfairDisplay-Bold.ttf", 60, 0, 0);
    bodyFont = LoadFontEx("../../assets/fonts/PlayfairDisplay-Medium.ttf", 32, 0, 0);

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
    UnloadFont(headerFont);
    UnloadFont(bodyFont);
}

void Login::Update()
{
    Vector2 mouse = GetMousePosition();

    Rectangle formRect = { 500, 180, 800, 600 };

    Rectangle usernameBox = { formRect.x + 50, formRect.y + 260, 700, 40 };
    Rectangle passBox = { formRect.x + 50, formRect.y + 320, 700, 40 };
    Rectangle loginBtn = { formRect.x + 200, formRect.y + 450, 200, 50 };
    Rectangle signupLink = { formRect.x + 175, formRect.y + 175, 120, 30 };

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
        { 0, 0 }, 0, GRAY
    );

    Rectangle formRect = { 500, 180, 570, 650 };

    Rectangle usernameBox = {
        formRect.x + 35,
        formRect.y + 290,
        formRect.width - 70,
        40
    };

    Rectangle passBox = {
        formRect.x + 35,
        formRect.y + 370,
        formRect.width - 70,
        40
    };

    Rectangle loginBtn = {
        formRect.x + (formRect.width - 200) / 2,
        formRect.y + 460,
        200,
        50
    };

    Rectangle signupLink = {
        formRect.x + 200,
        formRect.y + 380,
        120,
        30
    };

    for (int i = 8; i > 0; i--)
    {
        DrawRectangle(
            formRect.x + i,
            formRect.y + i,
            formRect.width,
            formRect.height,
            Fade(BLACK, 0.03f)
        );
    }

    for (int i = 20; i > 0; i--)
    {
        DrawRectangleRounded(
            {
                formRect.x + i * 0.5f,
                formRect.y + i * 0.5f,
                formRect.width,
                formRect.height
            },
            0.08f,
            10,
            Fade(GRAY, 0.02f)
        );
    }

    DrawRectangleRounded(
        formRect,
        0.08f,
        20,
        Fade(WHITE, 0.82f)
    );

    DrawRectangleRoundedLines(
        formRect,
        0.08f,
        20,
        2,
        Fade(WHITE, 0.4f)
    );
    
    float logoScale = 0.4f;
    float logoWidth = logo.width * logoScale;

    Vector2 logoPos = {
        formRect.x + (formRect.width / 2) - (logoWidth / 2),
        formRect.y
    };

    DrawTextureEx(logo, logoPos, 0.0f, logoScale, WHITE);

    DrawTextEx(
        headerFont,
        "LOGIN",
        {formRect.x + 210, formRect.y + 185}, 
        55, 
        3,
        BLACK
    );

    DrawRectangleRounded(usernameBox, 0.2f, 10, WHITE);
    DrawRectangleRounded(passBox, 0.2f, 10, WHITE);

    DrawRectangleRoundedLines(usernameBox, 0.2f, 10, 2, activeField == 0 ? BLUE : GRAY);
    DrawRectangleRoundedLines(passBox, 0.2f, 10, 2, activeField == 1 ? BLUE : GRAY);

    if (username.empty())
        DrawTextEx(
            bodyFont, 
            "Username", 
            { usernameBox.x + 10, usernameBox.y + 5 }, 
            30,
            3,
            GRAY
        );
    else
        DrawText(username.c_str(), usernameBox.x + 10, usernameBox.y + 10, 20, BLACK);

    if (password.empty())
        DrawTextEx(
            bodyFont,
            "Password",
            { passBox.x + 10, passBox.y + 5 },
            30,
            3,
            GRAY
        );
    else
    {
        string masked(password.length(), '*');
        DrawText(masked.c_str(), passBox.x + 10, passBox.y + 10, 20, BLACK);
    }

    Vector2 mouse = GetMousePosition();
    bool hoverLogin = CheckCollisionPointRec(mouse, loginBtn);

    Color buttonColor = { 164, 192, 213, 255 };

    if (hoverLogin)
    {
        DrawRectangle(
            loginBtn.x + 4,
            loginBtn.y + 4,
            loginBtn.width,
            loginBtn.height,
            Fade(BLACK, 0.35f)
        );
    }

    DrawRectangleRounded(loginBtn, 0.3f, 10, buttonColor);

    DrawRectangleRoundedLines(
        loginBtn,
        0.3f,
        10,
        2,
        buttonColor
    );

    DrawTextEx(
        headerFont,
        "Login",
        { loginBtn.x + 60, loginBtn.y + 10 },
        35,
        2,
        BLACK
    );

    DrawTextEx(
        bodyFont, 
        "Don't have an account?", 
        { formRect.x + 100, formRect.y + 555 }, 
        30, 
        2,
        BLACK
    );

    bool hover = CheckCollisionPointRec(mouse, signupLink);

    DrawTextEx(
        bodyFont,
        "Sign up",
        { signupLink.x + 175,
        signupLink.y + 175 },
        30,
        2,
        hover ? DARKBLUE : BLUE
    );

    if (hover)
    {
        DrawLine(
            signupLink.x - 3,
            signupLink.y + 22,
            signupLink.x + 76,
            signupLink.y + 22,
            DARKBLUE
        );
    }

    if (showErrorPopup)
    {
        float w = 400;
        float h = 120;
        float x = (GetScreenWidth() - w) / 2 - 10;
        float y = 400;

        DrawRectangle(x + 4, y + 4, w, h, Fade(BLACK, 0.2f));
        DrawRectangle(x, y, w, h, LIGHTGRAY);
        DrawRectangleLinesEx({ x, y, w, h }, 3, LIGHTGRAY);

        DrawText(
            "Wrong username or password",
            x + 25, y + 50,
            24,
            BLACK
        );
    }
}