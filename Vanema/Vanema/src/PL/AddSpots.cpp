#include "AddSpots.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>

void DrawTextBoxed(Font font, const char* text, Rectangle box, float fontSize, float spacing, bool wordWrap, Color tint) {
    int length = strlen(text);
    float textHeight = fontSize;
    float textWidth = 0;

    float lineOffsetY = 0;
    float lineOffsetX = 0;

    float scaleFactor = fontSize / (float)font.baseSize;

    int startLine = 0;
    int endLine = -1;
    int lastk = -1;

    for (int i = 0; i < length; i++) {
        int byteLeft = 0;
        int codepoint = GetCodepoint(&text[i], &byteLeft);
        if (byteLeft == 0) byteLeft = 1;

        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == '\n') {
            int glyphWidth = 0;
            if (font.glyphs[index].advanceX == 0) glyphWidth = font.recs[index].width;
            else glyphWidth = font.glyphs[index].advanceX;

            DrawTextCodepoint(font, codepoint, { box.x + lineOffsetX, box.y + lineOffsetY }, fontSize, tint);
            lineOffsetX = 0;
            lineOffsetY += (textHeight + 4);
            startLine = i + byteLeft;
            endLine = -1;
            lastk = -1;
            i += (byteLeft - 1);
            continue;
        }

        if (codepoint == ' ') { lastk = i; }

        int glyphWidth = 0;
        if (font.glyphs[index].advanceX == 0) glyphWidth = font.recs[index].width;
        else glyphWidth = font.glyphs[index].advanceX;

        textWidth = glyphWidth * scaleFactor + spacing;

        if ((lineOffsetX + textWidth) >= box.width) {
            endLine = (lastk == -1) ? i : lastk;
            if (lastk == -1) {
                i -= (byteLeft - 1);
            }
            else {
                i = endLine;
            }

            lineOffsetX = 0;
            lineOffsetY += (textHeight + 4);
            startLine = i + byteLeft;
            endLine = -1;
            lastk = -1;
        }
        else {
            DrawTextCodepoint(font, codepoint, { box.x + lineOffsetX, box.y + lineOffsetY }, fontSize, tint);
            lineOffsetX += textWidth;
        }
        i += (byteLeft - 1);
    }
}

AddSpots::AddSpots() {
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");
    iconHome = LoadTexture("assets/icons/icon_home.png");
    iconMap = LoadTexture("assets/icons/icon_map.png");
    iconFilms = LoadTexture("assets/icons/icon_films.png");
    iconOffers = LoadTexture("assets/icons/icon_offers.png");
    iconProfile = LoadTexture("assets/icons/icon_profile.png");
    customFont = LoadFont("assets/fonts/Roboto-Medium.ttf");

    currentScreen = nullptr;
    activeIndex = 1;
    isLoggedIn = false;
    userName = "";
    isAdmin = false;

    scrollYOffset = 0.0f;
    maxScrollY = 300.0f;

    isTransitioning = false;
    transitionFrames = 0;
    shouldRefreshExperiences = false;

    ResetForm();
}

AddSpots::~AddSpots() {
    Unload();
}

void AddSpots::SetScreenPointer(int* screen) {
    currentScreen = screen;
}

void AddSpots::SetUserData(bool loggedIn, const std::string& name, bool admin) {
    isLoggedIn = loggedIn;
    isAdmin = admin;
    size_t spacePos = name.find(' ');
    if (spacePos != std::string::npos) {
        userName = name.substr(0, spacePos);
    }
    else {
        userName = name;
    }
}

void AddSpots::Unload() {
    if (background.id != 0) UnloadTexture(background);
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);
    if (customFont.texture.id != 0) UnloadFont(customFont);
}

void AddSpots::ResetForm() {
    titleInput = "";
    descInput = "";
    tag1Input = "";
    tag2Input = "";
    tag3Input = "";
    activeField = -1;
    scrollYOffset = 0.0f;
    isTransitioning = false;
    transitionFrames = 0;
}

