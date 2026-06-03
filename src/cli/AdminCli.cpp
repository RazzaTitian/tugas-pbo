#ifdef _WIN32
#include <conio.h>
#endif

#include "cli/AdminCli.hpp"
#include "models/Book.hpp"
#include "models/Member.hpp"
#include "models/Loan.hpp"
#include "utils/PasswordHasher.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
        std::string password = readPassword("Password: ");

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
            showMemberMenu();
        } else if (choice == "3") {
            showLoanMenu();
        } else if (choice == "4") {
            showReportsMenu();
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
        std::cout << "[3] Edit book\n";
        std::cout << "[4] Search books\n";
        std::cout << "[5] Delete book\n";
        std::cout << "[B] Back\n";

        std::string choice = readLine("Choice: ");

        if (choice == "1") {
            listBooks();
        } else if (choice == "2") {
            addBook();
        } else if (choice == "3") {
            editBook();
        } else if (choice == "4") {
            searchBooks();
        } else if (choice == "5") {
            deleteBook();
        } else if (choice == "B" || choice == "b") {
            running = false;
        } else {
            std::cout << "\nInvalid choice.\n";
            waitForEnter();
        }
    }
}

void AdminCli::showMemberMenu() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Member Management ==========\n";
        std::cout << "[1] List members\n";
        std::cout << "[2] Add member\n";
        std::cout << "[3] Edit member\n";
        std::cout << "[4] Delete member\n";
        std::cout << "[5] View member loan history\n";
        std::cout << "[B] Back\n";

        std::string choice = readLine("Choice: ");

        if (choice == "1") {
            listMembers();
        } else if (choice == "2") {
            addMember();
        } else if (choice == "3") {
            editMember();
        } else if (choice == "4") {
            deleteMember();
        } else if (choice == "5") {
            viewMemberLoans();
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

void AdminCli::editBook() {
    std::cout << "\n========== Edit Book ==========\n";

    int id = readInt("Book ID: ");

    auto bookOpt = bookService_.findBookById(id);

    if (!bookOpt.has_value()) {
        std::cout << "\n[ERROR] Book not found.\n";
        waitForEnter();
        return;
    }

    Book book = bookOpt.value();

    std::cout << "\nCurrent title : " << book.title() << '\n';
    std::cout << "Current author: " << book.author() << '\n';

    std::string newTitle = readLine("New title  (leave empty to keep current): ");
    std::string newAuthor = readLine("New author (leave empty to keep current): ");

    if (!newTitle.empty()) {
        book.setTitle(newTitle);
    }

    if (!newAuthor.empty()) {
        book.setAuthor(newAuthor);
    }

    if (bookService_.updateBook(book)) {
        std::cout << "\n[OK] Book updated successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to update book.\n";
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

void AdminCli::listMembers() {
    std::cout << "\n========== Members ==========\n";

    std::vector<Member> members = memberService_.listMembers();

    if (members.empty()) {
        std::cout << "No members found.\n";
        waitForEnter();
        return;
    }

    for (const Member& member : members) {
        std::cout << member << '\n';
    }

    waitForEnter();
}

void AdminCli::addMember() {
    std::cout << "\n========== Add Member ==========\n";

    std::string memberId = readLine("Member ID: ");
    std::string username = readLine("Username: ");
    std::string password = readPassword("Password: ");
    std::string name = readLine("Name: ");
    std::string email = readLine("Email: ");

    if (
        memberId.empty() ||
        username.empty() ||
        password.empty() ||
        name.empty() ||
        email.empty()
    ) {
        std::cout << "\n[ERROR] All fields are required.\n";
        waitForEnter();
        return;
    }

    if (memberService_.findMemberById(memberId).has_value()) {
        std::cout << "\n[ERROR] Member ID already exists.\n";
        waitForEnter();
        return;
    }

    Member member(
        memberId,
        username,
        PasswordHasher::hash(password),
        name,
        email
    );

    if (memberService_.addMember(member)) {
        std::cout << "\n[OK] Member added successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to add member.\n";
    }

    waitForEnter();
}

void AdminCli::editMember() {
    std::cout << "\n========== Edit Member ==========\n";

    std::string memberId = readLine("Member ID: ");

    auto memberOpt = memberService_.findMemberById(memberId);

    if (!memberOpt.has_value()) {
        std::cout << "\n[ERROR] Member not found.\n";
        waitForEnter();
        return;
    }

    Member member = memberOpt.value();

    std::cout << "\nCurrent name : " << member.name() << '\n';
    std::cout << "Current email: " << member.email() << '\n';

    std::string newName = readLine("New name  (leave empty to keep current): ");
    std::string newEmail = readLine("New email (leave empty to keep current): ");

    if (!newName.empty()) {
        member.setName(newName);
    }

    if (!newEmail.empty()) {
        member.setEmail(newEmail);
    }

    if (memberService_.updateMember(member)) {
        std::cout << "\n[OK] Member updated successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to update member.\n";
    }

    waitForEnter();
}

void AdminCli::deleteMember() {
    std::cout << "\n========== Delete Member ==========\n";

    std::string memberId = readLine("Member ID: ");

    if (!memberService_.findMemberById(memberId).has_value()) {
        std::cout << "\n[ERROR] Member not found.\n";
        waitForEnter();
        return;
    }

    if (memberService_.deleteMember(memberId)) {
        std::cout << "\n[OK] Member deleted successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to delete member. The member may have an active loan.\n";
    }

    waitForEnter();
}

void AdminCli::viewMemberLoans() {
    std::cout << "\n========== Member Loan History ==========\n";

    std::string memberId = readLine("Member ID: ");

    if (!memberService_.findMemberById(memberId).has_value()) {
        std::cout << "\n[ERROR] Member not found.\n";
        waitForEnter();
        return;
    }

    std::vector<Loan> loans = memberService_.listMemberLoans(memberId);

    if (loans.empty()) {
        std::cout << "\nNo loan history found for this member.\n";
        waitForEnter();
        return;
    }

    std::cout << "\nLoan history:\n";

    for (const Loan& loan : loans) {
        std::cout << loan << '\n';
    }

    waitForEnter();
}

void AdminCli::showLoanMenu() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Loan Workflow ==========\n";
        std::cout << "[1] Issue loan\n";
        std::cout << "[2] Return loan\n";
        std::cout << "[3] Reserve book\n";
        std::cout << "[4] List active loans\n";
        std::cout << "[B] Back\n";

        std::string choice = readLine("Choice: ");

        if (choice == "1") {
            issueLoan();
        } else if (choice == "2") {
            returnLoan();
        } else if (choice == "3") {
            reserveBook();
        } else if (choice == "4") {
            listActiveLoans();
        } else if (choice == "B" || choice == "b") {
            running = false;
        } else {
            std::cout << "\nInvalid choice.\n";
            waitForEnter();
        }
    }
}

void AdminCli::issueLoan() {
    std::cout << "\n========== Issue Loan ==========\n";

    int bookId = readInt("Book ID: ");
    std::string memberId = readLine("Member ID: ");
    std::string borrowDate = readLine("Borrow date (YYYY-MM-DD): ");
    std::string dueDate = readLine("Due date (YYYY-MM-DD): ");

    if (memberId.empty() || borrowDate.empty() || dueDate.empty()) {
        std::cout << "\n[ERROR] All fields are required.\n";
        waitForEnter();
        return;
    }

    if (loanService_.borrowBook(bookId, memberId, borrowDate, dueDate)) {
        std::cout << "\n[OK] Loan issued successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to issue loan. Check book availability and member ID.\n";
    }

    waitForEnter();
}

void AdminCli::returnLoan() {
    std::cout << "\n========== Return Loan ==========\n";

    int loanId = readInt("Loan ID: ");
    std::string returnDate = readLine("Return date (YYYY-MM-DD): ");
    std::string nextDueDate = readLine("Next due date if reserved (YYYY-MM-DD): ");

    if (returnDate.empty() || nextDueDate.empty()) {
        std::cout << "\n[ERROR] Return date and next due date are required.\n";
        waitForEnter();
        return;
    }

    if (loanService_.returnBook(loanId, returnDate, nextDueDate)) {
        std::cout << "\n[OK] Loan returned successfully.\n";
        std::cout << "[INFO] If the book had a reservation queue, it was assigned automatically.\n";
    } else {
        std::cout << "\n[ERROR] Failed to return loan. Check loan ID or loan status.\n";
    }

    waitForEnter();
}

void AdminCli::reserveBook() {
    std::cout << "\n========== Reserve Book ==========\n";

    int bookId = readInt("Book ID: ");
    std::string memberId = readLine("Member ID: ");
    std::string reservedAt = readLine("Reservation date (YYYY-MM-DD): ");

    if (memberId.empty() || reservedAt.empty()) {
        std::cout << "\n[ERROR] All fields are required.\n";
        waitForEnter();
        return;
    }

    if (loanService_.reserveBook(bookId, memberId, reservedAt)) {
        std::cout << "\n[OK] Book reserved successfully.\n";
    } else {
        std::cout << "\n[ERROR] Failed to reserve book. Check book ID, member ID, or duplicate reservation.\n";
    }

    waitForEnter();
}

void AdminCli::listActiveLoans() {
    std::cout << "\n========== Active Loans ==========\n";

    std::vector<Loan> loans = loanService_.listActiveLoans();

    if (loans.empty()) {
        std::cout << "No active loans found.\n";
        waitForEnter();
        return;
    }

    for (const Loan& loan : loans) {
        std::cout << loan << '\n';
    }

    waitForEnter();
}

void AdminCli::showReportsMenu() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Reports ==========\n";
        std::cout << "[1] Book summary\n";
        std::cout << "[2] Member summary\n";
        std::cout << "[3] Active loan summary\n";
        std::cout << "[B] Back\n";

        std::string choice = readLine("Choice: ");

        if (choice == "1") {
            reportBooks();
        } else if (choice == "2") {
            reportMembers();
        } else if (choice == "3") {
            reportActiveLoans();
        } else if (choice == "B" || choice == "b") {
            running = false;
        } else {
            std::cout << "\nInvalid choice.\n";
            waitForEnter();
        }
    }
}

