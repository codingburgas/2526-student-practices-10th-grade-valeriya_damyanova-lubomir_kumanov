#include "MovieDetails.h"
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime> 

static std::string g_liveDates[3];
static void UpdateLiveDateBuffers() {
    for (int i = 0; i < 3; i++) {
        std::time_t t = std::time(nullptr);
        std::tm* localTime = std::localtime(&t);
        if (localTime) {
            localTime->tm_mday += i;
            std::mktime(localTime); 
            char buf[64];
            if (i == 0) std::strftime(buf, sizeof(buf), "Today, %b %d", localTime);
            else std::strftime(buf, sizeof(buf), "%a, %b %d", localTime);
            g_liveDates[i] = buf;
        }
    }
}

MovieDetails::MovieDetails() : textureLoaded(false), posterTexture{} {
    customFont = LoadFont("assets/fonts/Roboto-Medium.ttf");
    background = LoadTexture("assets/booking.png");
    logo = LoadTexture("assets/logo.png");

    iconHome = LoadTexture("assets/icons/icon_home.png");
    iconMap = LoadTexture("assets/icons/icon_map.png");
    iconFilms = LoadTexture("assets/icons/icon_films.png");
    iconOffers = LoadTexture("assets/icons/icon_offers.png");
    iconProfile = LoadTexture("assets/icons/icon_profile.png");

    isLoggedIn = false;
    userName = "";

    selectedShowtimeIndex = 2;
    calendarDropdownOpen = false;

    UpdateLiveDateBuffers();
    selectedDateText = g_liveDates[0].c_str();
}

MovieDetails::~MovieDetails() {
    UnloadCurrentPoster();
    if (customFont.texture.id != 0) UnloadFont(customFont);
    if (background.id != 0) UnloadTexture(background);
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);
}

void MovieDetails::SetUserData(bool loggedIn, const std::string& name) {
    isLoggedIn = loggedIn;
    size_t spacePos = name.find(' ');
    userName = (spacePos != std::string::npos) ? name.substr(0, spacePos) : name;
}

void MovieDetails::LoadMovie(const Movie& movie) {
    UnloadCurrentPoster();
    currentMovie = movie;

    std::string rawPath = currentMovie.getPosterPath();
    std::replace(rawPath.begin(), rawPath.end(), '\\', '/');

    std::string verifiedPath = "";
    std::vector<std::string> pathVariants = {
        rawPath, "assets/" + rawPath, "../assets/" + rawPath,
        "Vanema/" + rawPath, "Vanema/assets/" + rawPath, "../" + rawPath
    };

    for (const auto& variant : pathVariants) {
        if (FileExists(variant.c_str())) {
            verifiedPath = variant;
            break;
        }
    }

    if (!verifiedPath.empty()) {
        posterTexture = LoadTexture(verifiedPath.c_str());
        textureLoaded = (posterTexture.id != 0);
    }
    else {
        if (FileExists("assets/placeholder.png")) {
            posterTexture = LoadTexture("assets/placeholder.png");
            textureLoaded = (posterTexture.id != 0);
        }
        else {
            posterTexture.id = 0;
            textureLoaded = false;
        }
    }
}

void MovieDetails::UnloadCurrentPoster() {
    if (textureLoaded && posterTexture.id != 0) {
        UnloadTexture(posterTexture);
        posterTexture.id = 0;
        textureLoaded = false;
    }
}

