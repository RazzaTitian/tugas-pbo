#include "cli/AdminCli.hpp"

#include "models/Book.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

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
        std::string username = readLine("Username: ");
        std::string password = readLine("Password: ");

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

        std::string choice = readLine("Choice: ");

        if (choice == "1") {
            showBookMenu();
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

void AdminCli::showBookMenu() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Book Management ==========\n";
        std::cout << "[1] List books\n";
        std::cout << "[2] Add book\n";
        std::cout << "[3] Search books\n";
        std::cout << "[4] Delete book\n";
        std::cout << "[B] Back\n";

        std::string choice = readLine("Choice: ");

        if (choice == "1") {
            listBooks();
        } else if (choice == "2") {
            addBook();
        } else if (choice == "3") {
            searchBooks();
        } else if (choice == "4") {
            deleteBook();
        } else if (choice == "B" || choice == "b") {
            running = false;
        } else {
            std::cout << "\nInvalid choice.\n";
            waitForEnter();
        }
    }
}

void AdminCli::listBooks() {
    std::cout << "\n========== Books ==========\n";

    std::vector<Book> books = bookService_.listBooks();

    if (books.empty()) {
        std::cout << "No books found.\n";
        waitForEnter();
        return;
    }

    for (const Book& book : books) {
        std::cout << book << '\n';
    }

    waitForEnter();
}

void AdminCli::addBook() {
    std::cout << "\n========== Add Book ==========\n";

    int id = readInt("Book ID: ");
    std::string title = readLine("Title: ");
    std::string author = readLine("Author: ");

    if (title.empty() || author.empty()) {
        std::cout << "\n[ERROR] Title and author cannot be empty.\n";
        waitForEnter();
        return;
    }

    if (bookService_.findBookById(id).has_value()) {
        std::cout << "\n[ERROR] Book ID already exists.\n";
        waitForEnter();
        return;
    }

    Book book(id, title, author, true);

    if (bookService_.addBook(book)) {
        std::cout << "\n[OK] Book added successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to add book.\n";
    }

    waitForEnter();
}

void AdminCli::searchBooks() {
    std::cout << "\n========== Search Books ==========\n";

    std::string keyword = readLine("Keyword: ");

    if (keyword.empty()) {
        std::cout << "\n[ERROR] Keyword cannot be empty.\n";
        waitForEnter();
        return;
    }

    std::vector<Book> books = bookService_.searchBooks(keyword);

    if (books.empty()) {
        std::cout << "\nNo matching books found.\n";
        waitForEnter();
        return;
    }

    std::cout << "\nSearch results:\n";

    for (const Book& book : books) {
        std::cout << book << '\n';
    }

    waitForEnter();
}

void AdminCli::deleteBook() {
    std::cout << "\n========== Delete Book ==========\n";

    int id = readInt("Book ID: ");

    if (!bookService_.findBookById(id).has_value()) {
        std::cout << "\n[ERROR] Book not found.\n";
        waitForEnter();
        return;
    }

    if (bookService_.deleteBook(id)) {
        std::cout << "\n[OK] Book deleted successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to delete book. The book may have an active loan.\n";
    }

    waitForEnter();
}

int AdminCli::readInt(const std::string& prompt) const {
    while (true) {
        std::string input = readLine(prompt);

        try {
            std::size_t processedCharacters = 0;
            int value = std::stoi(input, &processedCharacters);

            if (processedCharacters == input.length()) {
                return value;
            }
        } catch (const std::invalid_argument&) {
        } catch (const std::out_of_range&) {
        }

        std::cout << "[ERROR] Please enter a valid number.\n";
    }
}

std::string AdminCli::readLine(const std::string& prompt) const {
    std::cout << prompt;

    std::string input;
    std::getline(std::cin, input);

    return input;
}

void AdminCli::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
}