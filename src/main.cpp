#include "cli/AdminCli.hpp"
#include "web/WebServer.hpp"

#include "models/Admin.hpp"

#include "repositories/CsvAdminRepository.hpp"
#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "repositories/CsvMemberRepository.hpp"
#include "repositories/CsvReservationRepository.hpp"

#include "services/AdminService.hpp"
#include "services/BookService.hpp"
#include "services/LoanService.hpp"
#include "services/MemberService.hpp"

#include "utils/PasswordHasher.hpp"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    CsvAdminRepository adminRepository("data/admins.csv");
    CsvBookRepository bookRepository("data/books.csv");
    CsvMemberRepository memberRepository("data/members.csv");
    CsvLoanRepository loanRepository("data/loans.csv");
    CsvReservationRepository reservationRepository("data/reservations.csv");

    auto adminOpt = adminRepository.findById("admin");

    if (!adminOpt.has_value()) {
        adminRepository.save(Admin(
            "admin",
            PasswordHasher::hash("admin123"),
            "Main Admin"
        ));
    } else if (adminOpt.value().passwordHash().find('$') == std::string::npos) {
        adminRepository.save(Admin(
            "admin",
            PasswordHasher::hash("admin123"),
            "Main Admin"
        ));
    }

    AdminService adminService(adminRepository);
    BookService bookService(bookRepository, loanRepository);
    MemberService memberService(memberRepository, loanRepository);
    LoanService loanService(
        bookRepository,
        loanRepository,
        memberRepository,
        reservationRepository
    );

    std::string mode = "admin";

    if (argc >= 2) {
        mode = argv[1];
    }

    if (mode == "admin") {
        AdminCli adminCli(
            adminService,
            bookService,
            memberService,
            loanService
        );

        adminCli.run();
        return 0;
    }

    if (mode == "web") {
        WebServer webServer(
            bookService,
            memberService,
            loanService
        );

        webServer.run(8080);
        return 0;
    }

    std::cout << "Unknown mode: " << mode << '\n';
    std::cout << "Usage:\n";
    std::cout << "  oop_project admin\n";
    std::cout << "  oop_project web\n";

    return 1;
}