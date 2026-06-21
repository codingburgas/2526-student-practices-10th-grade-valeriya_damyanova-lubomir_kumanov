#include "DataSeeder.h"
#include <vector>
#include "../DAL/MovieRepository.h"  
#include "../BLL/Movie.h"

void DataSeeder::SeedIfEmpty(MovieRepository& repo)
{
    if (!repo.getAllMovies().empty()) {
        return;
    }

    std::vector<Movie> defaultMovies = {
        // --- ACTION ---
        Movie(0, "Avengers: Infinity War", "assets/action/avengers.png", "Action", 8.4f, "The Avengers must stop Thanos from wiping out half of the universe.", 149),
        Movie(0, "Fast and Furious", "assets/action/fast and furious.png", "Action", 6.8f, "An undercover cop infiltrates a subculture of street racers.", 106),
        Movie(0, "Gladiator", "assets/action/gladiator.png", "Action", 8.5f, "A former Roman General sets out to exact vengeance against the corrupt emperor.", 155),
        Movie(0, "Indiana Jones", "assets/action/indiana jones.png", "Action", 8.4f, "Archaeologist Indiana Jones races against Nazis to find the Ark of the Covenant.", 115),
        Movie(0, "Mad Max", "assets/action/mad max.png", "Action", 8.1f, "In a post-apocalyptic wasteland, a woman rebels against a tyrannical ruler.", 120),
        Movie(0, "Mission Impossible", "assets/action/mission impossible.png", "Action", 7.8f, "An agent is framed for the deaths of his espionage team.", 110),
        Movie(0, "Spider-Man 2", "assets/action/spider-man 2.png", "Action", 7.4f, "Peter Parker struggles to balance his personal life with his superhero duties.", 127),
        Movie(0, "Transformers", "assets/action/transformers.png", "Action", 7.1f, "An ancient struggle between two Cybertronian factions comes to Earth.", 144),

        // --- ADVENTURE ---
        Movie(0, "Avatar", "assets/adventure/avatar.png", "Adventure", 7.9f, "A paraplegic Marine dispatched to the moon Pandora on a unique mission.", 162),
        Movie(0, "Dungeons & Dragons", "assets/adventure/d&d.png", "Adventure", 7.3f, "A charming thief and a band of unlikely adventurers undertake an epic heist.", 134),
        Movie(0, "Furiosa", "assets/adventure/furiosa.png", "Adventure", 7.6f, "The origin story of renegade warrior Furiosa before her team-up with Mad Max.", 148),
        Movie(0, "The Hobbit", "assets/adventure/hobit.png", "Adventure", 7.8f, "A reluctant Hobbit sets out to the Lonely Mountain with a spirited group of dwarves.", 169),
        Movie(0, "Jumanji", "assets/adventure/jumaji.png", "Adventure", 7.0f, "Four teenagers are sucked into a magical video game world.", 119),
        Movie(0, "Jungle", "assets/adventure/jungle.png", "Adventure", 6.7f, "A group of travelers set off into the Amazon rainforest with a mysterious guide.", 115),
        Movie(0, "The Lion King", "assets/adventure/LionKing.png", "Adventure", 8.5f, "A young lion prince flees his kingdom only to learn the true meaning of responsibility.", 88),
        Movie(0, "The Lost City", "assets/adventure/the lost city.png", "Adventure", 6.1f, "A reclusive romance novelist on a book tour gets swept up in a jungle adventure.", 112),

        // --- ANIMATION ---
        Movie(0, "Elemental", "assets/animation/elemental.png", "Animation", 7.0f, "Follows Ember and Wade in a city where fire-, water-, land- and air-residents live together.", 101),
        Movie(0, "Encanto", "assets/animation/encanto.png", "Animation", 7.2f, "A Colombian teenage girl has to face the frustration of being the only member of her family without magical powers.", 102),
        Movie(0, "Frozen 2", "assets/animation/frozen 2.png", "Animation", 6.8f, "Elsa, Anna, Kristoff, Olaf and Sven embark on a journey beyond their homeland Arendelle.", 103),
        Movie(0, "Ice Age", "assets/animation/ice age.png", "Animation", 7.5f, "Set during the Ice Age, a sabertooth tiger, a sloth, and a mammoth find a human baby.", 81),
        Movie(0, "Inside Out 2", "assets/animation/inside out 2.png", "Animation", 7.9f, "Return to the mind of newly minted teenager Riley just as headquarters is undergoing a sudden demolition.", 96),
        Movie(0, "Moana", "assets/animation/moana.png", "Animation", 7.6f, "In Ancient Polynesia, when a terrible curse reaches Moana's island, she answers the Ocean's call.", 107),
        Movie(0, "The Lorax", "assets/animation/TheLorax.png", "Animation", 6.4f, "A 12-year-old boy searches for the one thing that will enable him to win the affection of the girl of his dreams.", 86),
        Movie(0, "Zootopia 2", "assets/animation/Zootopia2.png", "Animation", 8.0f, "Brave bunny cop Judy Hopps and her sharp-witted fox partner Nick Wilde return for a new case.", 105),

        // --- COMEDY ---
        Movie(0, "Accepted", "assets/comedy/Accepted.png", "Comedy", 6.4f, "A high school slacker who's rejected by every school he applies to creates his own institution.", 93),
        Movie(0, "Alvin and the Chipmunks", "assets/comedy/Alvin.png", "Comedy", 5.2f, "Three musical chipmunks are discovered by a struggling songwriter.", 92),
        Movie(0, "Bad Trip", "assets/comedy/bad trip.png", "Comedy", 6.5f, "This hidden-camera comedy follows two best friends on a cross-country road trip full of hilarious pranks.", 86),
        Movie(0, "Balls Up", "assets/comedy/balls up.png", "Comedy", 6.0f, "Two American marketing executives lose their client's money and sponsor a fictional soccer team.", 95),
        Movie(0, "Barbie", "assets/comedy/barbie.png", "Comedy", 6.9f, "Barbie suffers a crisis that leads her to question her world and her existence.", 114),
        Movie(0, "Booksmart", "assets/comedy/booksmart.png", "Comedy", 7.1f, "On the eve of their high school graduation, two academic superstars realize they should have worked less and played more.", 102),
        Movie(0, "Bridesmaids", "assets/comedy/bridesmaids.png", "Comedy", 6.9f, "Competition between the maid of honor and a bridesmaid, over who is the bride's best friend.", 125),
        Movie(0, "Deadpool", "assets/comedy/deadpool.png", "Comedy", 8.0f, "A wisecracking mercenary with accelerated healing powers seeks revenge on the man who disfigured him.", 108),

        // --- DRAMA ---
        Movie(0, "A Complete Unknown", "assets/drama/A complete unknown.png", "Drama", 7.3f, "The rise of Bob Dylan in the 1960s folk music scene.", 115),
        Movie(0, "Jay Kelly", "assets/drama/jay kelly.png", "Drama", 6.8f, "A dramatic look into the life and emotional struggles of Jay Kelly.", 110),
        Movie(0, "Manchester by the Sea", "assets/drama/Manchester by the sea.png", "Drama", 7.8f, "A depressed uncle is asked to take care of his teenage nephew after the boy's father dies.", 137),
        Movie(0, "Past Lives", "assets/drama/Past lives.png", "Drama", 7.9f, "Two deeply connected childhood friends are wrest apart after one's family emigrates from South Korea.", 105),
        Movie(0, "Steve", "assets/drama/steve.png", "Drama", 7.0f, "A tense psychological drama exploring a crucial 24 hours in the life of a school headteacher.", 98),
        Movie(0, "The Father", "assets/drama/The father.png", "Drama", 8.2f, "A man refuses all assistance from his daughter as he ages and begins to lose his grip on reality.", 97),
        Movie(0, "The Social Network", "assets/drama/The social network.png", "Drama", 7.8f, "As Harvard student Mark Zuckerberg creates the social networking site that would become Facebook, he is sued by the twins who claimed he stole their idea.", 120),
        Movie(0, "Whiplash", "assets/drama/Whiplash.png", "Drama", 8.5f, "A promising young drummer enrolls at a cut-throat music conservatory where his dreams of greatness are mentored by an abusive instructor.", 106),

        // --- FAMILY ---
        Movie(0, "Dora and the Lost City of Gold", "assets/family/Dora - The lost city of gold.png", "Family", 6.1f, "Dora, a teenage explorer, leads her friends on an adventure to save her parents and solve the mystery of a lost city.", 102),
        Movie(0, "Home Alone", "assets/family/Home alone.png", "Family", 7.7f, "An eight-year-old troublemaker must protect his house from a pair of burglars when he is accidentally left home alone.", 103),
        Movie(0, "Instant Family", "assets/family/Instant family.png", "Family", 7.3f, "A couple find themselves in over their heads when they foster three children.", 118),
        Movie(0, "Paddington", "assets/family/Paddington.png", "Family", 7.8f, "A young Peruvian bear travels to London in search of a home.", 95),
        Movie(0, "The Parent Trap", "assets/family/The parent trap.png", "Family", 6.6f, "Identical twins Annie and Hallie, separated at birth and each raised by one of their biological parents, discover each other for the first time at summer camp.", 128),
        Movie(0, "Willow", "assets/family/Willow.png", "Family", 7.2f, "A reluctant dwarf farmer plays a critical role in protecting a special baby from a tyrannical queen.", 126),
        Movie(0, "Wonder", "assets/family/Wonder.png", "Family", 8.0f, "Based on the New York Times bestseller, this movie tells the incredibly inspiring and heartwarming story of August Pullman.", 113),
        Movie(0, "Yes Day", "assets/family/Yes day.png", "Family", 6.0f, "A mom and dad who usually say no decide to say yes to their kids' wildest requests with a few ground rules for 24 hours.", 89),

        // --- HORROR ---
        Movie(0, "Annabelle", "assets/horror/annabelle.png", "Horror", 5.4f, "A couple begins to experience terrifying supernatural occurrences involving a vintage doll shortly after their home is invaded by satanic cultists.", 99),
        Movie(0, "It", "assets/horror/It.png", "Horror", 7.3f, "In the summer of 1989, a group of bullied kids band together to destroy a shape-shifting monster.", 135),
        Movie(0, "Jaws", "assets/horror/jaw.png", "Horror", 8.1f, "When a killer shark unleashes chaos on a beach community off Long Island, it's up to a local sheriff, a marine biologist, and an old seafarer to hunt it down.", 124),
        Movie(0, "Misery", "assets/horror/misery.png", "Horror", 7.8f, "After a famous author is rescued from a car crash by a fan of his novels, he realizes that the care he is receiving is only the beginning of a nightmare.", 107),
        Movie(0, "Smile", "assets/horror/Smile.png", "Horror", 6.5f, "After witnessing a bizarre, traumatic incident involving a patient, Dr. Rose Cotter starts experiencing frightening occurrences.", 115),
        Movie(0, "The Ritual", "assets/horror/the ritual.png", "Horror", 6.3f, "A group of college friends reunite for a trip to the forest, but encounter a menacing presence in the woods.", 94),
        Movie(0, "The Village", "assets/horror/the village.png", "Horror", 6.6f, "A series of events tests the beliefs of an isolated town in 1897 Pennsylvania.", 108),
        Movie(0, "The Conjuring 4", "assets/horror/theconjuring.png", "Horror", 6.7f, "Paranormal investigators Ed and Lorraine Warren return to take on one last terrifying supernatural case.", 112),

        // --- ROMANCE ---
        Movie(0, "Cinderella", "assets/romance/Cinderella.png", "Romance", 6.9f, "When her father unexpectedly dies, young Ella finds herself at the mercy of her cruel stepmother and her daughters.", 105),
        Movie(0, "Purple Hearts", "assets/romance/Purple hearts.png", "Romance", 6.7f, "In spite of their many differences, an aspiring singer-songwriter and a Marine agree to marry solely for military benefits.", 122),
        Movie(0, "The Drama", "assets/romance/The drama.png", "Romance", 7.0f, "A complex romantic story following unexpected turns before a couple's big wedding day.", 110),
        Movie(0, "The Idea of You", "assets/romance/The idea of you.png", "Romance", 6.4f, "Solène, a 40-year-old single mom, begins an unexpected romance with 24-year-old Hayes Campbell, the lead singer of the hottest boy band on the planet.", 115),
        Movie(0, "The Longest Ride", "assets/romance/The longest ride.png", "Romance", 7.0f, "The look of a star-crossed love affair between a former bull-riding champion and a college student looking to enter the New York art world.", 128),
        Movie(0, "The Notebook", "assets/romance/The notebook.png", "Romance", 7.8f, "A poor yet passionate young man falls in love with a rich young woman, giving her a sense of freedom, but they are soon separated because of their social differences.", 123),
        Movie(0, "The Proposal", "assets/romance/The proposal.png", "Romance", 6.7f, "A pushy boss forces her young assistant to marry her in order to keep her visa status in the U.S.", 108),
        Movie(0, "Titanic", "assets/romance/Titanic.png", "Romance", 7.9f, "A seventeen-year-old aristocrat falls in love with a kind but poor artist aboard the luxurious, ill-fated R.M.S. Titanic.", 194),

        // --- SCI-FI ---
        Movie(0, "Alien", "assets/sci-fi/Alien.png", "Sci-Fi", 8.5f, "The crew of a commercial spacecraft encounter a deadly lifeform after investigating an unknown transmission.", 117),
        Movie(0, "Arrival", "assets/sci-fi/Arrival.png", "Sci-Fi", 7.9f, "A linguist works with the military to communicate with alien experiences that have appeared across the globe.", 116),
        Movie(0, "Harry Potter 5", "assets/sci-fi/Harry Potter and the orden of the phoenix.png", "Sci-Fi", 7.5f, "With their warning about Lord Voldemort's return scoffed at, Harry and Dumbledore are targeted by the Wizard authorities.", 138),
        Movie(0, "Interstellar", "assets/sci-fi/Intersteller.png", "Sci-Fi", 8.7f, "A team of explorers travel through a wormhole in space in an attempt to ensure humanity's survival.", 169),
        Movie(0, "Oppenheimer", "assets/sci-fi/Oppenheimer.png", "Sci-Fi", 8.3f, "The story of American scientist J. Robert Oppenheimer and his role in the development of the atomic bomb.", 180),
        Movie(0, "Percy Jackson", "assets/sci-fi/Percy Jackson and the Olimpians.png", "Sci-Fi", 5.9f, "A teenager discovers he's the descendant of a Greek god and sets out on an adventure to settle an ongoing battle between the gods.", 118),
        Movie(0, "Star Wars: The Last Jedi", "assets/sci-fi/Star Wars - The last Jedi.png", "Sci-Fi", 6.9f, "The Skywalker saga continues as the heroes of The Force Awakens join the galactic legends in an epic adventure.", 152),
        Movie(0, "The Matrix", "assets/sci-fi/The matrix.png", "Sci-Fi", 8.7f, "When a beautiful stranger leads computer hacker Neo to a forbidding underworld, he discovers the shocking truth--the life he knows is the elaborate deception of an evil cyber-intelligence.", 136)
    };

    for (const auto& movie : defaultMovies) {
        repo.addMovie(movie);
    }
}