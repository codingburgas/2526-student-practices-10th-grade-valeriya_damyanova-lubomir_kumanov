#include "UserSeeder.h"
#include "bcrypt.h" 

void UserSeeder::SeedIfEmpty(UserRepository& repo) {
    if (!repo.getAllUsers().empty()) {
        return; 
    }

    std::string adminHash = Crypto::hashPassword("admin123");
    User admin(0, "admin", "admin@vanema.com", adminHash, "System Administrator", UserRole::ADMIN);
    repo.addUser(admin);

    std::string userPass1 = Crypto::hashPassword("alex99");
    User user1(0, "alex_jones", "alex@mail.com", userPass1, "Alex Jones", UserRole::STANDARD);
    repo.addUser(user1);

    std::string userPass2 = Crypto::hashPassword("cinema_lover");
    User user2(0, "sarah_m", "sarah@cinemafan.com", userPass2, "Sarah Miller", UserRole::STANDARD);
    repo.addUser(user2);

    std::string userPass3 = Crypto::hashPassword("popcorn12");
    User user3(0, "john_doe", "john.doe@domain.com", userPass3, "John Doe", UserRole::STANDARD);
    repo.addUser(user3);
}