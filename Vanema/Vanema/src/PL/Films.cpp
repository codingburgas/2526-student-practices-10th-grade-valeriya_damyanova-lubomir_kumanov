#include "Films.h"
#include "AddMovie.h"
#include "raylib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

static bool skipGridClickThisFrame = false;

Films::Films() {
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");

    iconHome = LoadTexture("assets/icons/icon_home.png");
    iconMap = LoadTexture("assets/icons/icon_map.png");
    iconFilms = LoadTexture("assets/icons/icon_films.png");
    iconOffers = LoadTexture("assets/icons/icon_offers.png");
    iconProfile = LoadTexture("assets/icons/icon_profile.png");

    customFont = LoadFont("assets/fonts/Roboto-Medium.ttf");

    genres = {
        {"All"}, {"Action"}, {"Adventure"}, {"Animation"},
        {"Comedy"}, {"Drama"}, {"Horror"}, {"Sci-Fi"},
        {"Romance"}, {"Family"}
    };

    currentScreen = nullptr;
    movieService = nullptr;
    addMovieScreen = nullptr;
    scrollOffset = 0.0f;
    maxScroll = 1200.0f;
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
    isAdmin = false;

    isDeleteMode = false;
    showDeleteConfirmation = false;
    hasSelectedMovieChanged = false;

    justActivated = false;
    activationFrames = 0;
    wasActive = false;
    pendingRefresh = false;
}

Films::~Films() {
    Unload();
}

