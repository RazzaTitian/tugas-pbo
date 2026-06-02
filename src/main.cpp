#include "cli/AdminCli.hpp"

#include "repositories/CsvAdminRepository.hpp"
#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "repositories/CsvMemberRepository.hpp"
#include "repositories/CsvReservationRepository.hpp"

#include "services/AdminService.hpp"
#include "services/BookService.hpp"
#include "services/LoanService.hpp"
#include "services/MemberService.hpp"

#include "models/Admin.hpp"
#include "utils/PasswordHasher.hpp"

int main() {
    CsvAdminRepository adminRepository("data/admins.csv");
    CsvBookRepository bookRepository("data/books.csv");
    CsvMemberRepository memberRepository("data/members.csv");
    CsvLoanRepository loanRepository("data/loans.csv");
    CsvReservationRepository reservationRepository("data/reservations.csv");

    if (!adminRepository.findById("admin").has_value()) {
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

    AdminCli adminCli(
        adminService,
        bookService,
        memberService,
        loanService
    );

    adminCli.run();

    return 0;
}