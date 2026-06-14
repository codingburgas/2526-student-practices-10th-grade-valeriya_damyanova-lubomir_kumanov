#include "Spots.h"
#include <stdio.h>
#include <string.h>

Spots::Spots() {
    logo = LoadTexture("assets/logo.png");
    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");
    iconLocationMarker = LoadTexture("assets/icon_map.png");
    iconHalls = LoadTexture("assets/icon_halls.png");
    icon3D = LoadTexture("assets/icon_3d.png");
    iconAudio = LoadTexture("assets/icon_audio.png");
    iconBeach = LoadTexture("assets/icon_beach.png"); // Note: You can change this asset path to a car icon later if you wish!
    iconPool = LoadTexture("assets/icon_pool.png");
    iconSofa = LoadTexture("assets/icon_sofa.png");

    customFont = LoadFont("assets/fonts/PlayfairDisplay-Medium.ttf");

    scrollOffset = 0.0f;
    maxScroll = 1200.0f;
    activeIndex = 1;
    activeCityIndex = 0;

    memset(searchBuffer, 0, sizeof(searchBuffer));
    searchLetterCount = 0;

    currentScreen = nullptr;
    isLoggedIn = false;
    isAdmin = false;  // Initialized to false
    userName = "";

    InitializeData();
}

Spots::~Spots() {
    Unload();
}

void Spots::InitializeData() {
    cities = { "All Locations", "Burgas", "Nessebar", "Pomorie", "Sozopol", "Primorsko", "Sunny Beach", "Aheloy", "Kableshkovo" };

    locations = {
        { "Burgas Center", "ul. Aleksandrovska 100, Burgas", 11, true, true },
        { "Nessebar", "ul. Han Krum 12, Nessebar", 5, true, true },
        { "Pomorie", "ul. Otec Paisii 8, Pomorie", 4, true, true },
        { "Sozopol", "ul. Republikanska 15, Sozopol", 4, true, true },
        { "Primorsko", "ul. Treti mart 6, Primorsko", 4, true, true },
        { "Sunny Beach", "k.k. Sunny Beach, zona Zapad", 6, true, true },
        { "Aheloy", "ul. Cherno more 9, Aheloy", 3, true, true },
        { "Kableshkovo", "ul. Mladezhka 3, Kableshkovo", 2, true, true }
    };

    experiences = {
        { "Vanema - Drive In", "Retro outdoor cinema experience from your vehicle.\nMovies, car audio tuning, and stars.", "Car Cinema", "Big Screen", "Snacks & Drinks" },
        { "Vanema - By the Pool", "Relax by the pool and enjoy your\nfavorite movies.", "Outdoor", "Big Screen", "Drinks" },
        { "Vanema - Private Hall", "Private cinema hall for you and your friends.\nCatering and comfy sofa zone included.", "Private Hall", "Catering", "Comfy Sofas" }
    };
}

void Spots::SetScreenPointer(int* screen) {
    currentScreen = screen;
}

void Spots::SetUserData(bool loggedIn, const std::string& name, bool isAdmin) {
    this->isLoggedIn = loggedIn;
    this->isAdmin = isAdmin;
    size_t spacePos = name.find(' ');
    this->userName = (spacePos != std::string::npos) ? name.substr(0, spacePos) : name;
}

void Spots::Unload() {
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);
    if (iconLocationMarker.id != 0) UnloadTexture(iconLocationMarker);
    if (iconHalls.id != 0) UnloadTexture(iconHalls);
    if (icon3D.id != 0) UnloadTexture(icon3D);
    if (iconAudio.id != 0) UnloadTexture(iconAudio);
    if (iconBeach.id != 0) UnloadTexture(iconBeach);
    if (iconPool.id != 0) UnloadTexture(iconPool);
    if (iconSofa.id != 0) UnloadTexture(iconSofa);
    if (customFont.texture.id != 0) UnloadFont(customFont);
}

void Spots::Update() {
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 45.0f;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
}

