#include "Films.h"
#include <iostream>

Films::Films() {
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");

    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");

    customFont = LoadFont("assets/fonts/PlayfairDisplay-Medium.ttf");

    genres = {
        {"All"},
        {"Action"},
        {"Adventure"},
        {"Animation"},
        {"Comedy"},
        {"Drama"},
        {"Horror"},
        {"Sci-Fi"},
        {"Thriller"},
        {"Other"}
    };

    currentScreen = nullptr;
    scrollOffset = 0.0f;
    maxScroll = 500.0f;
    activeIndex = 2;
    searchActive = false;
    letterCount = 0;
    searchQuery[0] = '\0';
    selectedGenreIndex = 0;

    genreScrollX = 0.0f;
    targetGenreScrollX = 0.0f;
    maxGenreScrollWidth = 0.0f;
}

Films::~Films() {
    Unload();
}

void Films::Unload() {
    UnloadTexture(background);
    UnloadTexture(logo);
    UnloadTexture(iconHome);
    UnloadTexture(iconMap);
    UnloadTexture(iconFilms);
    UnloadTexture(iconOffers);
    UnloadTexture(iconProfile);
    UnloadFont(customFont);
}

void Films::Update() {
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 40;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }

    Vector2 mousePos = GetMousePosition();

    Rectangle searchBox = { 1150, 205 - scrollOffset, 260, 50 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(mousePos, searchBox))
        {
            searchActive = true;
        }
        else
        {
            if (mousePos.y > 140) {
                searchActive = false;
            }
        }
    }

    if (searchActive) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = GetCharPressed();
        while (key > 0) {
            if ((key >= 32) && (key <= 125) && (letterCount < 63)) {
                searchQuery[letterCount] = (char)key;
                searchQuery[letterCount + 1] = '\0';
                letterCount++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            searchQuery[letterCount] = '\0';
        }
    }
}

void Films::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect = { (screenWidth - navWidth) / 2, 20, navWidth, navHeight };
    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) {
        DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    }

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);
    }

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
                if (i == 0) {
                    searchActive = false;
                    *currentScreen = 2;
                    printf("Films -> Switching to Booking screen\n");
                    return;
                }
                else if (i == 2) {
                    *currentScreen = 4;
                }
            }
        }

        if (icons[i].id != 0) {
            DrawTextureEx(icons[i], { itemX + 22, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        }

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, labels[i], { itemX + 30, navBarRect.y + 70 }, 20, 1, BLACK);
        }
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (isProfileHovered) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
            bool isUserLoggedIn = false;

            searchActive = false;
            if (!isUserLoggedIn) {
                *currentScreen = 1;
                printf("Films -> Switching to Login screen\n");
            }
            else {
                *currentScreen = 2;
                printf("Films -> Active session found, heading to Booking dashboard\n");
            }
            return;
        }
    }

    if (iconProfile.id != 0) {
        DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + 10 }, 0.0f, 0.1f, isProfileHovered ? BLUE : DARKBLUE);
    }
}

void Films::DrawScrollbar()
{
    int screenWidth = GetScreenWidth();

    float scrollbarHeight = 350;
    float scrollbarWidth = 10;

    float scrollbarX = screenWidth - 25;
    float scrollbarY = 150;

    DrawRectangleRounded(
        { scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight },
        0.5f, 8, Fade(LIGHTGRAY, 0.4f)
    );

    float thumbHeight = 80;
    float thumbY = scrollbarY;

    if (maxScroll > 0)
    {
        thumbY = scrollbarY + (scrollOffset / maxScroll) * (scrollbarHeight - thumbHeight);
    }

    DrawRectangleRounded(
        { scrollbarX, thumbY, scrollbarWidth, thumbHeight },
        0.5f, 8, DARKBLUE
    );
}

void Films::Draw() {
    if (background.id != 0) {
        DrawTexturePro(
            background,
            { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 }, 0.0f, WHITE
        );
    }
    else {
        ClearBackground(RAYWHITE);
    }

    int contentTopY = 140;
    int contentHeight = GetScreenHeight() - contentTopY;

    BeginScissorMode(0, contentTopY, GetScreenWidth(), contentHeight);

    if (customFont.texture.id != 0)
    {
        DrawTextEx(customFont, "Films", { 120, 160 - scrollOffset }, 45, 1, BLACK);
        DrawTextEx(customFont, "Discover by genre, trending and more", { 120, 210 - scrollOffset }, 30, 1, BLACK);
    }

    Rectangle searchBox = { 1150, 205 - scrollOffset, 260, 50 };

    DrawRectangleRounded(searchBox, 0.3f, 6, searchActive ? WHITE : Fade(WHITE, 0.7f));
    DrawRectangleRoundedLines(searchBox, 0.3f, 6, 2, searchActive ? BLUE : WHITE);

    if (letterCount == 0 && !searchActive)
    {
        DrawText("Search films...", searchBox.x + 15, searchBox.y + 15, 20, GRAY);
    }
    else
    {
        DrawText(searchQuery, searchBox.x + 15, searchBox.y + 15, 20, BLACK);
    }

    if (searchActive) {
        if (((int)(GetTime() * 2) % 2) == 0) {
            int textWidth = MeasureText(searchQuery, 20);
            DrawRectangle(searchBox.x + 15 + textWidth + 2, searchBox.y + 15, 2, 20, BLACK);
        }
    }

    DrawGenreBar(290.0f - scrollOffset);

    EndScissorMode();
    DrawScrollbar();
    DrawNavigationBar();
}

