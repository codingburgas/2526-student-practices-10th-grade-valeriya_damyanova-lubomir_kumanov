#include "Offers.h"
#include <stdio.h>
#include <string.h>
#include <sstream>

Offers::Offers() {
    logo = LoadTexture("assets/logo.png");
    iconHome = LoadTexture("assets/icon_home.png");
    iconMap = LoadTexture("assets/icon_map.png");
    iconFilms = LoadTexture("assets/icon_films.png");
    iconOffers = LoadTexture("assets/icon_offers.png");
    iconProfile = LoadTexture("assets/icon_profile.png");
    iconLocationMarker = LoadTexture("assets/icon_map.png");

    iconTicket = LoadTexture("assets/icon_ticket.png");       
    iconGift = LoadTexture("assets/icon_gift.png");
    iconStar = LoadTexture("assets/icon_star.png");

    uiFont = LoadFont("assets/fonts/Roboto-Medium.ttf");

    scrollOffset = 0.0f;
    maxScroll = 800.0f;
    activeIndex = 3; 
    activeCityIndex = 0;

    currentScreen = nullptr;
    isLoggedIn = false;
    isAdmin = false;
    userName = "";

    InitializeData();
}

Offers::~Offers() {
    Unload();
}

void Offers::InitializeData() {
    cities = { "All Locations", "Burgas", "Nessebar", "Pomorie", "Sozopol", "Primorsko", "Sunny Beach", "Aheloy", "Kableshkovo" };

    promoOffers = {
        { "Combo Madness", "Get large popcorn, two soft drinks, and crispy nachos\nat a special package discount rate.", "COMBO30", "30% OFF", "Valid until June 30", true },
        { "Student Special", "Show your valid Student ID card at any ticket desk\nand claim your discount ticket entry.", "STUDENT20", "20% OFF", "Every Mon - Thu", true },
        { "Family Weekend Matinee", "Four tickets with matching medium snacks pack included\nfor morning animation features.", "FAMILYPACK", "Save 25 BGN", "Sat & Sun mornings", true },
        { "Late Night Drive-In Perks", "Get complimentary hot drinks or milkshakes when booking\na premium vehicle space for double-features.", "LATEVANEMA", "Free Drinks", "After 22:00 Only", true },
        { "VIP Private Zone Upgrade", "Book a private lounge for 10+ visitors and receive\ncomplimentary premium catering appetizers on us.", "VIPUPGRADE", "Free Catering", "Reservation required", true },
        { "Early Bird Cinema", "Watch any standard IMAX digital screening before 13:00\nfor a flat-rate special price entry.", "EARLYBIRD", "Flat Rate", "Weekdays Only", true }
    };
}

void Offers::SetScreenPointer(int* screen) {
    currentScreen = screen;
}

void Offers::SetUserData(bool loggedIn, const std::string& name, bool isAdmin) {
    this->isLoggedIn = loggedIn;
    this->isAdmin = isAdmin;
    size_t spacePos = name.find(' ');
    this->userName = (spacePos != std::string::npos) ? name.substr(0, spacePos) : name;
}

void Offers::Unload() {
    if (logo.id != 0) UnloadTexture(logo);
    if (iconHome.id != 0) UnloadTexture(iconHome);
    if (iconMap.id != 0) UnloadTexture(iconMap);
    if (iconFilms.id != 0) UnloadTexture(iconFilms);
    if (iconOffers.id != 0) UnloadTexture(iconOffers);
    if (iconProfile.id != 0) UnloadTexture(iconProfile);
    if (iconLocationMarker.id != 0) UnloadTexture(iconLocationMarker);
    if (iconTicket.id != 0) UnloadTexture(iconTicket);
    if (iconGift.id != 0) UnloadTexture(iconGift);
    if (iconStar.id != 0) UnloadTexture(iconStar);
    if (uiFont.texture.id != 0) UnloadFont(uiFont);
}

void Offers::Update() {
    float wheelMove = GetMouseWheelMove();
    if (wheelMove != 0) {
        scrollOffset -= wheelMove * 45.0f;
        if (scrollOffset < 0) scrollOffset = 0;
        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
    }
}

