#ifndef OFFERS_H
#define OFFERS_H

#include "raylib.h"
#include <string>
#include <vector>

struct CinemaOffer {
    std::string title;
    std::string description;
    std::string code;
    std::string discountTag;
    std::string validity;
    bool isActive;
};

class Offers {
public:
    Offers();
    ~Offers();

    void Update();
    void Draw();
    void SetScreenPointer(int* screen);
    void SetUserData(bool loggedIn, const std::string& name, bool isAdmin);

    int* currentScreen;

private:
    void InitializeData();
    void Unload();
    void DrawNavigationBar();
    void DrawSearchBarAndFilters();
    void DrawOfferCards(float& currentY, Vector2 mousePos);
    void DrawScrollbar();

    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Texture2D iconLocationMarker;
    Texture2D iconTicket;
    Texture2D iconGift;
    Texture2D iconStar;

    Font uiFont;

    float scrollOffset;
    float maxScroll;
    int activeIndex;
    int activeCityIndex;

    bool isLoggedIn;
    bool isAdmin;
    std::string userName;

    std::vector<std::string> cities;
    std::vector<CinemaOffer> promoOffers;
};

#endif 