void Spots::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    Rectangle navBarRect = { (screenWidth - navWidth) / 2, 20, navWidth, navHeight };

    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);

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
                    if (i == 0) *currentScreen = 2;
                    if (i == 2) *currentScreen = 4;
                }
            }
        }
        if (icons[i].id != 0) DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        if (customFont.texture.id != 0) DrawTextEx(customFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
        *currentScreen = 1;
    }
    if (iconProfile.id != 0) DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + (isLoggedIn ? -5.0f : 10.0f) }, 0.0f, 0.1f, DARKBLUE);
    if (isLoggedIn && !userName.empty() && customFont.texture.id != 0) {
        DrawTextEx(customFont, userName.c_str(), { profileRect.x, profileRect.y + profileRect.height + 5.0f }, 22, 1, BLACK);
    }
}

void Spots::DrawSearchBarAndFilters() {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;
    Vector2 mousePos = GetMousePosition();

    float titleY = 150 - scrollOffset;

    if (customFont.texture.id != 0) {
        DrawTextEx(customFont, "Locations", { startX + 20, titleY }, 42, 1, Color{ 16, 25, 48, 255 });
        DrawTextEx(customFont, "Find a Vanema cinema near you in Burgas region.", { startX + 20, titleY + 50 }, 20, 1, GRAY);
    }

    float chipY = titleY + 110;
    float currentChipX = startX + 20;

    for (size_t i = 0; i < cities.size(); i++) {
        float textWidth = customFont.texture.id != 0 ? MeasureTextEx(customFont, cities[i].c_str(), 19, 1).x : 80;
        float chipWidth = textWidth + 40;
        Rectangle chipRect = { currentChipX, chipY, chipWidth, 42 };

        bool hoverChip = CheckCollisionPointRec(mousePos, chipRect);
        if (hoverChip) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) activeCityIndex = i;
        }

        Color bgCol = (i == activeCityIndex) ? Color{ 16, 25, 48, 255 } : WHITE;
        Color textCol = (i == activeCityIndex) ? WHITE : Color{ 16, 25, 48, 255 };

        DrawRectangleRounded(chipRect, 0.3f, 8, bgCol);
        DrawRectangleRoundedLines(chipRect, 0.3f, 8, 1, LIGHTGRAY);

        if (iconLocationMarker.id != 0 && i > 0) {
            DrawTextureEx(iconLocationMarker, { chipRect.x + 12, chipRect.y + 11 }, 0.0f, 0.04f, textCol);
            if (customFont.texture.id != 0) DrawTextEx(customFont, cities[i].c_str(), { chipRect.x + 35, chipRect.y + 11 }, 19, 1, textCol);
        }
        else {
            if (customFont.texture.id != 0) DrawTextEx(customFont, cities[i].c_str(), { chipRect.x + 20, chipRect.y + 11 }, 19, 1, textCol);
        }

        currentChipX += chipWidth + 15;
    }
}