void Films::DrawGenreBar(float startY) {
    if (customFont.texture.id == 0) return;

    DrawTextEx(customFont, "Browse by Genre", { 120, startY }, 35, 1, BLACK);

    genreScrollX += (targetGenreScrollX - genreScrollX) * 0.15f;

    float baseStartX = 180.0f;
    float viewRightBoundary = GetScreenWidth() - 180.0f;

    float genresStartX = 230.0f; 
    float currentX = genresStartX + genreScrollX;

    float buttonsY = startY + 55.0f;
    float buttonWidth = 150.0f;
    float buttonHeight = 50.0f;
    float spacingBetween = 16.0f;

    Vector2 mousePos = GetMousePosition();

    for (size_t i = 0; i < genres.size(); i++) {
        Rectangle pillRect = { currentX, buttonsY, buttonWidth, buttonHeight };

        bool isVisible = (pillRect.x >= baseStartX - 5.0f) && (pillRect.x + pillRect.width <= viewRightBoundary + 5.0f);
        bool isHovered = isVisible && CheckCollisionPointRec(mousePos, pillRect);

        if (isHovered) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedGenreIndex = (int)i;
            }
        }

        if (isVisible) {
            bool isActive = (selectedGenreIndex == (int)i);
            Color bgCol = isActive ? Color{ 14, 21, 61, 255 } : Color{ 240, 244, 248, 255 };
            Color textCol = isActive ? WHITE : Color{ 60, 70, 90, 255 };

            DrawRectangleRounded(pillRect, 0.45f, 8, bgCol);
            if (!isActive) {
                DrawRectangleRoundedLines(pillRect, 0.45f, 8, 1, Color{ 220, 226, 235, 255 });
            }

            float targetFontSize = 26.0f;
            Vector2 textSize = MeasureTextEx(customFont, genres[i].name.c_str(), targetFontSize, 1);
            float textX = pillRect.x + (buttonWidth - textSize.x) / 2.0f;
            float textY = pillRect.y + (buttonHeight - textSize.y) / 2.0f;

            DrawTextEx(customFont, genres[i].name.c_str(), { textX, textY }, targetFontSize, 1, textCol);
        }

        currentX += buttonWidth + spacingBetween;
    }

    maxGenreScrollWidth = (genres.size() * (buttonWidth + spacingBetween)) - spacingBetween;
    float visibleWidthBoundary = viewRightBoundary - baseStartX;
    float maxScrollAllowed = maxGenreScrollWidth - visibleWidthBoundary;

    if (maxScrollAllowed < 0) maxScrollAllowed = 0;

    float arrowButtonSize = 50.0f;
    float stepSlideAmount = 166.0f;

    if (targetGenreScrollX < 0) {
        Rectangle leftBtn = { baseStartX - 65, buttonsY, arrowButtonSize, buttonHeight };
        bool leftHover = CheckCollisionPointRec(mousePos, leftBtn);

        DrawRectangleRounded(leftBtn, 0.45f, 8, leftHover ? LIGHTGRAY : Color{ 240, 244, 248, 255 });
        DrawRectangleRoundedLines(leftBtn, 0.45f, 8, 1, Color{ 220, 226, 235, 255 });
        DrawText("<", leftBtn.x + 18, leftBtn.y + 13, 24, BLACK);

        if (leftHover) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                targetGenreScrollX += stepSlideAmount;
                if (targetGenreScrollX > 0) targetGenreScrollX = 0;
            }
        }
    }

    if (maxGenreScrollWidth > visibleWidthBoundary && targetGenreScrollX > -maxScrollAllowed) {
        Rectangle rightBtn = { viewRightBoundary + 15, buttonsY, arrowButtonSize, buttonHeight };
        bool rightHover = CheckCollisionPointRec(mousePos, rightBtn);

        DrawRectangleRounded(rightBtn, 0.45f, 8, rightHover ? LIGHTGRAY : Color{ 240, 244, 248, 255 });
        DrawRectangleRoundedLines(rightBtn, 0.45f, 8, 1, Color{ 220, 226, 235, 255 });
        DrawText(">", rightBtn.x + 20, rightBtn.y + 13, 24, BLACK);

        if (rightHover) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                targetGenreScrollX -= stepSlideAmount;
                if (targetGenreScrollX < -maxScrollAllowed) targetGenreScrollX = -maxScrollAllowed;
            }
        }
    }
}