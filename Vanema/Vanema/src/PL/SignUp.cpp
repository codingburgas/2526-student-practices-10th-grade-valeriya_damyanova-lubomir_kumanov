#include "SignUp.h"
#include <string>

using namespace std;

void SignUp::Init() {
    background = LoadTexture("assets/login.png");
    logo = LoadTexture("assets/logo.png");
    headerFont = LoadFontEx("assets/fonts/Roboto-Bold.ttf", 60, 0, 0);
    bodyFont = LoadFontEx("assets/fonts/Roboto-Medium.ttf", 32, 0, 0);

    name.clear();
    email.clear();
    username.clear();
    password.clear();

    activeField = -1;
}

void SignUp::Unload() {
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadFont(headerFont);
    UnloadFont(bodyFont);
}

void SignUp::Update() {
    Vector2 mouse = GetMousePosition();

    Rectangle formRect = { 500, 100, 570, 720 };

    Rectangle usernameBox = { formRect.x + 35, formRect.y + 260, formRect.width - 70, 40 };
    Rectangle emailBox = { formRect.x + 35, formRect.y + 330, formRect.width - 70, 40 };
    Rectangle passBox = { formRect.x + 35, formRect.y + 400, formRect.width - 70, 40 };
    Rectangle nameBox = { formRect.x + 35, formRect.y + 470, formRect.width - 70, 40 };

    Rectangle signupBtn = { formRect.x + (formRect.width - 200) / 2, formRect.y + 560, 200, 50 };

    std::string textPrompt = "Already have an account? ";
    Vector2 promptSize = MeasureTextEx(bodyFont, textPrompt.c_str(), 28, 2);
    float totalBlockWidth = promptSize.x + 70.0f;
    float blockStartX = formRect.x + (formRect.width - totalBlockWidth) / 2.0f;

    Rectangle backLink = { blockStartX + promptSize.x, formRect.y + 640, 75, 30 };

    if (CheckCollisionPointRec(mouse, usernameBox) ||
        CheckCollisionPointRec(mouse, emailBox) ||
        CheckCollisionPointRec(mouse, passBox) ||
        CheckCollisionPointRec(mouse, nameBox) ||
        CheckCollisionPointRec(mouse, signupBtn) ||
        CheckCollisionPointRec(mouse, backLink))
    {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        activeField = -1;

        if (CheckCollisionPointRec(mouse, usernameBox))      activeField = 0;
        else if (CheckCollisionPointRec(mouse, emailBox))    activeField = 1;
        else if (CheckCollisionPointRec(mouse, passBox))     activeField = 2;
        else if (CheckCollisionPointRec(mouse, nameBox))     activeField = 3;

        if (CheckCollisionPointRec(mouse, signupBtn))
        {
            if (name.empty() || email.empty() || username.empty() || password.empty())
            {
                return;
            }

            if (password.length() < 6)
            {
                return;
            }

            std::string serviceError;
            if (userService.registerUser(username, email, password, name, serviceError))
            {
                if (currentScreen != nullptr) *currentScreen = 1;
            }
        }

        if (CheckCollisionPointRec(mouse, backLink))
        {
            if (currentScreen != nullptr)
            {
                *currentScreen = 1;
            }
        }
    }

    int key = GetCharPressed();
    while (key > 0)
    {
        if (activeField == 0 && username.length() < 20)
            username += (char)key;
        else if (activeField == 1 && email.length() < 50)
            email += (char)key;
        else if (activeField == 2 && password.length() < 20)
            password += (char)key;
        else if (activeField == 3 && name.length() < 30)
            name += (char)key;

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (activeField == 0 && !username.empty()) username.pop_back();
        else if (activeField == 1 && !email.empty()) email.pop_back();
        else if (activeField == 2 && !password.empty()) password.pop_back();
        else if (activeField == 3 && !name.empty()) name.pop_back();
    }
}

