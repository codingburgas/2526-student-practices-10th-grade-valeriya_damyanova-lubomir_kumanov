#include "AddMovie.h"
#include "raylib.h"

AddMovie::AddMovie() {
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");

    iconHome = LoadTexture("assets/icons/icon_home.png");
    iconMap = LoadTexture("assets/icons/icon_map.png");
    iconFilms = LoadTexture("assets/icons/icon_films.png");
    iconOffers = LoadTexture("assets/icons/icon_offers.png");
    iconProfile = LoadTexture("assets/icons/icon_profile.png");

    customFont = LoadFont("assets/fonts/Roboto-Medium.ttf");

    currentScreen = nullptr;
    movieService = nullptr;
    activeIndex = 2;
    isLoggedIn = false;
    userName = "";
    isAdmin = false;

    scrollYOffset = 0.0f;
    maxScrollY = 550.0f; // Adjusted for comfortable scrolling room with the buttons at the bottom

    ResetForm();
}

AddMovie::~AddMovie() {
    Unload();
}

void AddMovie::SetUserData(bool loggedIn, const std::string& name, bool admin) {
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

void AddMovie::Unload() {
    if (background.id != 0) UnloadTexture(background);
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);
    if (customFont.texture.id != 0) UnloadFont(customFont);
}

void AddMovie::ResetForm() {
    titleInput = "";
    plotInput = "";
    yearInput = "";
    genreInput = "";
    durationInput = "";
    languageInput = "";
    ratingInput = "";
    posterPathInput = "";
    activeField = -1;
    scrollYOffset = 0.0f;
}

