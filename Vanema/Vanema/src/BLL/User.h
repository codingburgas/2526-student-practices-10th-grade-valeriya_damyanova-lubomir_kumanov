#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole {
    STANDARD,
    ADMIN
};

class User {
private:
    int id;
    std::string username;
    std::string email;
    std::string passwordHash; 
    std::string name;
    UserRole role;

public:
    User();
    User(int id, const std::string& username, const std::string& email,
        const std::string& passwordHash, const std::string& name, UserRole role);

    int getId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getPasswordHash() const;
    std::string getName() const;
    UserRole getRole() const;
    std::string getRoleAsString() const;

    void setId(int id);
    void setUsername(const std::string& username);
    void setEmail(const std::string& email);
    void setPasswordHash(const std::string& passwordHash);
    void setName(const std::string& name);
    void setRole(UserRole role);
};

#endif