void AdminCli::reportBooks() {
    std::cout << "\n========== Book Summary ==========\n";

    std::vector<Book> books = bookService_.listBooks();

    int availableCount = 0;
    int onLoanCount = 0;

    for (const Book& book : books) {
        if (book.isAvailable()) {
            ++availableCount;
        } else {
            ++onLoanCount;
        }
    }

    std::cout << "Total books     : " << books.size() << '\n';
    std::cout << "Available books : " << availableCount << '\n';
    std::cout << "Books on loan   : " << onLoanCount << '\n';

    waitForEnter();
}

void AdminCli::reportMembers() {
    std::cout << "\n========== Member Summary ==========\n";

    std::vector<Member> members = memberService_.listMembers();

    std::cout << "Total members: " << members.size() << '\n';

    waitForEnter();
}

void AdminCli::reportActiveLoans() {
    std::cout << "\n========== Active Loan Summary ==========\n";

    std::vector<Loan> loans = loanService_.listActiveLoans();

    std::cout << "Total active loans: " << loans.size() << "\n\n";

    if (loans.empty()) {
        waitForEnter();
        return;
    }

    for (const Loan& loan : loans) {
        std::cout << loan << '\n';
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

std::string AdminCli::readPassword(const std::string& prompt) const {
    std::cout << prompt;

    std::string password;

#ifdef _WIN32
    while (true) {
        int character = _getch();

        if (character == '\r') {
            std::cout << '\n';
            break;
        }

        if (character == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }

            continue;
        }

        if (character == 0 || character == 224) {
            _getch();
            continue;
        }

        password += static_cast<char>(character);
        std::cout << '*';
    }
#else
    std::getline(std::cin, password);
#endif

    return password;
}

void AdminCli::waitForEnter() const {
    std::cout << "\nPress Enter to continue...";
    std::string ignored;
    std::getline(std::cin, ignored);
}