void Films::ConsumeMouseClicks() {
    while (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {}
    while (GetCharPressed() != 0) {}
    while (GetKeyPressed() != 0) {}
    GetGestureDetected();
}

void Films::SetUserData(bool loggedIn, const std::string& name, bool admin) {
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
    underlyingMovies.clear();

    if (customFont.texture.id != 0) {
        UnloadFont(customFont);
    }
}

void Films::SyncDisplayWithDatabase() {
    if (movieService == nullptr) return;

    // Unload old textures from memory
    for (size_t i = 0; i < displayedMovies.size(); i++) {
        if (displayedMovies[i].posterTexture.id != 0) {
            UnloadTexture(displayedMovies[i].posterTexture);
        }
    }
    displayedMovies.clear();
    underlyingMovies.clear();

    movieService->reloadMovies();

    std::string contextGenre = genres[selectedGenreIndex].name;
    std::vector<Movie> rawDbMovies;

    if (contextGenre == "All") {
        rawDbMovies = movieService->getRandomMixForAll();
    }
    else {
        rawDbMovies = movieService->getMoviesByGenre(contextGenre);
    }

    underlyingMovies = rawDbMovies;

    for (const auto& item : rawDbMovies) {
        DisplayMovie uiCard;
        uiCard.title = item.getTitle();
        uiCard.genre = item.getGenre();
        uiCard.rating = TextFormat("Rating: %.1f", item.getRating());

        std::string rawPath = item.getPosterPath();

        // Normalize backslashes to forward slashes just in case Windows paths were saved
        std::replace(rawPath.begin(), rawPath.end(), '\\', '/');

        std::string verifiedPath = "";

        // Checking path variants
        std::vector<std::string> pathVariants = {
            rawPath,
            "assets/" + rawPath,
            "../assets/" + rawPath,
            "Vanema/" + rawPath,
            "Vanema/assets/" + rawPath,
            "../" + rawPath
        };

        for (const auto& variant : pathVariants) {
            if (FileExists(variant.c_str())) {
                verifiedPath = variant;
                break;
            }
        }

        if (!verifiedPath.empty()) {
            uiCard.posterTexture = LoadTexture(verifiedPath.c_str());
            if (uiCard.posterTexture.id == 0) {
                std::cout << "[ERROR] Raylib failed to load verified file into VRAM: " << verifiedPath << std::endl;
            }
            else {
                std::cout << "[SUCCESS] Loaded poster for " << uiCard.title << " from: " << verifiedPath << std::endl;
            }
        }
        else {
            std::cout << "[WARNING] File completely missing across all check variations for: " << uiCard.title << " (Raw path: " << rawPath << ")" << std::endl;
            // Attempt generic placeholder fallback asset 
            if (FileExists("assets/placeholder.png")) {
                uiCard.posterTexture = LoadTexture("assets/placeholder.png");
            }
            else if (FileExists("../assets/placeholder.png")) {
                uiCard.posterTexture = LoadTexture("../assets/placeholder.png");
            }
            else {
                uiCard.posterTexture.id = 0;
            }
        }

        displayedMovies.push_back(uiCard);
    }

    int layoutRows = ((int)displayedMovies.size() + 3) / 4;
    maxScroll = 550.0f + (layoutRows * 520.0f) - GetScreenHeight();
    if (maxScroll < 600.0f) maxScroll = 600.0f;
}

void Films::Update() {
    bool isActive = (currentScreen != nullptr && *currentScreen == 4);

    if (isActive && !wasActive) {
        ConsumeMouseClicks();
        skipGridClickThisFrame = true;
        justActivated = true;
        activationFrames = 0;

        if (addMovieScreen && addMovieScreen->ShouldRefreshMovies()) {
            pendingRefresh = true;
        }

        SyncDisplayWithDatabase();
    }

    wasActive = isActive;

    if (justActivated) {
        activationFrames++;
        if (activationFrames > 3) {
            justActivated = false;
            skipGridClickThisFrame = false;
        }
    }

    if (!isActive) return;

    if (pendingRefresh) {
        SyncDisplayWithDatabase();
        pendingRefresh = false;
        if (addMovieScreen) {
            addMovieScreen->SetRefreshMovies(false);
        }
    }

    Vector2 mousePos = GetMousePosition();

    if (!showDeleteConfirmation && !justActivated) {
        skipGridClickThisFrame = false;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        if (showDeleteConfirmation) {
            showDeleteConfirmation = false;
            skipGridClickThisFrame = true;
        }
        else if (isDeleteMode) {
            isDeleteMode = false;
        }
    }

    if (showDeleteConfirmation) {
        float mWidth = 650.0f;
        float mHeight = 220.0f;
        Rectangle modalContainer = {
            ((float)GetScreenWidth() - mWidth) / 2.0f,
            ((float)GetScreenHeight() - mHeight) / 2.0f,
            mWidth,
            mHeight
        };

        Rectangle yesBtn = { modalContainer.x + 60, modalContainer.y + 140, 150, 45 };
        Rectangle noBtn = { modalContainer.x + 440, modalContainer.y + 140, 150, 45 };

        if (CheckCollisionPointRec(mousePos, yesBtn)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (movieService != nullptr) {
                    std::string movieTitle = movieToDelete.getTitle();
                    movieService->deleteMovie(movieTitle);

                    auto it = std::find_if(underlyingMovies.begin(), underlyingMovies.end(),
                        [&movieTitle](const Movie& m) { return m.getTitle() == movieTitle; });
                    if (it != underlyingMovies.end()) {
                        size_t dist = std::distance(underlyingMovies.begin(), it);
                        if (dist < displayedMovies.size()) {
                            if (displayedMovies[dist].posterTexture.id != 0) {
                                UnloadTexture(displayedMovies[dist].posterTexture);
                            }
                            displayedMovies.erase(displayedMovies.begin() + dist);
                        }
                        underlyingMovies.erase(it);
                    }

                    movieService->reloadMovies();

                    int layoutRows = ((int)displayedMovies.size() + 3) / 4;
                    maxScroll = 550.0f + (layoutRows * 520.0f) - GetScreenHeight();
                    if (maxScroll < 600.0f) maxScroll = 600.0f;
                }
                showDeleteConfirmation = false;
                isDeleteMode = false;
                movieToDelete = Movie();
                skipGridClickThisFrame = true;
            }
        }
        else if (CheckCollisionPointRec(mousePos, noBtn)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showDeleteConfirmation = false;
                skipGridClickThisFrame = true;
            }
        }
        return;
    }

    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 45;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }

    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navBarX = (screenWidth - navWidth) / 2;

    Rectangle searchBox = { navBarX + navWidth - 280, 155, 260, 50 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !justActivated) {
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

    float posterWidth = 260;
    float posterHeight = 380;
    float spacingX = 45;

    float startPosterX = ((screenWidth - navWidth) / 2) + 120;
    float startPosterY = startY + 80;
    int maxColumns = 4;

    Vector2 mousePos = GetMousePosition();

    for (size_t i = 0; i < displayedMovies.size(); i++) {
        int targetCol = i % maxColumns;
        int targetRow = i / maxColumns;

        float x = startPosterX + targetCol * (posterWidth + spacingX);
        float y = startPosterY + targetRow * (posterHeight + 110.0f);
        Rectangle posterRect = { x, y, posterWidth, posterHeight };

        bool hovered = !showDeleteConfirmation && CheckCollisionPointRec(mousePos, posterRect);

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
            // If the texture context id is 0, render a fallback visual rectangle
            DrawRectangleRec(posterRect, DARKGRAY);
            if (customFont.texture.id != 0) {
                DrawTextEx(customFont, "Missing Image", { x + 55, y + posterHeight / 2 - 10 }, 22, 1, WHITE);
            }
        }

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, displayedMovies[i].title.c_str(), { x + 10, y + posterHeight + 5 }, 27, 1, BLACK);
            DrawTextEx(customFont, displayedMovies[i].rating.c_str(), { x + 10, y + posterHeight + 33 }, 24, 1, ORANGE);
            DrawTextEx(customFont, displayedMovies[i].genre.c_str(), { x + 10, y + posterHeight + 55 }, 23, 1, BLACK);
        }

        DrawRectangleRoundedLines(
            posterRect,
            0.08f,
            8,
            4,
            hovered ? (isDeleteMode ? RED : BLUE) : (isDeleteMode ? ORANGE : WHITE)
        );

        if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !skipGridClickThisFrame && !justActivated) {
            if (isDeleteMode) {
                if (i < underlyingMovies.size()) {
                    movieToDelete = underlyingMovies[i];
                    showDeleteConfirmation = true;
                    return;
                }
            }
            else {
                if (!isLoggedIn) {
                    if (currentScreen != nullptr) {
                        *currentScreen = 1;
                        return;
                    }
                }
                else if (currentScreen != nullptr && i < underlyingMovies.size()) {
                    lastClickedMovie = underlyingMovies[i];
                    hasSelectedMovieChanged = true;
                    *currentScreen = 7;
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

        if (isHovered && !showDeleteConfirmation && !skipGridClickThisFrame && !justActivated) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                activeIndex = i;
                if (currentScreen != nullptr) {
                    if (i == 0) *currentScreen = 2;
                    else if (i == 1) *currentScreen = 5;
                    else if (i == 2) *currentScreen = 4;
                    else if (i == 3) *currentScreen = 6;
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

    if (isProfileHovered && !showDeleteConfirmation && !skipGridClickThisFrame && !justActivated) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
            *currentScreen = 1;
        }
    }

    if (iconProfile.id != 0) {
        float iconYOffset = isLoggedIn ? -5.0f : 10.0f;
        DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + iconYOffset }, 0.0f, 0.1f, isProfileHovered ? BLUE : DARKBLUE);
    }

    if (isLoggedIn && !userName.empty() && customFont.texture.id != 0) {
        Vector2 textSize = MeasureTextEx(customFont, userName.c_str(), 22, 1);
        float textX = profileRect.x + (profileRect.width / 2.0f) - (textSize.x / 1.5f);
        DrawTextEx(customFont, userName.c_str(), { textX, profileRect.y + profileRect.height + 5.0f }, 22, 1, BLACK);
    }
}