void Spots::DrawCinemaCards(float& currentY, Vector2 mousePos) {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    currentY += 190;
    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema Cinemas", { startX + 20, currentY }, 26, 1, Color{ 16, 25, 48, 255 });

    currentY += 40;

    const int cardsPerRow = 3;
    float cardGap = 20.0f;

    float cardWidth = (contentWidth - (cardsPerRow - 1) * cardGap) / cardsPerRow;
    float cardHeight = 260;

    for (size_t i = 0; i < locations.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float cx = startX + col * (cardWidth + cardGap);
        float cy = currentY + row * (cardHeight + cardGap);

        Rectangle cardRect = { cx, cy, cardWidth, cardHeight };
        DrawRectangleRounded(cardRect, 0.08f, 10, WHITE);
        DrawRectangleRoundedLines(cardRect, 0.08f, 10, 1, Fade(LIGHTGRAY, 0.6f));

        if (iconLocationMarker.id != 0) DrawTextureEx(iconLocationMarker, { cx + 20, cy + 20 }, 0.0f, 0.07f, Color{ 16, 25, 48, 255 });

        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, "Vanema", { cx + 65, cy + 18 }, 20, 1, GRAY);
            DrawTextEx(customFont, locations[i].name.c_str(), { cx + 65, cy + 42 }, 30, 1, Color{ 16, 25, 48, 255 });
            DrawTextEx(customFont, locations[i].address.c_str(), { cx + 22, cy + 90 }, 21, 1, GRAY);
        }

        float itemY = cy + 135;
        if (iconHalls.id != 0) DrawTextureEx(iconHalls, { cx + 22, itemY }, 0.0f, 0.04f, Color{ 16, 25, 48, 255 });
        char hallsStr[16]; sprintf(hallsStr, "%d Halls", locations[i].halls);
        if (customFont.texture.id != 0) DrawTextEx(customFont, hallsStr, { cx + 48, itemY + 3 }, 20, 1, Color{ 16, 25, 48, 255 });

        if (icon3D.id != 0) DrawTextureEx(icon3D, { cx + 125, itemY }, 0.0f, 0.04f, Color{ 16, 25, 48, 255 });
        if (customFont.texture.id != 0) DrawTextEx(customFont, "3D", { cx + 152, itemY + 3 }, 20, 1, Color{ 16, 25, 48, 255 });

        if (iconAudio.id != 0) DrawTextureEx(iconAudio, { cx + 200, itemY }, 0.0f, 0.04f, Color{ 16, 25, 48, 255 });
        if (customFont.texture.id != 0)     DrawTextEx(customFont, "Dolby Atmos", { cx + 225, itemY + 3 }, 18, 1, Color{ 16, 25, 48, 255 });

        Rectangle btnRect = { cx + 20, cy + 195, cardWidth - 40, 45 };
        bool hoverBtn = CheckCollisionPointRec(mousePos, btnRect);

        DrawRectangleRounded(btnRect, 0.2f, 6, WHITE);
        DrawRectangleRoundedLines(btnRect, 0.2f, 6, 1.5f, Color{ 16, 25, 48, 255 });
        if (customFont.texture.id != 0) {
            DrawTextEx(customFont,
                "View Details",
                { btnRect.x + (btnRect.width / 2) - 60, btnRect.y + 11 },
                22, 1,
                Color{ 16, 25, 48, 255 });
        }

        if (hoverBtn) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            DrawRectangleRounded(btnRect, 0.2f, 6, Fade(LIGHTGRAY, 0.2f));
        }
    }

    int rows = (locations.size() + cardsPerRow - 1) / cardsPerRow;
    currentY += rows * (cardHeight + cardGap) + 10;
}