void Offers::DrawNavigationBar() {
    int screenWidth = GetScreenWidth();
    float navWidth = screenWidth * 0.9f;
    float navHeight = 100.0f;
    Rectangle navBarRect = { (screenWidth - navWidth) / 2, 20, navWidth, navHeight };

    DrawRectangleRounded(navBarRect, 0.5f, 10, WHITE);

    if (logo.id != 0) DrawTextureEx(logo, { navBarRect.x - 2, navBarRect.y - 20 }, 0.0f, 0.3f, WHITE);
    if (uiFont.texture.id != 0) DrawTextEx(uiFont, "Vanema", { navBarRect.x + 130, navBarRect.y + 40 }, 34, 1, BLACK);

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
                if (currentScreen != nullptr) {
                    if (i == 0) *currentScreen = 2;
                    if (i == 1) *currentScreen = 3; 
                    if (i == 2) *currentScreen = 4; 
                }
            }
        }
        if (icons[i].id != 0) DrawTextureEx(icons[i], { itemX + 50, navBarRect.y + 5 }, 0.0f, 0.1f, tint);
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, labels[i], { itemX + 58, navBarRect.y + 70 }, 20, 1, BLACK);
    }

    Rectangle profileRect = { navBarRect.x + navWidth - 70, navBarRect.y + 15, 50, 50 };
    if (CheckCollisionPointRec(mousePos, profileRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentScreen != nullptr) {
        *currentScreen = 1;
    }
    if (iconProfile.id != 0) DrawTextureEx(iconProfile, { profileRect.x - 15, profileRect.y + (isLoggedIn ? -5.0f : 10.0f) }, 0.0f, 0.1f, DARKBLUE);
    if (isLoggedIn && !userName.empty() && uiFont.texture.id != 0) {
        DrawTextEx(uiFont, userName.c_str(), { profileRect.x, profileRect.y + profileRect.height + 5.0f }, 22, 1, BLACK);
    }
}

void Offers::DrawSearchBarAndFilters() {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    float titleY = 150 - scrollOffset;

    if (uiFont.texture.id != 0) {
        DrawTextEx(uiFont, "Exclusive Offers", { startX + 20, titleY }, 52, 1, Color{ 16, 25, 48, 255 });
        DrawTextEx(uiFont, "Explore special bundles, promo codes, and limited-time discounts.", { startX + 20, titleY + 65 }, 24, 1, GRAY);
    }

    float chipY = titleY + 135;
    float currentChipX = startX + 20;
    float chipFontSize = 22.0f;
    float chipGap = 8.0f;
    Vector2 mousePos = GetMousePosition();

    for (size_t i = 0; i < cities.size(); i++) {
        float textWidth = uiFont.texture.id != 0 ? MeasureTextEx(uiFont, cities[i].c_str(), chipFontSize, 1).x : 85;
        float chipWidth = textWidth + (i > 0 ? 56 : 30);
        Rectangle chipRect = { currentChipX, chipY, chipWidth, 50 };

        if (CheckCollisionPointRec(mousePos, chipRect)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                activeCityIndex = (int)i;
            }
        }

        Color bgCol = (i == activeCityIndex) ? Color{ 16, 25, 48, 255 } : WHITE;
        Color textCol = (i == activeCityIndex) ? WHITE : Color{ 16, 25, 48, 255 };

        DrawRectangleRounded(chipRect, 0.25f, 8, bgCol);
        DrawRectangleRoundedLines(chipRect, 0.25f, 8, 1.5f, Fade(LIGHTGRAY, 0.8f));

        float textYOffset = chipRect.y + (chipRect.height - chipFontSize) / 2.0f;

        if (iconLocationMarker.id != 0 && i > 0) {
            float iconHeight = iconLocationMarker.height * 0.050f;
            float iconYOffset = chipRect.y + (chipRect.height - iconHeight) / 2.0f;
            DrawTextureEx(iconLocationMarker, { chipRect.x + 10, iconYOffset }, 0.0f, 0.050f, textCol);
            if (uiFont.texture.id != 0) DrawTextEx(uiFont, cities[i].c_str(), { chipRect.x + 44, textYOffset }, chipFontSize, 1, textCol);
        }
        else {
            if (uiFont.texture.id != 0) DrawTextEx(uiFont, cities[i].c_str(), { chipRect.x + 15, textYOffset }, chipFontSize, 1, textCol);
        }

        currentChipX += chipWidth + chipGap;
    }
}

