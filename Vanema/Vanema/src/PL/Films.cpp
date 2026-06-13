#include "Films.h"
#include <iostream>
#include <fstream>

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
        {"Romance"},
        {"Family"}
    };

    currentScreen = nullptr;
    scrollOffset = 0.0f;
    maxScroll = 500.0f;
    activeIndex = 2;
    searchActive = false;
    letterCount = 0;
    searchQuery[0] = '\0';
    selectedGenreIndex = 0;
    lastSelectedGenreIndex = -1;

    genreScrollX = 0.0f;
    targetGenreScrollX = 0.0f;
    maxGenreScrollWidth = 0.0f;

    isLoggedIn = false;
    userName = "";
}

Films::~Films() {
    Unload();
}

void Films::SetUserData(bool loggedIn, const std::string& name) {
    isLoggedIn = loggedIn;

    // Look for the first space character to isolate the first name
    size_t spacePos = name.find(' ');
    if (spacePos != std::string::npos) {
        userName = name.substr(0, spacePos); // Extract just the first name
    }
    else {
        userName = name; // Fallback if there is no surname/space present
    }
}

void Films::Unload() {
    if (background.id != 0) UnloadTexture(background);
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);

    for (size_t i = 0; i < displayedMovies.size(); i++) {
        if (displayedMovies[i].posterTexture.id != 0) {
            UnloadTexture(displayedMovies[i].posterTexture);
        }
    }
    displayedMovies.clear();

    if (customFont.texture.id != 0) {
        UnloadFont(customFont);
    }
}

void Films::SyncDisplayWithDatabase() {
    if (movieService == nullptr) return;

    for (size_t i = 0; i < displayedMovies.size(); i++) {
        if (displayedMovies[i].posterTexture.id != 0) {
            UnloadTexture(displayedMovies[i].posterTexture);
        }
    }
    displayedMovies.clear();

    std::string contextGenre = genres[selectedGenreIndex].name;
    std::vector<Movie> rawDbMovies;

    if (contextGenre == "All") {
        rawDbMovies = movieService->getRandomMixForAll();
    }
    else {
        rawDbMovies = movieService->getMoviesByGenre(contextGenre);
    }

    for (const auto& item : rawDbMovies) {
        DisplayMovie uiCard;
        uiCard.title = item.getTitle();
        uiCard.genre = item.getGenre();
        uiCard.rating = TextFormat("Rating: %.1f", item.getRating());

        std::string rawPath = item.getPosterPath();
        std::string verifiedPath = rawPath;
        std::ifstream fileCheck(verifiedPath);
        if (!fileCheck.good()) {
            verifiedPath = "Vanema/" + rawPath;
            std::ifstream nestedCheck(verifiedPath);
            if (!nestedCheck.good()) {
                verifiedPath = "../" + rawPath;
            }
        }

        uiCard.posterTexture = LoadTexture(verifiedPath.c_str());
        displayedMovies.push_back(uiCard);
    }

    int layoutRows = ((int)displayedMovies.size() + 3) / 4;
    maxScroll = 320.0f + (layoutRows * 500.0f) - GetScreenHeight();
    if (maxScroll < 400.0f) maxScroll = 400.0f;
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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mousePos, searchBox)) {
            searchActive = true;
        }
        else {
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

    if (selectedGenreIndex != lastSelectedGenreIndex) {
        lastSelectedGenreIndex = selectedGenreIndex;
        SyncDisplayWithDatabase();
    }
}

void Films::DrawMovieGrid(float startY) {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect =
    {
        (screenWidth - navWidth) / 2,
        20,
        navWidth,
        navHeight
    };

    float posterWidth = 260;
    float posterHeight = 380;
    float spacingX = 45;

    float startPosterX = navBarRect.x + 120;
    float startPosterY = startY + 80;
    int maxColumns = 4;

    Vector2 mousePos = GetMousePosition();

    for (size_t i = 0; i < displayedMovies.size(); i++) {
        int targetCol = i % maxColumns;
        int targetRow = i / maxColumns;

        float x = startPosterX + targetCol * (posterWidth + spacingX);
        float y = startPosterY + targetRow * (posterHeight + 110.0f);
        Rectangle posterRect = { x, y, posterWidth, posterHeight };

        bool hovered = CheckCollisionPointRec(mousePos, posterRect);

        if (displayedMovies[i].posterTexture.id != 0) {
            DrawTexturePro(
                displayedMovies[i].posterTexture,
                { 0, 0, (float)displayedMovies[i].posterTexture.width, (float)displayedMovies[i].posterTexture.height },
                posterRect,
                { 0, 0 },
                0.0f,
                hovered ? LIGHTGRAY : WHITE
            );
        }
        else {
            DrawRectangleRec(posterRect, DARKGRAY);
            if (customFont.texture.id != 0) {
                DrawTextEx(customFont, "No Image", { x + 80, y + posterHeight / 2 }, 20, 1, WHITE);
            }
        }

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, displayedMovies[i].title.c_str(),
                { x + 10, y + posterHeight + 5 }, 27, 1, BLACK);

            DrawTextEx(customFont, displayedMovies[i].rating.c_str(),
                { x + 10, y + posterHeight + 33 }, 24, 1, ORANGE);

            DrawTextEx(customFont, displayedMovies[i].genre.c_str(),
                { x + 10, y + posterHeight + 55 }, 23, 1, BLACK);
        }

        DrawRectangleRoundedLines(
            posterRect,
            0.08f,
            8,
            4,
            hovered ? BLUE : WHITE
        );

        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            std::cout << "Selected movie: " << displayedMovies[i].title << "\n";
            if (!isLoggedIn) {
                if (currentScreen != nullptr) {
                    *currentScreen = 1;
                    EndScissorMode();
                    return;
                }
            }
        }
    }
}