void Spots::DrawExperiences(float& currentY) {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;
    Vector2 mousePos = GetMousePosition();

    if (customFont.texture.id != 0) DrawTextEx(customFont, "Vanema Experiences", { startX + 20, currentY }, 26, 1, Color{ 16, 25, 48, 255 });
    currentY += 40;

    // Modified layout: 2 items per row, larger size boxes
    const int cardsPerRow = 2;
    float gap = 24.0f;
    float expWidth = (contentWidth - (cardsPerRow - 1) * gap) / cardsPerRow;
    float expHeight = 180; // Expanded box dimensions
    Texture2D expIcons[] = { iconBeach, iconPool, iconSofa };

    // Draw the 3 explicit existing experiences
    for (size_t i = 0; i < experiences.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float ex = startX + col * (expWidth + gap);
        float ey = currentY + row * (expHeight + gap);
        Rectangle expRect = { ex, ey, expWidth, expHeight };

        DrawRectangleRounded(expRect, 0.06f, 10, WHITE);
        DrawRectangleRoundedLines(expRect, 0.06f, 10, 1, Fade(LIGHTGRAY, 0.6f));

        if (expIcons[i].id != 0) DrawTextureEx(expIcons[i], { ex + 30, ey + 45 }, 0.0f, 0.09f, Color{ 16, 25, 48, 255 });

        float textX = ex + 110;
        if (customFont.texture.id != 0) {
            DrawTextEx(customFont, experiences[i].title.c_str(), { textX, ey + 30 }, 24, 1, Color{ 16, 25, 48, 255 });
            DrawTextEx(customFont, experiences[i].description.c_str(), { textX, ey + 65 }, 18, 1, GRAY);
        }

        float tagY = ey + 130;
        float currentTagX = textX;
        const char* tags[] = { experiences[i].tag1.c_str(), experiences[i].tag2.c_str(), experiences[i].tag3.c_str() };

        for (int j = 0; j < 3; j++) {
            if (strlen(tags[j]) == 0) continue;
            float tw = customFont.texture.id != 0 ? MeasureTextEx(customFont, tags[j], 15, 1).x : 50;

            if (customFont.texture.id != 0) DrawTextEx(customFont, tags[j], { currentTagX + 15, tagY + 2 }, 15, 1, Color{ 16, 25, 48, 255 });
            currentTagX += tw + 30;
        }
    }

    // Dynamic Slot Assignment (Index 3 -> Row 1, Col 1)
    int dynamicRow = 3 / cardsPerRow;
    int dynamicCol = 3 % cardsPerRow;
    float slotX = startX + dynamicCol * (expWidth + gap);
    float slotY = currentY + dynamicRow * (expHeight + gap);
    Rectangle slotRect = { slotX, slotY, expWidth, expHeight };

    if (isLoggedIn && isAdmin) {
        // Administrator View: Interactive Create Field Box
        bool hoverSlot = CheckCollisionPointRec(mousePos, slotRect);
        if (hoverSlot) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            DrawRectangleRounded(slotRect, 0.06f, 10, Color{ 230, 240, 255, 255 });
        }
        else {
            DrawRectangleRounded(slotRect, 0.06f, 10, WHITE);
        }
        DrawRectangleRoundedLines(slotRect, 0.06f, 10, 2.0f, Color{ 16, 25, 48, 150 });

        // Centered Plus Symbol and Text Field
        float midX = slotX + (expWidth / 2.0f);
        float midY = slotY + (expHeight / 2.0f);

        DrawCircleV({ midX, midY - 20 }, 24, Color{ 16, 25, 48, 255 });
        DrawLineEx({ midX - 10, midY - 20 }, { midX + 10, midY - 20 }, 3.5f, WHITE);
        DrawLineEx({ midX, midY - 30 }, { midX, midY - 10 }, 3.5f, WHITE);

        if (customFont.texture.id != 0) {
            Vector2 sizeTxt = MeasureTextEx(customFont, "Add Experience", 22, 1);
            DrawTextEx(customFont, "Add Experience", { midX - (sizeTxt.x / 2.0f), midY + 15 }, 22, 1, Color{ 16, 25, 48, 255 });
        }
    }
    else {
        // Standard Account / Guest View: Teaser Coming Soon Card
        DrawRectangleRounded(slotRect, 0.06f, 10, Color{ 245, 247, 250, 255 });
        DrawRectangleRoundedLines(slotRect, 0.06f, 10, 1, Fade(LIGHTGRAY, 0.5f));

        float midX = slotX + (expWidth / 2.0f);
        float midY = slotY + (expHeight / 2.0f);

        if (customFont.texture.id != 0) {
            Vector2 sizeTitle = MeasureTextEx(customFont, "Coming Soon", 26, 1);
            Vector2 sizeDesc = MeasureTextEx(customFont, "We are designing new custom ways to watch cinema.", 16, 1);

            DrawTextEx(customFont, "Coming Soon", { midX - (sizeTitle.x / 2.0f), midY - 25 }, 26, 1, GRAY);
            DrawTextEx(customFont, "We are designing new custom ways to watch cinema.", { midX - (sizeDesc.x / 2.0f), midY + 15 }, 16, 1, MAGENTA);
        }
    }

    // 2 Rows calculated manually for final tracking layout shift
    currentY += (2 * expHeight) + gap + 40;
}

void Spots::DrawScrollbar() {
    int screenWidth = GetScreenWidth();
    float sbHeight = 500;
    float sbWidth = 8;
    float sbX = screenWidth - 20;
    float sbY = 150;

    DrawRectangleRounded({ sbX, sbY, sbWidth, sbHeight }, 0.5f, 8, Fade(LIGHTGRAY, 0.3f));
    float thumbHeight = 120;
    float thumbY = sbY;
    if (maxScroll > 0) thumbY = sbY + (scrollOffset / maxScroll) * (sbHeight - thumbHeight);

    DrawRectangleRounded({ sbX, thumbY, sbWidth, thumbHeight }, 0.5f, 8, Color{ 16, 25, 48, 255 });
}

void Spots::Draw() {
    ClearBackground(Color{ 238, 244, 253, 255 });

    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    BeginScissorMode((int)startX, 130, (int)contentWidth, GetScreenHeight() - 130);

    float trackingY = 150 - scrollOffset;
    DrawSearchBarAndFilters();
    DrawCinemaCards(trackingY, mousePos);
    DrawExperiences(trackingY);

    EndScissorMode();

    DrawScrollbar();
    DrawNavigationBar();
}