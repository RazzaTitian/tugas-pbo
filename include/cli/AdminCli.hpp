#pragma once

#include "services/AdminService.hpp"
#include "services/BookService.hpp"
#include "services/LoanService.hpp"
#include "services/MemberService.hpp"

#include <string>

class AdminCli {
private:
    AdminService& adminService_;
    BookService& bookService_;
    MemberService& memberService_;
    LoanService& loanService_;

    bool login();

    void showMainMenu();
    void showBookMenu();
    void showMemberMenu();

    void listBooks();
    void addBook();
    void searchBooks();
    void deleteBook();

    void listMembers();
    void addMember();
    void deleteMember();
    void viewMemberLoans();

    int readInt(const std::string& prompt) const;
    std::string readLine(const std::string& prompt) const;
    void waitForEnter() const;

public:
    AdminCli(
        AdminService& adminService,
        BookService& bookService,
        MemberService& memberService,
        LoanService& loanService
    );

    void run();
};