void Offers::DrawOfferCards(float& currentY, Vector2 mousePos) {
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    currentY += 240;
    if (uiFont.texture.id != 0) DrawTextEx(uiFont, "Available Promotions", { startX + 20, currentY }, 32, 1, Color{ 16, 25, 48, 255 });
    currentY += 50;

    const int cardsPerRow = 2;
    float gap = 24.0f;
    float cardWidth = (contentWidth - (cardsPerRow - 1) * gap) / cardsPerRow;
    float cardHeight = 240;

    for (size_t i = 0; i < promoOffers.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;

        float ox = startX + col * (cardWidth + gap);
        float oy = currentY + row * (cardHeight + gap);
        Rectangle cardRect = { ox, oy, cardWidth, cardHeight };

        bool isCardHovered = CheckCollisionPointRec(mousePos, cardRect);
        Color bgCol = isCardHovered ? Color{ 245, 249, 255, 255 } : WHITE;
        if (isCardHovered) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }

        DrawRectangleRounded(cardRect, 0.06f, 10, bgCol);
        DrawRectangleRoundedLines(cardRect, 0.06f, 10, 1.5f, Fade(LIGHTGRAY, 0.8f));

        Texture2D currentIcon = (i % 3 == 0) ? iconTicket : ((i % 3 == 1) ? iconGift : iconStar);
        if (currentIcon.id != 0) {
            DrawTextureEx(currentIcon, { ox + 32, oy + 55 }, 0.0f, 0.12f, Color{ 16, 25, 48, 255 });
        }
        else {
            DrawCircleV({ ox + 65, oy + 85 }, 28, Color{ 230, 240, 255, 255 });
            DrawCircleLines((int)(ox + 65), (int)(oy + 85), 28.0f, Color{ 16, 25, 48, 255 });
        }

        float textX = ox + 145;
        if (uiFont.texture.id != 0) {
            DrawTextEx(uiFont, promoOffers[i].title.c_str(), { textX, oy + 26 }, 32, 1, Color{ 16, 25, 48, 255 });

            std::stringstream ss(promoOffers[i].description);
            std::string line;
            float lineYOffset = oy + 72;
            float customLineHeight = 32.0f;

            while (std::getline(ss, line, '\n')) {
                DrawTextEx(uiFont, line.c_str(), { textX, lineYOffset }, 25, 1, GRAY);
                lineYOffset += customLineHeight;
            }
        }
        float tagY = oy + 182;

        float tw1 = uiFont.texture.id != 0 ? MeasureTextEx(uiFont, promoOffers[i].discountTag.c_str(), 24, 1).x : 80;
        Rectangle tagRect1 = { textX, tagY - 4, tw1 + 24, 38 };
        DrawRectangleRounded(tagRect1, 0.3f, 6, Color{ 16, 25, 48, 255 });
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, promoOffers[i].discountTag.c_str(), { textX + 12, tagY + 3 }, 20, 1, WHITE);

        float startTag2X = textX + tw1 + 38;
        float tw2 = uiFont.texture.id != 0 ? MeasureTextEx(uiFont, promoOffers[i].validity.c_str(), 24, 1).x : 100;
        Rectangle tagRect2 = { startTag2X, tagY - 4, tw2 + 24, 38 };
        DrawRectangleRounded(tagRect2, 0.3f, 6, Color{ 240, 244, 250, 255 });
        DrawRectangleRoundedLines(tagRect2, 0.3f, 6, 1.0f, LIGHTGRAY);
        if (uiFont.texture.id != 0) DrawTextEx(uiFont, promoOffers[i].validity.c_str(), { startTag2X + 12, tagY + 3 }, 20, 1, Color{ 16, 25, 48, 255 });
    }

    int totalRows = (promoOffers.size() + cardsPerRow - 1) / cardsPerRow;
    currentY += totalRows * (cardHeight + gap) + 40;
}

void Offers::DrawScrollbar() {
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

void Offers::Draw() {
    ClearBackground(Color{ 238, 244, 253, 255 });

    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    float contentWidth = screenWidth * 0.9f;
    float startX = (screenWidth - contentWidth) / 2;

    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    BeginScissorMode((int)startX - 4, 130, (int)contentWidth + 8, GetScreenHeight() - 130);

    float trackingY = 150 - scrollOffset;
    DrawSearchBarAndFilters();
    DrawOfferCards(trackingY, mousePos);

    EndScissorMode();

    DrawScrollbar();
    DrawNavigationBar();
}