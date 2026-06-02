#pragma once

#include "services/BookService.hpp"
#include "services/LoanService.hpp"
#include "services/MemberService.hpp"

class WebServer {
private:
    BookService& bookService_;
    MemberService& memberService_;
    LoanService& loanService_;

public:
    WebServer(
        BookService& bookService,
        MemberService& memberService,
        LoanService& loanService
    );

    void run(int port = 8080);
};