void Films::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect =
    {
        (screenWidth - navWidth) / 2,
        20,
        navWidth,
        navHeight
    };

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
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                activeIndex = i;
                if (currentScreen != nullptr) {
                    if (i == 0) {
                        searchActive = false;
                        *currentScreen = 2;
                        return;
                    }
                    else if (i == 2) {
                        *currentScreen = 4;
                    }
                }
            }
        }

        if (icons[i].id != 0) {
            DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        }

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
        }
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool isProfileHovered = CheckCollisionPointRec(mousePos, profileRect);

    if (isProfileHovered) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
            searchActive = false;
            *currentScreen = 1;
            return;
        }
    }
    if (iconProfile.id != 0)
    {
        float iconYOffset = isLoggedIn ? -10.0f : 10.0f;

        DrawTextureEx(
            iconProfile,
            { profileRect.x - 5, profileRect.y + iconYOffset },
            0.0f,
            0.1f,
            isProfileHovered ? BLUE : DARKBLUE
        );
    }

    if (isLoggedIn && !userName.empty()) {
        std::cout << "Films class rendering user: " << userName << "\n";

        float fontSize = 20.0f;
        Color nameColor = MAROON;

        if (customFont.texture.id != 0) {
            Vector2 textSize = MeasureTextEx(customFont, userName.c_str(), fontSize, 1);
            float textX = profileRect.x + (profileRect.width / 2.0f) - (textSize.x / 2.0f);
            float textY = profileRect.y + profileRect.height + 5.0f;

            DrawTextEx(customFont, userName.c_str(), { textX, textY }, fontSize, 1, nameColor);
        }
        else {
            int textWidth = MeasureText(userName.c_str(), 16);
            int textX = profileRect.x + (profileRect.width / 2) - (textWidth / 2);
            DrawText(userName.c_str(), textX, profileRect.y + profileRect.height + 5, 16, BLACK);
        }
    }
}

void Films::DrawScrollbar() {
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

    if (maxScroll > 0) {
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
            { 0, 0 },
            0.0f,
            WHITE
        );
    }
    else {
        ClearBackground(RAYWHITE);
    }

    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;

    Rectangle navBarRect =
    {
        (screenWidth - navWidth) / 2,
        20,
        navWidth,
        navHeight
    };

    BeginScissorMode(
        (int)navBarRect.x,
        (int)(navBarRect.y + navBarRect.height),
        (int)navBarRect.width,
        GetScreenHeight() - (int)(navBarRect.y + navBarRect.height)
    );

    float contentY = navBarRect.y + navBarRect.height + 40 - scrollOffset;

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Films", { navBarRect.x + 20, contentY }, 45, 1, BLACK);
        DrawTextEx(customFont, "Discover by genre, trending and more", { navBarRect.x + 20, contentY + 50 }, 30, 1, BLACK);
    }

    Rectangle searchBox = { navBarRect.x + navWidth - 280, contentY + 15, 260, 50 };
    DrawRectangleRounded(searchBox, 0.3f, 6, searchActive ? WHITE : Fade(WHITE, 0.7f));
    DrawRectangleRoundedLines(searchBox, 0.3f, 6, 2, searchActive ? BLUE : WHITE);

    if (letterCount == 0 && !searchActive) {
        DrawText("Search films...", searchBox.x + 15, searchBox.y + 15, 20, GRAY);
    }
    else {
        DrawText(searchQuery, searchBox.x + 15, searchBox.y + 15, 20, BLACK);
    }

    if (searchActive) {
        if (((int)(GetTime() * 2) % 2) == 0) {
            int textWidth = MeasureText(searchQuery, 20);
            DrawRectangle(searchBox.x + 15 + textWidth + 2, searchBox.y + 15, 2, 20, BLACK);
        }
    }

    DrawGenreBar(contentY + 120.0f);
    DrawMovieGrid(contentY + 230.0f);

    EndScissorMode();
    DrawScrollbar();
    DrawNavigationBar();
}

void Films::DrawGenreBar(float startY) {
    if (customFont.texture.id == 0) return;

    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navBarX = (screenWidth - navWidth) / 2;

    DrawTextEx(customFont, "Browse by Genre", { navBarX + 20, startY }, 35, 1, BLACK);
    genreScrollX += (targetGenreScrollX - genreScrollX) * 0.15f;

    float baseStartX = navBarX + 120.0f;
    float viewRightBoundary = navBarX + navWidth - 120.0f;
    float genresStartX = navBarX + 150.0f;
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