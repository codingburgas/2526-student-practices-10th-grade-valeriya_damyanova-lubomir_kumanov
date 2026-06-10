#include "DataSeeder.h"
#include <vector>

void DataSeeder::SeedIfEmpty(MovieRepository& repo)
{
    if (!repo.getAllMovies().empty()) {
        return;
    }

    std::vector<Movie> defaultMovies = {

        Movie(0, "Avengers: Infinity War", "assets/action/avengers.jpg", "Action", 8.4f),
        Movie(0, "Fast and Furious", "assets/action/fast and furious.jpg", "Action", 6.8f),
        Movie(0, "Gladiator", "assets/action/gladiator.png", "Action", 8.5f),
        Movie(0, "Indiana Jones", "assets/action/indiana jones.jpg", "Action", 8.4f),
        Movie(0, "Mad Max", "assets/action/mad max.jpg", "Action", 8.1f),
        Movie(0, "Mission Impossible", "assets/action/mission impossible.png", "Action", 7.8f),
        Movie(0, "Spider-Man 2", "assets/action/spider-man 2.jpg", "Action", 7.4f),
        Movie(0, "Transformers", "assets/action/transformers.jpg", "Action", 7.1f),

        Movie(0, "Avatar", "assets/adventure/avatar.jpg", "Adventure", 7.9f),
        Movie(0, "Dungeons & Dragons", "assets/adventure/d&d.jpg", "Adventure", 7.3f),
        Movie(0, "Furiosa", "assets/adventure/furiosa.jpg", "Adventure", 7.6f),
        Movie(0, "The Hobbit", "assets/adventure/hobit.jpg", "Adventure", 7.8f),
        Movie(0, "Jumanji", "assets/adventure/jumaji.jpg", "Adventure", 7.0f),
        Movie(0, "Jungle", "assets/adventure/jungle.jpg", "Adventure", 6.7f),
        Movie(0, "The Lion King", "assets/adventure/LionKing.png", "Adventure", 8.5f),
        Movie(0, "The Lost City", "assets/adventure/the lost city.jpg", "Adventure", 6.1f),

        Movie(0, "Elemental", "assets/animation/elemental.jpg", "Animation", 7.0f),
        Movie(0, "Encanto", "assets/animation/encanto.jpg", "Animation", 7.2f),
        Movie(0, "Frozen 2", "assets/animation/frozen 2.jpeg", "Animation", 6.8f),
        Movie(0, "Ice Age", "assets/animation/ice age.jpg", "Animation", 7.5f),
        Movie(0, "Inside Out 2", "assets/animation/inside out 2.jpg", "Animation", 7.9f),
        Movie(0, "Moana", "assets/animation/moana.jpeg", "Animation", 7.6f),
        Movie(0, "The Lorax", "assets/animation/TheLorax.png", "Animation", 6.4f),
        Movie(0, "Zootopia 2", "assets/animation/Zootopia2.png", "Animation", 8.0f),

        Movie(0, "Accepted", "assets/comedy/Accepted.jpg", "Comedy", 6.4f),
        Movie(0, "Alvin and the Chipmunks", "assets/comedy/Alvin.png", "Comedy", 5.2f),
        Movie(0, "Bad Trip", "assets/comedy/bad trip.jpg", "Comedy", 6.5f),
        Movie(0, "Balls Up", "assets/comedy/balls up.jpeg", "Comedy", 6.0f),
        Movie(0, "Barbie", "assets/comedy/barbie.jpg", "Comedy", 6.9f),
        Movie(0, "Booksmart", "assets/comedy/booksmart.jpg", "Comedy", 7.1f),
        Movie(0, "Bridesmaids", "assets/comedy/bridesmaids.jpg", "Comedy", 6.9f),
        Movie(0, "Deadpool", "assets/comedy/deadpool.jpg", "Comedy", 8.0f),
       
        Movie(0, "A Complete Unknown", "assets/drama/A complete unknown.jpg", "Drama", 7.3f),
        Movie(0, "Jay Kelly", "assets/drama/jay kelly.jpg", "Drama", 6.8f),
        Movie(0, "Manchester by the Sea", "assets/drama/Manchester by the sea.jpg", "Drama", 7.8f),
        Movie(0, "Past Lives", "assets/drama/Past lives.jpg", "Drama", 7.9f),
        Movie(0, "Steve", "assets/drama/steve.jpg", "Drama", 7.0f),
        Movie(0, "The Father", "assets/drama/The father.webp", "Drama", 8.2f),
        Movie(0, "The Social Network", "assets/drama/The social network.jpg", "Drama", 7.8f),
        Movie(0, "Whiplash", "assets/drama/Whiplash.jpg", "Drama", 8.5f),

        Movie(0, "Dora and the Lost City of Gold", "assets/family/Dora - The lost city of gold.jpg", "Family", 6.1f),
        Movie(0, "Home Alone", "assets/family/Home alone.jpg", "Family", 7.7f),
        Movie(0, "Instant Family", "assets/family/Instant family.png", "Family", 7.3f),
        Movie(0, "Paddington", "assets/family/Paddington.png", "Family", 7.8f),
        Movie(0, "The Parent Trap", "assets/family/The parent trap.jpg", "Family", 6.6f),
        Movie(0, "Willow", "assets/family/Willow.jpg", "Family", 7.2f),
        Movie(0, "Wonder", "assets/family/Wonder.png", "Family", 8.0f),
        Movie(0, "Yes Day", "assets/family/Yes day.png", "Family", 6.0f),

        Movie(0, "Annabelle", "assets/horror/annabelle.webp", "Horror", 5.4f),
        Movie(0, "It", "assets/horror/It.png", "Horror", 7.3f),
        Movie(0, "Jaws", "assets/horror/jaw.jpg", "Horror", 8.1f),
        Movie(0, "Misery", "assets/horror/misery.jpg", "Horror", 7.8f),
        Movie(0, "Smile", "assets/horror/Smile.png", "Horror", 6.5f),
        Movie(0, "The Ritual", "assets/horror/the ritual.jpg", "Horror", 6.3f),
        Movie(0, "The Village", "assets/horror/the village.webp", "Horror", 6.6f),
        Movie(0, "The Conjuring 4", "assets/horror/theconjuring.png", "Horror", 6.7f),

        Movie(0, "Cinderella", "assets/romance/Cinderella.jpg", "Romance", 6.9f),
        Movie(0, "Purple Hearts", "assets/romance/Purple hearts.jpg", "Romance", 6.7f),
        Movie(0, "The Drama", "assets/romance/The drama.jpg", "Romance", 7.0f),
        Movie(0, "The Idea of You", "assets/romance/The idea of you.jpg", "Romance", 6.4f),
        Movie(0, "The Longest Ride", "assets/romance/The longest ride.jpg", "Romance", 7.0f),
        Movie(0, "The Notebook", "assets/romance/The notebook.jpg", "Romance", 7.8f),
        Movie(0, "The Proposal", "assets/romance/The proposal.jpg", "Romance", 6.7f),
        Movie(0, "Titanic", "assets/romance/Titanic.png", "Romance", 7.9f),

        Movie(0, "Alien", "assets/sci-fi/Alien.jpg", "Sci-Fi", 8.5f),
        Movie(0, "Arrival", "assets/sci-fi/Arrival.jpg", "Sci-Fi", 7.9f),
        Movie(0, "Harry Potter and the Order of the Phoenix", "assets/sci-fi/Harry Potter and the orden of the phoenix.jpg", "Sci-Fi", 7.5f),
        Movie(0, "Interstellar", "assets/sci-fi/Interstellar.jpg", "Sci-Fi", 8.7f),
        Movie(0, "Oppenheimer", "assets/sci-fi/Oppenheimer.jpg", "Sci-Fi", 8.3f),
        Movie(0, "Percy Jackson and the Olympians", "assets/sci-fi/Percy Jackson and the Olimpians.jpg", "Sci-Fi", 5.9f),
        Movie(0, "Star Wars: The Last Jedi", "assets/sci-fi/Star Wars - The last Jedi.jpg", "Sci-Fi", 6.9f),
        Movie(0, "The Matrix", "assets/sci-fi/The matrix.jpg", "Sci-Fi", 8.7f)
    };

    for (const auto& movie : defaultMovies) {
        repo.addMovie(movie);
    }
}