#include "DataSeeder.h"
#include <vector>

void DataSeeder::SeedIfEmpty(MovieRepository& repo)
{
    if (!repo.getAllMovies().empty()) {
        return;
    }

    std::vector<Movie> defaultMovies = {

        Movie(0, "Avengers: Infinity War", "assets/action/avengers.png", "Action", 8.4f),
        Movie(0, "Fast and Furious", "assets/action/fast and furious.png", "Action", 6.8f),
        Movie(0, "Gladiator", "assets/action/gladiator.png", "Action", 8.5f),
        Movie(0, "Indiana Jones", "assets/action/indiana jones.png", "Action", 8.4f),
        Movie(0, "Mad Max", "assets/action/mad max.png", "Action", 8.1f),
        Movie(0, "Mission Impossible", "assets/action/mission impossible.png", "Action", 7.8f),
        Movie(0, "Spider-Man 2", "assets/action/spider-man 2.png", "Action", 7.4f),
        Movie(0, "Transformers", "assets/action/transformers.png", "Action", 7.1f),

        Movie(0, "Avatar", "assets/adventure/avatar.png", "Adventure", 7.9f),
        Movie(0, "Dungeons & Dragons", "assets/adventure/d&d.png", "Adventure", 7.3f),
        Movie(0, "Furiosa", "assets/adventure/furiosa.png", "Adventure", 7.6f),
        Movie(0, "The Hobbit", "assets/adventure/hobit.png", "Adventure", 7.8f),
        Movie(0, "Jumanji", "assets/adventure/jumaji.png", "Adventure", 7.0f),
        Movie(0, "Jungle", "assets/adventure/jungle.png", "Adventure", 6.7f),
        Movie(0, "The Lion King", "assets/adventure/LionKing.png", "Adventure", 8.5f),
        Movie(0, "The Lost City", "assets/adventure/the lost city.png", "Adventure", 6.1f),

        Movie(0, "Elemental", "assets/animation/elemental.png", "Animation", 7.0f),
        Movie(0, "Encanto", "assets/animation/encanto.png", "Animation", 7.2f),
        Movie(0, "Frozen 2", "assets/animation/frozen 2.png", "Animation", 6.8f),
        Movie(0, "Ice Age", "assets/animation/ice age.png", "Animation", 7.5f),
        Movie(0, "Inside Out 2", "assets/animation/inside out 2.png", "Animation", 7.9f),
        Movie(0, "Moana", "assets/animation/moana.png", "Animation", 7.6f),
        Movie(0, "The Lorax", "assets/animation/TheLorax.png", "Animation", 6.4f),
        Movie(0, "Zootopia 2", "assets/animation/Zootopia2.png", "Animation", 8.0f),

        Movie(0, "Accepted", "assets/comedy/Accepted.png", "Comedy", 6.4f),
        Movie(0, "Alvin and the Chipmunks", "assets/comedy/Alvin.png", "Comedy", 5.2f),
        Movie(0, "Bad Trip", "assets/comedy/bad trip.png", "Comedy", 6.5f),
        Movie(0, "Balls Up", "assets/comedy/balls up.png", "Comedy", 6.0f),
        Movie(0, "Barbie", "assets/comedy/barbie.png", "Comedy", 6.9f),
        Movie(0, "Booksmart", "assets/comedy/booksmart.png", "Comedy", 7.1f),
        Movie(0, "Bridesmaids", "assets/comedy/bridesmaids.png", "Comedy", 6.9f),
        Movie(0, "Deadpool", "assets/comedy/deadpool.png", "Comedy", 8.0f),
       
        Movie(0, "A Complete Unknown", "assets/drama/A complete unknown.png", "Drama", 7.3f),
        Movie(0, "Jay Kelly", "assets/drama/jay kelly.png", "Drama", 6.8f),
        Movie(0, "Manchester by the Sea", "assets/drama/Manchester by the sea.png", "Drama", 7.8f),
        Movie(0, "Past Lives", "assets/drama/Past lives.png", "Drama", 7.9f),
        Movie(0, "Steve", "assets/drama/steve.png", "Drama", 7.0f),
        Movie(0, "The Father", "assets/drama/The father.png", "Drama", 8.2f),
        Movie(0, "The Social Network", "assets/drama/The social network.png", "Drama", 7.8f),
        Movie(0, "Whiplash", "assets/drama/Whiplash.png", "Drama", 8.5f),

        Movie(0, "Dora and the Lost City of Gold", "assets/family/Dora - The lost city of gold.png", "Family", 6.1f),
        Movie(0, "Home Alone", "assets/family/Home alone.png", "Family", 7.7f),
        Movie(0, "Instant Family", "assets/family/Instant family.png", "Family", 7.3f),
        Movie(0, "Paddington", "assets/family/Paddington.png", "Family", 7.8f),
        Movie(0, "The Parent Trap", "assets/family/The parent trap.png", "Family", 6.6f),
        Movie(0, "Willow", "assets/family/Willow.png", "Family", 7.2f),
        Movie(0, "Wonder", "assets/family/Wonder.png", "Family", 8.0f),
        Movie(0, "Yes Day", "assets/family/Yes day.png", "Family", 6.0f),

        Movie(0, "Annabelle", "assets/horror/annabelle.png", "Horror", 5.4f),
        Movie(0, "It", "assets/horror/It.png", "Horror", 7.3f),
        Movie(0, "Jaws", "assets/horror/jaw.png", "Horror", 8.1f),
        Movie(0, "Misery", "assets/horror/misery.png", "Horror", 7.8f),
        Movie(0, "Smile", "assets/horror/Smile.png", "Horror", 6.5f),
        Movie(0, "The Ritual", "assets/horror/the ritual.png", "Horror", 6.3f),
        Movie(0, "The Village", "assets/horror/the village.png", "Horror", 6.6f),
        Movie(0, "The Conjuring 4", "assets/horror/theconjuring.png", "Horror", 6.7f),

        Movie(0, "Cinderella", "assets/romance/Cinderella.png", "Romance", 6.9f),
        Movie(0, "Purple Hearts", "assets/romance/Purple hearts.png", "Romance", 6.7f),
        Movie(0, "The Drama", "assets/romance/The drama.png", "Romance", 7.0f),
        Movie(0, "The Idea of You", "assets/romance/The idea of you.png", "Romance", 6.4f),
        Movie(0, "The Longest Ride", "assets/romance/The longest ride.png", "Romance", 7.0f),
        Movie(0, "The Notebook", "assets/romance/The notebook.png", "Romance", 7.8f),
        Movie(0, "The Proposal", "assets/romance/The proposal.png", "Romance", 6.7f),
        Movie(0, "Titanic", "assets/romance/Titanic.png", "Romance", 7.9f),

        Movie(0, "Alien", "assets/sci-fi/Alien.png", "Sci-Fi", 8.5f),
        Movie(0, "Arrival", "assets/sci-fi/Arrival.png", "Sci-Fi", 7.9f),
        Movie(0, "Harry Potter 5", "assets/sci-fi/Harry Potter and the orden of the phoenix.png", "Sci-Fi", 7.5f),
        Movie(0, "Interstellar", "assets/sci-fi/Intersteller.png", "Sci-Fi", 8.7f),
        Movie(0, "Oppenheimer", "assets/sci-fi/Oppenheimer.png", "Sci-Fi", 8.3f),
        Movie(0, "Percy Jackson and the Olympians", "assets/sci-fi/Percy Jackson and the Olimpians.png", "Sci-Fi", 5.9f),
        Movie(0, "Star Wars: The Last Jedi", "assets/sci-fi/Star Wars - The last Jedi.png", "Sci-Fi", 6.9f),
        Movie(0, "The Matrix", "assets/sci-fi/The matrix.png", "Sci-Fi", 8.7f)
    };

    for (const auto& movie : defaultMovies) {
        repo.addMovie(movie);
    }
}