void AddMovie::Update() {
    Vector2 mousePos = GetMousePosition();

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentScreen != nullptr) {
            *currentScreen = 4;
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

    int screenWidth = GetScreenWidth();
    float panelW = screenWidth * 0.92f;
    float panelX = (screenWidth - panelW) / 2.0f;

    // Layout matrix configurations
    float centerContentWidth = 1140.0f;
    float contentStartX = panelX + (panelW - centerContentWidth) / 2.0f;

    float posterW = 280.0f;
    float formFieldsStartX = contentStartX + posterW + 80.0f;
    float subColSplitX = formFieldsStartX + 400.0f;
    float baseContentY = 320.0f - scrollYOffset;

    Rectangle fields[7] = {
        { contentStartX,     baseContentY + 90.0f,  1140, 60 },   // Title
        { contentStartX,     baseContentY + 230.0f, 1140, 180 },  // Plot / Description
        { formFieldsStartX,  baseContentY + 490.0f, 360,  60 },   // Release Year
        { subColSplitX,      baseContentY + 490.0f, 360,  60 },   // Genre
        { formFieldsStartX,  baseContentY + 620.0f, 360,  60 },   // Duration
        { subColSplitX,      baseContentY + 620.0f, 360,  60 },   // Language
        { formFieldsStartX,  baseContentY + 750.0f, 760,  60 }    // Rating
    };

    float posterX = contentStartX;
    float posterY = baseContentY + 490.0f;
    Rectangle chooseFileBtn = { posterX + (posterW - 160.0f) / 2.0f, posterY + 310.0f, 160, 45 };

    // --- ACTIONS BUTTONS REPOSITIONED DIRECTLY UNDER RATING FIELD ---
    float buttonsYStart = fields[6].y + fields[6].height + 40.0f; // Sits 40px below the rating box
    Rectangle submitBtnBounds = { formFieldsStartX, buttonsYStart, 760, 55 };
    Rectangle cancelBtnBounds = { formFieldsStartX, buttonsYStart + 75.0f, 760, 55 };

    Rectangle backLinkBounds = { panelX + 20, 155.0f - scrollYOffset, 150, 30 };

    bool hoverSubmit = CheckCollisionPointRec(mousePos, submitBtnBounds);
    bool hoverCancel = CheckCollisionPointRec(mousePos, cancelBtnBounds);
    bool hoverChooseFile = CheckCollisionPointRec(mousePos, chooseFileBtn);
    bool hoverBack = CheckCollisionPointRec(mousePos, backLinkBounds);

    if (hoverSubmit || hoverCancel || hoverChooseFile || hoverBack) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
    }
    else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (hoverBack || hoverCancel) {
            if (currentScreen != nullptr) *currentScreen = 4;
            ResetForm();
            return;
        }

        if (hoverChooseFile) {
            posterPathInput = "assets/posters/custom.png";
        }

        activeField = -1;
        for (int i = 0; i < 7; i++) {
            if (CheckCollisionPointRec(mousePos, fields[i])) {
                activeField = i;
                break;
            }
        }

        if (hoverSubmit) {
            if (!titleInput.empty() && movieService != nullptr) {
                if (currentScreen != nullptr) *currentScreen = 4;
                ResetForm();
                return;
            }
        }
    }

    if (activeField >= 0 && activeField <= 6) {
        std::string* targetStr = nullptr;
        size_t maxLen = 60;

        if (activeField == 0) { targetStr = &titleInput; maxLen = 100; }
        else if (activeField == 1) { targetStr = &plotInput; maxLen = 1000; }
        else if (activeField == 2) { targetStr = &yearInput; maxLen = 10; }
        else if (activeField == 3) { targetStr = &genreInput; maxLen = 40; }
        else if (activeField == 4) { targetStr = &durationInput; maxLen = 20; }
        else if (activeField == 5) { targetStr = &languageInput; maxLen = 40; }
        else if (activeField == 6) { targetStr = &ratingInput; maxLen = 5; }

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

void AddMovie::Draw() {
    // 1. LAYER 0: Draw background texture across the entire screen window frame boundary
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

    // 2. LAYER 1: Scissor Mode acts as window protection barrier for layout scroll behaviors
    BeginScissorMode(0, 135, GetScreenWidth(), GetScreenHeight() - 135);

    float baseContentY = 320.0f - scrollYOffset;
    float headerTopY = 165.0f - scrollYOffset;

    // Header Meta Text
    DrawTextEx(customFont, "<- Back to Films", { panelX + 20, headerTopY }, 24, 1, linkBlue);
    DrawTextEx(customFont, "Add New Movie", { panelX + 20, headerTopY + 37.0f }, 46, 1, Color{ 14, 21, 61, 255 });
    DrawTextEx(customFont, "Fill in the details below to add a new movie to Vanema.", { panelX + 20, headerTopY + 92.0f }, 22, 1, GRAY);

    // Dynamic Sizing Container Card
    float canvasCalculatedHeight = 1100.0f;
    Rectangle canvasRec = { panelX, baseContentY, panelW, canvasCalculatedHeight };
    DrawRectangleRounded(canvasRec, 0.015f, 8, WHITE);

    float centerContentWidth = 1140.0f;
    float contentStartX = panelX + (panelW - centerContentWidth) / 2.0f;

    float posterW = 280.0f;
    float posterH = 410.0f;
    float posterX = contentStartX;
    float posterY = baseContentY + 490.0f;

    // Draw Movie Poster Container
    DrawTextEx(customFont, "Movie Poster", { posterX, posterY - 28 }, 24, 1, Color{ 14, 21, 61, 255 });
    Rectangle dashBound = { posterX, posterY, posterW, posterH };
    DrawRectangleRoundedLines(dashBound, 0.03f, 8, 2, Color{ 195, 207, 225, 255 });

    DrawCircleV(Vector2{ posterX + posterW / 2.0f, posterY + 160.0f }, 34.0f, Color{ 232, 238, 248, 255 });
    DrawTextEx(customFont, "+", { posterX + posterW / 2.0f - 10, posterY + 142.0f }, 38, 1, linkBlue);
    DrawTextEx(customFont, "Upload Poster", { posterX + (posterW - MeasureTextEx(customFont, "Upload Poster", 22, 1).x) / 2.0f, posterY + 225.0f }, 22, 1, Color{ 14, 21, 61, 255 });
    DrawTextEx(customFont, "JPG, PNG or WEBP. Max size 5MB.", { posterX + (posterW - MeasureTextEx(customFont, "JPG, PNG or WEBP. Max size 5MB.", 15, 1).x) / 2.0f, posterY + 260.0f }, 15, 1, GRAY);

    Rectangle chooseFileBtn = { posterX + (posterW - 160.0f) / 2.0f, posterY + 310.0f, 160, 45 };
    bool hoverChooseFile = CheckCollisionPointRec(mousePos, chooseFileBtn);
    DrawRectangleRounded(chooseFileBtn, 0.5f, 6, hoverChooseFile ? Color{ 30, 40, 90, 255 } : Color{ 14, 21, 61, 255 });
    DrawTextEx(customFont, "Choose File", { chooseFileBtn.x + (160.0f - MeasureTextEx(customFont, "Choose File", 22, 1).x) / 2.0f, chooseFileBtn.y + 12.0f }, 22, 1, WHITE);

    if (!posterPathInput.empty()) {
        DrawTextEx(customFont, "[File Loaded]", { posterX + (posterW - MeasureTextEx(customFont, "[File Loaded]", 18, 1).x) / 2.0f, posterY + 370.0f }, 18, 1, Color{ 46, 204, 113, 255 });
    }

    // Input Form Fields Matrix Setup
    float formFieldsStartX = contentStartX + posterW + 80.0f;
    float subColSplitX = formFieldsStartX + 400.0f;

    Rectangle fields[7] = {
        { contentStartX,     baseContentY + 90.0f,  1140, 60 },
        { contentStartX,     baseContentY + 230.0f, 1140, 180 },
        { formFieldsStartX,  baseContentY + 490.0f, 360,  60 },
        { subColSplitX,      baseContentY + 490.0f, 360,  60 },
        { formFieldsStartX,  baseContentY + 620.0f, 360,  60 },
        { subColSplitX,      baseContentY + 620.0f, 360,  60 },
        { formFieldsStartX,  baseContentY + 750.0f, 760,  60 }
    };

    const char* labels[7] = { "Movie Title *", "Plot / Description *", "Release Year *", "Genre *", "Duration *", "Language *", "Rating (0-10) *" };
    std::string contents[7] = { titleInput, plotInput, yearInput, genreInput, durationInput, languageInput, ratingInput };

    for (int i = 0; i < 7; i++) {
        DrawTextEx(customFont, labels[i], { fields[i].x, fields[i].y - 32 }, 26, 1, Color{ 14, 21, 61, 255 });
        DrawRectangleRounded(fields[i], i == 1 ? 0.02f : 0.15f, 6, WHITE);
        DrawRectangleRoundedLines(fields[i], i == 1 ? 0.02f : 0.15f, 6, 1, (activeField == i) ? linkBlue : Color{ 215, 222, 235, 255 });

        float textYOffset = (i == 1) ? (fields[i].y + 15.0f) : (fields[i].y + (fields[i].height - 28.0f) / 2.0f);
        DrawTextEx(customFont, contents[i].c_str(), { fields[i].x + 15, textYOffset }, 26, 1, BLACK);
    }

    // Draw Vertically Stacked Action Buttons Below Rating Field
    float buttonsYStart = fields[6].y + fields[6].height + 40.0f;
    float buttonsWidth = fields[6].width;

    Rectangle submitBtn = { formFieldsStartX, buttonsYStart, buttonsWidth, 55 };
    bool hoverSubmit = CheckCollisionPointRec(mousePos, submitBtn);
    DrawRectangleRounded(submitBtn, 0.15f, 6, hoverSubmit ? Color{ 40, 50, 110, 255 } : Color{ 14, 21, 61, 255 });
    DrawTextEx(customFont, "Add Movie", { submitBtn.x + (buttonsWidth - MeasureTextEx(customFont, "Add Movie", 24, 1).x) / 2.0f, submitBtn.y + 15.0f }, 24, 1, WHITE);

    Rectangle cancelBtn = { formFieldsStartX, buttonsYStart + 75.0f, buttonsWidth, 55 };
    bool hoverCancel = CheckCollisionPointRec(mousePos, cancelBtn);
    DrawRectangleRounded(cancelBtn, 0.15f, 6, hoverCancel ? Color{ 230, 235, 245, 255 } : WHITE);
    DrawRectangleRoundedLines(cancelBtn, 0.15f, 6, 2, Color{ 195, 207, 225, 255 });
    DrawTextEx(customFont, "Cancel", { cancelBtn.x + (buttonsWidth - MeasureTextEx(customFont, "Cancel", 24, 1).x) / 2.0f, cancelBtn.y + 15.0f }, 24, 1, Color{ 80, 90, 110, 255 });

    EndScissorMode();

    // 3. LAYER 2: Sticky Top Navigation Interface Bar sitting on top of background
    DrawNavigationBar();
}

void AddMovie::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    float navBarX = (screenWidth - navWidth) / 2;

    Rectangle navBarRect = { navBarX, 20, navWidth, navHeight };
    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);

    const char* labels[] = { "Home", "Spots", "Films", "Offers" };
    Texture2D icons[] = { iconHome, iconMap, iconFilms, iconOffers };
    float spacing = 94.0f;
    float startX = navBarRect.x + 930;
    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < 4; i++) {
        float itemX = startX + (i * spacing);
        Rectangle btnRect = { itemX, navBarRect.y, 110, navHeight };
        bool isHovered = CheckCollisionPointRec(mousePos, btnRect);
        Color tint = (i == activeIndex) ? BLUE : DARKBLUE;

        if (isHovered) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
                if (i == 0) *currentScreen = 0;
                if (i == 2) *currentScreen = 4;
                return;
            }
        }
        if (icons[i].id != 0) DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        DrawTextEx(customFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
            *currentScreen = 1;
            return;
        }
    }
    if (iconProfile.id != 0) {
        float iconYOffset = isLoggedIn ? -10.0f : 10.0f;
        DrawTextureEx(iconProfile, { profileRect.x - 5, profileRect.y + iconYOffset }, 0.0f, 0.1f, DARKBLUE);
    }
    if (isLoggedIn && !userName.empty()) {
        Vector2 textSize = MeasureTextEx(customFont, userName.c_str(), 22.0f, 1);
        DrawTextEx(customFont, userName.c_str(), { profileRect.x + (profileRect.width / 2.0f) - (textSize.x / 2.0f), profileRect.y + profileRect.height + 5.0f }, 22.0f, 1, BLACK);
    }
}