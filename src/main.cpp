#include "repositories/CsvAdminRepository.hpp"
#include "utils/PasswordHasher.hpp"

#include <iostream>
#include <string>

int main() {
    CsvAdminRepository adminRepository("data/admins.csv");

    adminRepository.save(Admin(
        "admin",
        PasswordHasher::hash("admin123"),
        "Main Admin"
    ));

    std::string username;
    std::string password;

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Password: ";
    std::getline(std::cin, password);

    auto adminOpt = adminRepository.findById(username);

    if (!adminOpt.has_value()) {
        std::cout << "Login failed\n";
        return 0;
    }

    Admin admin = adminOpt.value();

    if (admin.login(username, PasswordHasher::hash(password))) {
        std::cout << "Login OK\n";
    } else {
        std::cout << "Login failed\n";
    }

    return 0;
}