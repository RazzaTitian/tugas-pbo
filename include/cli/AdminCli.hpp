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