DetailScreenResult MovieDetails::Update() {
    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navBarX = (screenWidth - navWidth) / 2.0f;

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    UpdateLiveDateBuffers(); 

    float startX = navBarX + 930;
    float spacing = 94.0f;
    for (int i = 0; i < 4; i++) {
        Rectangle btnRect = { startX + (i * spacing), 20, 110, 100 };
        if (CheckCollisionPointRec(mousePos, btnRect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (currentScreen != nullptr) {
                    if (i == 0) { *currentScreen = 2; return DetailScreenResult::NONE; }
                    if (i == 2) { *currentScreen = 4; return DetailScreenResult::BACK; }
                }
            }
        }
    }

    Rectangle profileRect = { navBarX + navWidth - 70, 35, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (currentScreen != nullptr) { *currentScreen = 1; return DetailScreenResult::NONE; }
        }
    }

    Rectangle backLinkRect = { navBarX + 20, 195, 130, 48 };
    if (CheckCollisionPointRec(mousePos, backLinkRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (currentScreen != nullptr) {
                *currentScreen = 4;
            }
            return DetailScreenResult::BACK;
        }
    }

    float rightColumnX = navBarX + 820;
    Rectangle dateSelectorRect = { rightColumnX + 20, 675, 180, 36 };

    if (calendarDropdownOpen) {
        for (int i = 0; i < 3; i++) {
            Rectangle optionRect = { rightColumnX + 20, 711 + (i * 30), 180, 30 };
            if (CheckCollisionPointRec(mousePos, optionRect)) {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedDateText = g_liveDates[i].c_str(); 
                    calendarDropdownOpen = false;
                    return DetailScreenResult::NONE;
                }
            }
        }
    }

    if (CheckCollisionPointRec(mousePos, dateSelectorRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            calendarDropdownOpen = !calendarDropdownOpen;
        }
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        calendarDropdownOpen = false;
    }

    for (int i = 0; i < 6; i++) {
        float row = i / 3;
        float col = i % 3;
        Rectangle timeBoxRect = { (rightColumnX + 20) + (col * 92), 737 + (row * 52), 82, 44 };
        if (CheckCollisionPointRec(mousePos, timeBoxRect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedShowtimeIndex = i;
            }
        }
    }

    float centerColumnX = navBarX + 410;
    Rectangle bookBtnRect = { centerColumnX, 745, 340, 65 };
    if (CheckCollisionPointRec(mousePos, bookBtnRect)) {
        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return DetailScreenResult::BOOKING;
        }
    }

    return DetailScreenResult::NONE;
}