void AddSpots::Update() {
    if (isTransitioning) {
        transitionFrames++;
        if (transitionFrames > 3) {
            isTransitioning = false;
            transitionFrames = 0;
            if (currentScreen != nullptr) {
                *currentScreen = 5;
            }
        }
        return;
    }

    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navBarX = (screenWidth - navWidth) / 2.0f;

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    float navStartX = navBarX + 930;
    float navSpacing = 94.0f;
    for (int i = 0; i < 4; i++) {
        Rectangle btnRect = { navStartX + (i * navSpacing), 20, 110, 100 };
        if (CheckCollisionPointRec(mousePos, btnRect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                activeIndex = i;
                if (currentScreen != nullptr) {
                    if (i == 0) { *currentScreen = 2; ResetForm(); return; }
                    else if (i == 1) { *currentScreen = 5; ResetForm(); return; }
                    else if (i == 2) { *currentScreen = 4; ResetForm(); return; }
                    else if (i == 3) { *currentScreen = 6; ResetForm(); return; }
                }
            }
        }
    }

    Rectangle profileRect = { navBarX + navWidth - 70, 35, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (currentScreen != nullptr) {
                *currentScreen = 1;
                ResetForm();
                return;
            }
        }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentScreen != nullptr) {
            *currentScreen = 5;
            ResetForm();
            return;
        }
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        scrollYOffset -= wheel * 45.0f;
        if (scrollYOffset < 0) scrollYOffset = 0;
        if (scrollYOffset > maxScrollY) scrollYOffset = maxScrollY;
    }

    float panelW = screenWidth * 0.92f;
    float panelX = (screenWidth - panelW) / 2.0f;

    float centerContentWidth = 1140.0f;
    float contentStartX = panelX + (panelW - centerContentWidth) / 2.0f;
    float baseContentY = 320.0f - scrollYOffset;

    Rectangle fields[5] = {
        { contentStartX, baseContentY + 90.0f,  1140, 60 },
        { contentStartX, baseContentY + 230.0f, 1140, 180 },
        { contentStartX, baseContentY + 490.0f, 360,  60 },
        { contentStartX + 390.0f, baseContentY + 490.0f, 360, 60 },
        { contentStartX + 780.0f, baseContentY + 490.0f, 360, 60 }
    };

    float buttonsYStart = fields[2].y + fields[2].height + 60.0f;
    Rectangle submitBtnBounds = { contentStartX, buttonsYStart, 1140, 55 };
    Rectangle cancelBtnBounds = { contentStartX, buttonsYStart + 75.0f, 1140, 55 };

    Rectangle backLinkBounds = { panelX + 20, 155.0f - scrollYOffset, 200, 30 };

    bool hoverSubmit = CheckCollisionPointRec(mousePos, submitBtnBounds);
    bool hoverCancel = CheckCollisionPointRec(mousePos, cancelBtnBounds);
    bool hoverBack = CheckCollisionPointRec(mousePos, backLinkBounds);

    if (hoverSubmit || hoverCancel || hoverBack) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (hoverBack || hoverCancel) {
            if (currentScreen != nullptr) {
                *currentScreen = 5;
            }
            ResetForm();
            return;
        }

        activeField = -1;
        for (int i = 0; i < 5; i++) {
            if (CheckCollisionPointRec(mousePos, fields[i])) {
                activeField = i;
                break;
            }
        }

        if (hoverSubmit) {
            if (!titleInput.empty() && !descInput.empty()) {
                shouldRefreshExperiences = true;
                isTransitioning = true;
                transitionFrames = 0;
                if (currentScreen != nullptr) {
                    *currentScreen = 5;
                }
                return;
            }
        }
    }

    if (activeField >= 0 && activeField <= 4) {
        std::string* targetStr = nullptr;
        size_t maxLen = 60;

        if (activeField == 0) { targetStr = &titleInput; maxLen = 100; }
        else if (activeField == 1) { targetStr = &descInput; maxLen = 500; }
        else if (activeField == 2) { targetStr = &tag1Input; maxLen = 30; }
        else if (activeField == 3) { targetStr = &tag2Input; maxLen = 30; }
        else if (activeField == 4) { targetStr = &tag3Input; maxLen = 30; }

        if (targetStr) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (targetStr->length() < maxLen)) {
                    *targetStr += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !targetStr->empty()) {
                targetStr->pop_back();
            }
        }
    }
}

