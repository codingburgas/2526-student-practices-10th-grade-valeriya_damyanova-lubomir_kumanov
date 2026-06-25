#ifndef ADDMOVIE_H
#define ADDMOVIE_H

#include "raylib.h"
#include <string>
#include "BLL/MovieService.h"

class AddMovie {
public:
    // Public members
    int* currentScreen;
    MovieService* movieService;

    // Constructor & Destructor
    AddMovie();
    ~AddMovie();

    // Public methods
    void Update();
    void Draw();
    void SetUserData(bool loggedIn, const std::string& name, bool admin);
    void ResetForm();

    // Getter and Setter for refresh flag
    bool ShouldRefreshMovies() const;
    void SetRefreshMovies(bool refresh);

private:
    // Textures
    Texture2D background;
    Texture2D logo;
    Texture2D iconHome;
    Texture2D iconMap;
    Texture2D iconFilms;
    Texture2D iconOffers;
    Texture2D iconProfile;
    Font customFont;

    // UI State
    int activeIndex;
    bool isLoggedIn;
    std::string userName;
    bool isAdmin;

    // Scroll
    float scrollYOffset;
    float maxScrollY;

    // Form inputs
    std::string titleInput;
    std::string plotInput;
    std::string yearInput;
    std::string genreInput;
    std::string durationInput;
    std::string languageInput;
    std::string ratingInput;
    std::string posterPathInput;
    int activeField;

    // Transition state tracking
    bool isTransitioning;
    int transitionFrames;

    // Refresh flag to signal when movies need reloading
    bool shouldRefreshMovies;

    // Private helper methods
    void Unload();
    void ConsumeMouseClicks();
    void DrawNavigationBar();
};

#endif // ADDMOVIE_H