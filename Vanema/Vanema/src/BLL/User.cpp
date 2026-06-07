#include "User.h"

User::User() : id(0), username(""), email(""), passwordHash(""), name(""), role(UserRole::STANDARD) {}

User::User(int id, const std::string& username, const std::string& email,
    const std::string& passwordHash, const std::string& name, UserRole role)
    : id(id), username(username), email(email), passwordHash(passwordHash), name(name), role(role) {}

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getEmail() const { return email; }
std::string User::getPasswordHash() const { return passwordHash; }
std::string User::getName() const { return name; }
UserRole User::getRole() const { return role; }

std::string User::getRoleAsString() const {
    return (role == UserRole::ADMIN) ? "Admin" : "Standard";
}

void User::setId(int id) { this->id = id; }
void User::setUsername(const std::string& username) { this->username = username; }
void User::setEmail(const std::string& email) { this->email = email; }
void User::setPasswordHash(const std::string& passwordHash) { this->passwordHash = passwordHash; }
void User::setName(const std::string& name) { this->name = name; }
void User::setRole(UserRole role) { this->role = role; }