void SignUp::Draw() {
    DrawTexturePro(
        background,
        { 0, 0, (float)background.width, (float)background.height },
        { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
        { 0, 0 }, 0, GRAY
    );

    Rectangle formRect = { 500, 100, 570, 720 };

    Rectangle nameBox = { formRect.x + 35, formRect.y + 470, formRect.width - 70, 40 };
    Rectangle emailBox = { formRect.x + 35, formRect.y + 330, formRect.width - 70, 40 };
    Rectangle usernameBox = { formRect.x + 35, formRect.y + 260, formRect.width - 70, 40 };
    Rectangle passBox = { formRect.x + 35, formRect.y + 400, formRect.width - 70, 40 };
    Rectangle signupBtn = { formRect.x + (formRect.width - 200) / 2, formRect.y + 560, 200, 50 };

    for (int i = 8; i > 0; i--)
    {
        DrawRectangle(formRect.x + i, formRect.y + i, formRect.width, formRect.height, Fade(BLACK, 0.03f));
    }

    for (int i = 20; i > 0; i--)
    {
        DrawRectangleRounded(
            { formRect.x + i * 0.5f, formRect.y + i * 0.5f, formRect.width, formRect.height },
            0.08f, 10, Fade(GRAY, 0.02f)
        );
    }

    DrawRectangleRounded(formRect, 0.08f, 20, Fade(WHITE, 0.82f));
    DrawRectangleRoundedLines(formRect, 0.08f, 20, 2, Fade(WHITE, 0.4f));

    float logoScale = 0.35f;
    float logoWidth = logo.width * logoScale;
    Vector2 logoPos = { formRect.x + (formRect.width / 2) - (logoWidth / 2), formRect.y + 10 };

    DrawTextureEx(logo, logoPos, 0.0f, logoScale, WHITE);

    DrawTextEx(headerFont, "SIGN UP", { formRect.x + 195, formRect.y + 140 }, 50, 3, BLACK);

    DrawRectangleRounded(nameBox, 0.2f, 10, WHITE);
    DrawRectangleRounded(emailBox, 0.2f, 10, WHITE);
    DrawRectangleRounded(usernameBox, 0.2f, 10, WHITE);
    DrawRectangleRounded(passBox, 0.2f, 10, WHITE);

    DrawRectangleRoundedLines(usernameBox, 0.2f, 10, 2, activeField == 0 ? BLUE : GRAY);
    DrawRectangleRoundedLines(emailBox, 0.2f, 10, 2, activeField == 1 ? BLUE : GRAY);
    DrawRectangleRoundedLines(passBox, 0.2f, 10, 2, activeField == 2 ? BLUE : GRAY);
    DrawRectangleRoundedLines(nameBox, 0.2f, 10, 2, activeField == 3 ? BLUE : GRAY);

    if (name.empty()) DrawTextEx(bodyFont, "Name", { nameBox.x + 10, nameBox.y + 5 }, 30, 3, GRAY);
    else DrawText(name.c_str(), nameBox.x + 10, nameBox.y + 10, 20, BLACK);

    if (username.empty()) DrawTextEx(bodyFont, "Username", { usernameBox.x + 10, usernameBox.y + 5 }, 30, 3, GRAY);
    else DrawText(username.c_str(), usernameBox.x + 10, usernameBox.y + 10, 20, BLACK);

    if (email.empty()) DrawTextEx(bodyFont, "Email", { emailBox.x + 10, emailBox.y + 5 }, 30, 3, GRAY);
    else DrawText(email.c_str(), emailBox.x + 10, emailBox.y + 10, 20, BLACK);

    if (password.empty()) DrawTextEx(bodyFont, "Password", { passBox.x + 10, passBox.y + 5 }, 30, 3, GRAY);
    else {
        string masked(password.length(), '*');
        DrawText(masked.c_str(), passBox.x + 10, passBox.y + 10, 20, BLACK);
    }

    Vector2 mouse = GetMousePosition();
    bool hoverSignup = CheckCollisionPointRec(mouse, signupBtn);
    Color buttonColor = { 164, 192, 213, 255 };

    if (hoverSignup)
    {
        DrawRectangle(signupBtn.x + 4, signupBtn.y + 4, signupBtn.width, signupBtn.height, Fade(BLACK, 0.35f));
    }

    DrawRectangleRounded(signupBtn, 0.3f, 10, buttonColor);
    DrawRectangleRoundedLines(signupBtn, 0.3f, 10, 2, buttonColor);
    DrawTextEx(headerFont, "Sign Up", { signupBtn.x + 45, signupBtn.y + 7 }, 35, 2, BLACK);

    std::string textPrompt = "Already have an account? ";
    Vector2 promptSize = MeasureTextEx(bodyFont, textPrompt.c_str(), 28, 2);
    float totalBlockWidth = promptSize.x + 70.0f;
    float blockStartX = formRect.x + (formRect.width - totalBlockWidth) / 2.0f;

    Rectangle backLink = { blockStartX + promptSize.x, formRect.y + 640, 75, 30 };
    bool hoverBack = CheckCollisionPointRec(mouse, backLink);

    DrawTextEx(bodyFont, textPrompt.c_str(), { blockStartX, formRect.y + 640 }, 28, 2, BLACK);
    DrawTextEx(bodyFont, "Login", { backLink.x, backLink.y }, 28, 2, hoverBack ? DARKBLUE : BLUE);

    if (hoverBack)
    {
        DrawLine(backLink.x, backLink.y + 25, backLink.x + 68, backLink.y + 25, DARKBLUE);
    }
}