void MovieDetails::Draw() {
    if (background.id != 0) {
        DrawTexturePro(background, { 0, 0, (float)background.width, (float)background.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        ClearBackground(Color{ 243, 246, 251, 255 });
    }

    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navBarX = (screenWidth - navWidth) / 2.0f;
    Vector2 mousePos = GetMousePosition();

    Color navIconBlue = Color{ 24, 119, 242, 255 };
    Color darkNavy = Color{ 14, 21, 61, 255 };

    UpdateLiveDateBuffers(); // Keep rendering buffers contextually up-to-date

    Rectangle backLinkRect = { navBarX + 20, 195, 130, 48 };
    bool backHovered = CheckCollisionPointRec(mousePos, backLinkRect);
    Color backBgColor = backHovered ? darkNavy : navIconBlue;

    DrawRectangleRounded(backLinkRect, 0.3f, 6, backBgColor);

    if (customFont.texture.id != 0) {
        Vector2 backTxtSize = MeasureTextEx(customFont, "Back", 22, 1);
        DrawTextEx(customFont, "Back",
            { backLinkRect.x + (backLinkRect.width - backTxtSize.x) / 2.0f, backLinkRect.y + (backLinkRect.height - backTxtSize.y) / 2.0f }, 22, 1, WHITE);
    }

    Rectangle posterRect = { navBarX + 20, 280, 360, 530 };
    if (textureLoaded && posterTexture.id != 0) {
        DrawTexturePro(posterTexture, { 0, 0, (float)posterTexture.width, (float)posterTexture.height },
            posterRect, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        DrawRectangleRounded(posterRect, 0.04f, 6, DARKGRAY);
    }
    DrawRectangleRoundedLines(posterRect, 0.04f, 6, 2, Color{ 220, 225, 238, 255 });

    float centerColumnX = navBarX + 410;
    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, currentMovie.getTitle().c_str(), { centerColumnX, 280 }, 48, 1, darkNavy);

        Rectangle genrePill = { centerColumnX, 345, 120, 36 };
        DrawRectangleRounded(genrePill, 0.3f, 6, Color{ 238, 242, 248, 255 });
        Vector2 genSize = MeasureTextEx(customFont, currentMovie.getGenre().c_str(), 20, 1);
        DrawTextEx(customFont, currentMovie.getGenre().c_str(),
            { genrePill.x + (genrePill.width - genSize.x) / 2.0f, genrePill.y + (genrePill.height - genSize.y) / 2.0f }, 20, 1, Color{ 80, 95, 120, 255 });

        std::string metaRow = "Duration: " + std::to_string(currentMovie.getDuration()) + "m   |   Rated R   |   Year: 2026";
        DrawTextEx(customFont, metaRow.c_str(), { centerColumnX, 405 }, 22, 1, Color{ 95, 110, 135, 255 });

        std::string ratingRaw = "Rating: " + std::to_string(currentMovie.getRating()).substr(0, 3) + " / 10";
        DrawTextEx(customFont, ratingRaw.c_str(), { centerColumnX, 455 }, 26, 1, darkNavy);
        DrawTextEx(customFont, "Community Score", { centerColumnX, 487 }, 17, 1, Color{ 140, 150, 170, 255 });

        DrawTextTextBoxed(customFont, currentMovie.getDescription().c_str(),
            Rectangle{ centerColumnX, 530, 370, 200 }, 22, 1, true, Color{ 60, 75, 90, 255 });
    }

    Rectangle bookBtnRect = { centerColumnX, 745, 340, 65 };
    bool bookHovered = CheckCollisionPointRec(mousePos, bookBtnRect);
    Color bookBtnColor = bookHovered ? Color{ 41, 128, 185, 255 } : navIconBlue;

    DrawRectangleRounded(bookBtnRect, 0.2f, 6, bookBtnColor);

    if (customFont.texture.id != 0) {
        Vector2 btnTxtSize = MeasureTextEx(customFont, "Book Now", 26, 1);
        DrawTextEx(customFont, "Book Now",
            { bookBtnRect.x + (bookBtnRect.width - btnTxtSize.x) / 2.0f, bookBtnRect.y + (bookBtnRect.height - btnTxtSize.y) / 2.0f }, 26, 1, WHITE);
    }

    float rightColumnX = navBarX + 820;
    float cardWidth = navWidth - 840;

    Rectangle detailsCard = { rightColumnX, 280, cardWidth, 320 };
    DrawRectangleRounded(detailsCard, 0.04f, 6, WHITE);
    DrawRectangleRoundedLines(detailsCard, 0.04f, 6, 1, Color{ 230, 235, 245, 255 });

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Details & Cast", { detailsCard.x + 20, detailsCard.y + 20 }, 26, 1, darkNavy);

        float rowY = detailsCard.y + 65;
        const char* detailLabels[] = { "Director", "Live Cast", "Voice Talent", "Language", "Release" };
        std::string detailValues[] = { "M. Chaves", "Sandra B., Ryan R.", "J. Black, A. Awkwafina", "English", "Sept 5, 2025" };

        for (int i = 0; i < 5; i++) {
            DrawTextEx(customFont, detailLabels[i], { detailsCard.x + 20, rowY }, 21, 1, Color{ 130, 140, 160, 255 });
            DrawTextEx(customFont, detailValues[i].c_str(), { detailsCard.x + 155, rowY }, 21, 1, Color{ 30, 40, 60, 255 });
            rowY += 46;
        }
    }

    Rectangle showtimeCardRect = { rightColumnX, 615, cardWidth, 240 };
    DrawRectangleRounded(showtimeCardRect, 0.04f, 6, WHITE);
    DrawRectangleRoundedLines(showtimeCardRect, 0.04f, 6, 1, Color{ 225, 230, 242, 255 });

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Showtimes", { showtimeCardRect.x + 20, showtimeCardRect.y + 18 }, 24, 1, darkNavy);

        Rectangle dateSelectorRect = { showtimeCardRect.x + 20, showtimeCardRect.y + 60, 180, 36 };
        DrawRectangleRounded(dateSelectorRect, 0.2f, 6, Color{ 240, 244, 250, 255 });

        DrawTextEx(customFont, selectedDateText, { dateSelectorRect.x + 15, dateSelectorRect.y + 8 }, 18, 1, darkNavy);
        DrawTextEx(customFont, calendarDropdownOpen ? "^" : "v", { dateSelectorRect.x + 155, dateSelectorRect.y + 8 }, 14, 1, Color{ 80, 95, 120, 255 });
    }

    const char* showtimes[] = { "11:00", "13:30", "16:00", "18:30", "21:00", "23:30" };
    for (int i = 0; i < 6; i++) {
        float row = i / 3;
        float col = i % 3;
        Rectangle timeBoxRect = { (showtimeCardRect.x + 20) + (col * 92), showtimeCardRect.y + 122 + (row * 52), 82, 44 };
        bool isCurrentSelected = (selectedShowtimeIndex == i);

        Color boxBg = isCurrentSelected ? darkNavy : WHITE;
        Color boxTxt = isCurrentSelected ? WHITE : darkNavy;
        Color boxBorder = isCurrentSelected ? darkNavy : Color{ 215, 225, 240, 255 };

        DrawRectangleRounded(timeBoxRect, 0.2f, 6, boxBg);
        DrawRectangleRoundedLines(timeBoxRect, 0.2f, 6, 1, boxBorder);

        if (customFont.texture.id != 0) {
            Vector2 txtSize = MeasureTextEx(customFont, showtimes[i], 18, 1);
            DrawTextEx(customFont, showtimes[i],
                { timeBoxRect.x + (timeBoxRect.width - txtSize.x) / 2.0f, timeBoxRect.y + (timeBoxRect.height - txtSize.y) / 2.0f }, 18, 1, boxTxt);
        }
    }

    Rectangle navBarRect = { navBarX, 20, navWidth, 100 };
    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 38 }, 36, 1, BLACK);

    const char* labels[] = { "Home", "Spots", "Films", "Offers" };
    Texture2D icons[] = { iconHome, iconMap, iconFilms, iconOffers };
    float navItemSpacing = 94.0f;
    float navItemStartX = navBarRect.x + 930;

    for (int i = 0; i < 4; i++) {
        float itemX = navItemStartX + (i * navItemSpacing);
        Color tint = (i == 2) ? navIconBlue : DARKBLUE;

        if (icons[i].id != 0) DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        if (customFont.texture.id != 0) DrawTextEx(customFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    bool profileHovered = CheckCollisionPointRec(mousePos, profileRect);
    if (iconProfile.id != 0) {
        float iconYOffset = isLoggedIn ? -10.0f : 10.0f;
        DrawTextureEx(iconProfile, { profileRect.x - 5, profileRect.y + iconYOffset }, 0.0f, 0.1f, profileHovered ? navIconBlue : DARKBLUE);
    }

    if (isLoggedIn && !userName.empty() && customFont.texture.id != 0) {
        Vector2 textSize = MeasureTextEx(customFont, userName.c_str(), 22.0f, 1);
        DrawTextEx(customFont, userName.c_str(), { profileRect.x + (profileRect.width / 2.0f) - (textSize.x / 2.0f), profileRect.y + profileRect.height + 5.0f }, 22.0f, 1, BLACK);
    }

    if (calendarDropdownOpen) {
        Rectangle dropdownBox = { rightColumnX + 20, 711, 180, 90 };

        DrawRectangleRec(dropdownBox, WHITE);
        DrawRectangleLinesEx(dropdownBox, 1, Color{ 200, 210, 225, 255 });

        for (int i = 0; i < 3; i++) {
            Rectangle optionRect = { dropdownBox.x, dropdownBox.y + (i * 30), dropdownBox.width, 30 };
            if (CheckCollisionPointRec(mousePos, optionRect)) {
                DrawRectangleRec(optionRect, Color{ 235, 241, 250, 255 });
            }
            if (customFont.texture.id != 0) {
                DrawTextEx(customFont, g_liveDates[i].c_str(), { optionRect.x + 12, optionRect.y + 6 }, 16, 1, darkNavy);
            }
        }
    }
}

void MovieDetails::DrawTextTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color selectColor) {
    if (text == nullptr || strlen(text) == 0) return;

    float lineSpacing = fontSize * 1.35f;
    float textOffsetY = 0;

    std::stringstream ss(text);
    std::string paragraph;

    while (std::getline(ss, paragraph, '\n')) {
        std::stringstream wordStream(paragraph);
        std::string word;
        std::string currentLine = "";

        while (wordStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            Vector2 size = MeasureTextEx(font, testLine.c_str(), fontSize, spacing);

            if (wordWrap && size.x > rec.width && !currentLine.empty()) {
                DrawTextEx(font, currentLine.c_str(), Vector2{ rec.x, rec.y + textOffsetY }, fontSize, spacing, selectColor);
                textOffsetY += lineSpacing;
                currentLine = word;
            }
            else {
                currentLine = testLine;
            }
            if (textOffsetY + lineSpacing > rec.height) break;
        }

        if (!currentLine.empty() && textOffsetY + lineSpacing <= rec.height) {
            DrawTextEx(font, currentLine.c_str(), Vector2{ rec.x, rec.y + textOffsetY }, fontSize, spacing, selectColor);
            textOffsetY += lineSpacing;
        }
    }
}