void AddSpots::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background, { 0, 0, (float)background.width, (float)background.height }, { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        ClearBackground(RAYWHITE);
    }

    if (customFont.texture.id == 0) return;

    int screenWidth = GetScreenWidth();
    float panelW = screenWidth * 0.92f;
    float panelX = (screenWidth - panelW) / 2.0f;
    Color linkBlue = Color{ 54, 84, 218, 255 };
    Vector2 mousePos = GetMousePosition();

    BeginScissorMode(0, 135, GetScreenWidth(), GetScreenHeight() - 135);

    float baseContentY = 320.0f - scrollYOffset;
    float headerTopY = 165.0f - scrollYOffset;

    DrawTextEx(customFont, "<- Back to Spots", { panelX + 20, headerTopY }, 24, 1, linkBlue);
    DrawTextEx(customFont, "Add New Experience", { panelX + 20, headerTopY + 37.0f }, 46, 1, Color{ 14, 21, 61, 255 });
    DrawTextEx(customFont, "Fill in the layout configuration rules to establish a new custom experience setting.", { panelX + 20, headerTopY + 92.0f }, 22, 1, GRAY);

    float canvasCalculatedHeight = 850.0f;
    Rectangle canvasRec = { panelX, baseContentY, panelW, canvasCalculatedHeight };
    DrawRectangleRounded(canvasRec, 0.015f, 8, WHITE);

    float centerContentWidth = 1140.0f;
    float contentStartX = panelX + (panelW - centerContentWidth) / 2.0f;

    Rectangle fields[5] = {
        { contentStartX, baseContentY + 90.0f,  1140, 60 },
        { contentStartX, baseContentY + 230.0f, 1140, 180 },
        { contentStartX, baseContentY + 490.0f, 360,  60 },
        { contentStartX + 390.0f, baseContentY + 490.0f, 360, 60 },
        { contentStartX + 780.0f, baseContentY + 490.0f, 360, 60 }
    };

    const char* labels[5] = { "Experience Title *", "Description / Subtext *", "Tag Identifier 1 *", "Tag Identifier 2", "Tag Identifier 3" };
    std::string contents[5] = { titleInput, descInput, tag1Input, tag2Input, tag3Input };

    for (int i = 0; i < 5; i++) {
        DrawTextEx(customFont, labels[i], { fields[i].x, fields[i].y - 32 }, 26, 1, Color{ 14, 21, 61, 255 });
        DrawRectangleRounded(fields[i], i == 1 ? 0.02f : 0.15f, 6, WHITE);
        DrawRectangleRoundedLines(fields[i], i == 1 ? 0.02f : 0.15f, 6, 1, (activeField == i) ? linkBlue : Color{ 215, 222, 235, 255 });

        if (!contents[i].empty()) {
            if (i == 1) {
                Rectangle textInnerBounds = { fields[i].x + 15, fields[i].y + 15, fields[i].width - 30, fields[i].height - 30 };
                DrawTextBoxed(customFont, contents[i].c_str(), textInnerBounds, 22, 1, true, BLACK);
            }
            else {
                DrawTextEx(customFont, contents[i].c_str(), { fields[i].x + 15, fields[i].y + 18 }, 22, 1, BLACK);
            }
        }
    }

    float buttonsYStart = fields[2].y + fields[2].height + 60.0f;
    Rectangle submitBtnBounds = { contentStartX, buttonsYStart, 1140, 55 };
    Rectangle cancelBtnBounds = { contentStartX, buttonsYStart + 75.0f, 1140, 55 };

    bool hoverSubmit = CheckCollisionPointRec(mousePos, submitBtnBounds);
    bool hoverCancel = CheckCollisionPointRec(mousePos, cancelBtnBounds);

    DrawRectangleRounded(submitBtnBounds, 0.15f, 6, hoverSubmit ? Color{ 30, 40, 90, 255 } : Color{ 14, 21, 61, 255 });
    DrawTextEx(customFont, "Publish Experience Setup", { submitBtnBounds.x + (1140.0f - MeasureTextEx(customFont, "Publish Experience Setup", 24, 1).x) / 2.0f, submitBtnBounds.y + 15.0f }, 24, 1, WHITE);

    DrawRectangleRounded(cancelBtnBounds, 0.15f, 6, hoverCancel ? Color{ 230, 235, 245, 255 } : WHITE);
    DrawRectangleRoundedLines(cancelBtnBounds, 0.15f, 6, 1.5f, Color{ 210, 215, 230, 255 });
    DrawTextEx(customFont, "Cancel Setup Action", { cancelBtnBounds.x + (1140.0f - MeasureTextEx(customFont, "Cancel Setup Action", 24, 1).x) / 2.0f, cancelBtnBounds.y + 15.0f }, 24, 1, Color{ 14, 21, 61, 255 });

    EndScissorMode();

    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    Rectangle navBarRect = { (screenWidth - navWidth) / 2.0f, 20.0f, navWidth, navHeight };

    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);

    const char* navLabels[] = { "Home", "Spots", "Films", "Offers" };
    Texture2D navIcons[] = { iconHome, iconMap, iconFilms, iconOffers };
    float navSpacing = 94.0f;
    float navStartX = navBarRect.x + 930;

    for (int i = 0; i < 4; i++) {
        float itemX = navStartX + (i * navSpacing);
        Color tint = (i == activeIndex) ? BLUE : DARKBLUE;

        if (navIcons[i].id != 0) {
            DrawTextureEx(navIcons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        }
        DrawTextEx(customFont, navLabels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (iconProfile.id != 0) {
        DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + (isLoggedIn ? -5.0f : 10.0f) }, 0.0f, 0.1f, isProfileHovered ? BLUE : DARKBLUE);
    }

    if (isLoggedIn && !userName.empty()) {
        Vector2 textSize = MeasureTextEx(customFont, userName.c_str(), 22, 1);
        float textX = profileRect.x + (profileRect.width / 2.0f) - (textSize.x / 1.5f);
        float textY = profileRect.y + profileRect.height + 5.0f;
        DrawTextEx(customFont, userName.c_str(), { textX, textY }, 22, 1, BLACK);
    }
}