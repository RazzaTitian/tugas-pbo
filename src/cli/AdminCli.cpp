#include "cli/AdminCli.hpp"

#include <iostream>
#include <limits>

AdminCli::AdminCli(
    AdminService& adminService,
    BookService& bookService,
    MemberService& memberService,
    LoanService& loanService
)
    : adminService_(adminService),
      bookService_(bookService),
      memberService_(memberService),
      loanService_(loanService) {}

void AdminCli::run() {
    std::cout << "====================================\n";
    std::cout << " UGM Library - Admin Console v1.0\n";
    std::cout << "====================================\n\n";

    if (!login()) {
        std::cout << "\nToo many failed attempts. Access denied.\n";
        return;
    }

    showMainMenu();
}

bool AdminCli::login() {
    constexpr int maxAttempts = 3;

    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        std::string username;
        std::string password;

        std::cout << "Username: ";
        std::getline(std::cin, username);

        std::cout << "Password: ";
        std::getline(std::cin, password);

        if (adminService_.authenticate(username, password)) {
            std::cout << "\n[OK] Login successful. Welcome, " << username << ".\n\n";
            return true;
        }

        std::cout << "[ERROR] Invalid username or password. Attempt "
                  << attempt << "/" << maxAttempts << ".\n\n";
    }

    return false;
}

void AdminCli::showMainMenu() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Main Menu ==========\n";
        std::cout << "[1] Manage books\n";
        std::cout << "[2] Manage members\n";
        std::cout << "[3] Loan workflow\n";
        std::cout << "[4] View reports\n";
        std::cout << "[Q] Quit\n";
        std::cout << "Choice: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "\nBook management coming next.\n";
            waitForEnter();
        } else if (choice == "2") {
            std::cout << "\nMember management coming next.\n";
            waitForEnter();
        } else if (choice == "3") {
            std::cout << "\nLoan workflow coming next.\n";
            waitForEnter();
        } else if (choice == "4") {
            std::cout << "\nReports coming next.\n";
            waitForEnter();
        } else if (choice == "Q" || choice == "q") {
            running = false;
        } else {
            std::cout << "\nInvalid choice.\n";
            waitForEnter();
        }
    }

    std::cout << "\nGoodbye.\n";
}

void AdminCli::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
}