void Films::DrawScrollbar() {
    int screenWidth = GetScreenWidth();
    float scrollbarHeight = 450;
    float scrollbarWidth = 12;
    float scrollbarX = screenWidth - 25;
    float scrollbarY = 180;

    DrawRectangleRounded({ scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight }, 0.5f, 8, Fade(LIGHTGRAY, 0.4f));

    float thumbHeight = 110;
    float thumbY = scrollbarY;
    if (maxScroll > 0) {
        thumbY = scrollbarY + (scrollOffset / maxScroll) * (scrollbarHeight - thumbHeight);
    }
    DrawRectangleRounded({ scrollbarX, thumbY, scrollbarWidth, thumbHeight }, 0.5f, 8, DARKBLUE);
}

void Films::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background, { 0, 0, (float)background.width, (float)background.height }, { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        ClearBackground(RAYWHITE);
    }

    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    float navBarX = (screenWidth - navWidth) / 2;

    Rectangle navBarRect = { navBarX, 20, navWidth, navHeight };

    // Set scissor coordinates cleanly to global application parameters
    BeginScissorMode(0, (int)(navBarRect.y + navBarRect.height + 10), GetScreenWidth(), GetScreenHeight() - (int)(navBarRect.y + navBarRect.height + 10));

    float currentY = navBarRect.y + navBarRect.height + 35.0f - scrollOffset;

    if (isDeleteMode && !showDeleteConfirmation) {
        DrawRectangle(navBarRect.x, currentY, navBarRect.width, 40, Fade(ORANGE, 0.9f));
        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, "SELECTION MODE: Click on any poster below to process deletion. (ESC to Cancel)", { navBarRect.x + 20, currentY + 8.0f }, 20, 1, WHITE);
        }
        currentY += 60.0f;
    }

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Films", { navBarRect.x + 20, currentY }, 45, 1, BLACK);
        DrawTextEx(customFont, "Discover by genre, trending and more", { navBarRect.x + 20, currentY + 55 }, 30, 1, BLACK);
    }

    Rectangle searchBox = { navBarRect.x + navWidth - 280, currentY + 15, 260, 50 };
    DrawRectangleRounded(searchBox, 0.3f, 6, searchActive ? WHITE : Fade(WHITE, 0.7f));
    DrawRectangleRoundedLines(searchBox, 0.3f, 6, 2, searchActive ? BLUE : WHITE);

    if (letterCount == 0 && !searchActive) {
        DrawText("Search films...", searchBox.x + 15, searchBox.y + 15, 20, GRAY);
    }
    else {
        DrawText(searchQuery, searchBox.x + 15, searchBox.y + 15, 20, BLACK);
    }

    if (searchActive && (((int)(GetTime() * 2) % 2) == 0)) {
        int textWidth = MeasureText(searchQuery, 20);
        DrawRectangle(searchBox.x + 15 + textWidth + 2, searchBox.y + 15, 2, 20, BLACK);
    }

    if (isLoggedIn && isAdmin) {
        float btnWidth = 90.0f;
        float btnHeight = 40.0f;
        float btnY = currentY + 20.0f;

        Rectangle addBtn = { searchBox.x - 210, btnY, btnWidth, btnHeight };
        Rectangle deleteBtn = { searchBox.x - 110, btnY, btnWidth, btnHeight };

        Vector2 mousePos = GetMousePosition();
        bool hoverAdd = !showDeleteConfirmation && CheckCollisionPointRec(mousePos, addBtn);
        bool hoverDelete = !showDeleteConfirmation && CheckCollisionPointRec(mousePos, deleteBtn);

        if ((hoverAdd || hoverDelete) && !skipGridClickThisFrame && !justActivated) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }

        if (hoverAdd && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !skipGridClickThisFrame && !justActivated) {
            if (currentScreen != nullptr) {
                *currentScreen = 8;
                EndScissorMode();
                return;
            }
        }
        if (hoverDelete && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !skipGridClickThisFrame && !justActivated) {
            isDeleteMode = !isDeleteMode;
        }

        DrawRectangleRounded(addBtn, 0.3f, 6, hoverAdd ? Color{ 46, 204, 113, 255 } : Color{ 39, 174, 96, 255 });
        DrawText("Add", addBtn.x + 28, addBtn.y + 10, 20, BLACK);

        DrawRectangleRounded(deleteBtn, 0.3f, 6, isDeleteMode ? Color{ 142, 40, 42, 255 } : (hoverDelete ? Color{ 231, 76, 60, 255 } : Color{ 192, 41, 43, 255 }));
        DrawText("Delete", deleteBtn.x + 16, deleteBtn.y + 10, 20, BLACK);
    }

    DrawGenreBar(currentY + 145.0f);
    DrawMovieGrid(currentY + 275.0f);

    EndScissorMode();
    DrawScrollbar();
    DrawNavigationBar();

    if (showDeleteConfirmation) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

        float mWidth = 650.0f;
        float mHeight = 220.0f;
        Rectangle modalContainer = { ((float)GetScreenWidth() - mWidth) / 2.0f, ((float)GetScreenHeight() - mHeight) / 2.0f, mWidth, mHeight };

        DrawRectangleRec(modalContainer, WHITE);
        DrawRectangleLinesEx(modalContainer, 2, MAROON);

        std::string line1 = "Confirm Deletion Status";
        std::string line2 = "Are you sure you want to remove \"" + movieToDelete.getTitle() + "\"?";

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, line1.c_str(), { modalContainer.x + 40, modalContainer.y + 35 }, 24, 1, MAROON);
            DrawTextEx(customFont, line2.c_str(), { modalContainer.x + 40, modalContainer.y + 85 }, 18, 1, DARKGRAY);
        }

        Rectangle yesBtn = { modalContainer.x + 60, modalContainer.y + 140, 150, 45 };
        Rectangle noBtn = { modalContainer.x + 440, modalContainer.y + 140, 150, 45 };

        Vector2 mousePos = GetMousePosition();
        bool hoverYes = CheckCollisionPointRec(mousePos, yesBtn);
        bool hoverNo = CheckCollisionPointRec(mousePos, noBtn);

        DrawRectangleRec(yesBtn, hoverYes ? Color{ 170, 30, 35, 255 } : MAROON);
        DrawRectangleRec(noBtn, hoverNo ? Color{ 110, 110, 110, 255 } : GRAY);

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, "CONFIRM", { yesBtn.x + 38, yesBtn.y + 13 }, 16, 1, WHITE);
            DrawTextEx(customFont, "CANCEL", { noBtn.x + 44, noBtn.y + 13 }, 16, 1, WHITE);
        }
    }
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
        bool isHovered = !showDeleteConfirmation && isVisible && CheckCollisionPointRec(mousePos, pillRect);

        if (isHovered && !skipGridClickThisFrame && !justActivated) {
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
        bool leftHover = !showDeleteConfirmation && CheckCollisionPointRec(mousePos, leftBtn);

        DrawRectangleRounded(leftBtn, 0.45f, 8, leftHover ? LIGHTGRAY : Color{ 240, 244, 248, 255 });
        DrawRectangleRoundedLines(leftBtn, 0.45f, 8, 1, Color{ 220, 226, 235, 255 });
        DrawText("<", leftBtn.x + 18, leftBtn.y + 13, 24, BLACK);

        if (leftHover && !skipGridClickThisFrame && !justActivated) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                targetGenreScrollX += stepSlideAmount;
                if (targetGenreScrollX > 0) targetGenreScrollX = 0;
            }
        }
    }

    if (maxGenreScrollWidth > visibleWidthBoundary && targetGenreScrollX > -maxScrollAllowed) {
        Rectangle rightBtn = { viewRightBoundary + 15, buttonsY, arrowButtonSize, buttonHeight };
        bool rightHover = !showDeleteConfirmation && CheckCollisionPointRec(mousePos, rightBtn);

        DrawRectangleRounded(rightBtn, 0.45f, 8, rightHover ? LIGHTGRAY : Color{ 240, 244, 248, 255 });
        DrawRectangleRoundedLines(rightBtn, 0.45f, 8, 1, Color{ 220, 226, 235, 255 });
        DrawText(">", rightBtn.x + 20, rightBtn.y + 13, 24, BLACK);

        if (rightHover && !skipGridClickThisFrame && !justActivated) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                targetGenreScrollX -= stepSlideAmount;
                if (targetGenreScrollX < -maxScrollAllowed) targetGenreScrollX = -maxScrollAllowed;
            }
        }
    }
}