#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "repositories/CsvReservationRepository.hpp"
#include "services/LoanService.hpp"
#include "repositories/CsvMemberRepository.hpp"

#include <iostream>

int main() {
    CsvBookRepository bookRepository("data/books.csv");
    CsvLoanRepository loanRepository("data/loans.csv");
    CsvMemberRepository memberRepository("data/members.csv");
    CsvReservationRepository reservationRepository("data/reservations.csv");

    LoanService loanService(
        bookRepository,
        loanRepository,
        memberRepository,
        reservationRepository
    );

    bool reserveOk = loanService.reserveBook(
        3,
        "M004",
        "2026-06-03"
    );

    std::cout << "Reserve result: " << (reserveOk ? "success" : "failed") << '\n';

    bool returnOk = loanService.returnBook(
        3,
        "2026-06-03",
        "2026-06-17"
    );

    std::cout << "Return result: " << (returnOk ? "success" : "failed") << '\n';

    std::cout << "\nBooks:\n";
    for (const Book& book : bookRepository.listAll()) {
        std::cout << book << '\n';
    }

    std::cout << "\nLoans:\n";
    for (const Loan& loan : loanRepository.listAll()) {
        std::cout << loan << '\n';
    }

    std::cout << "\nReservations:\n";
    for (const Reservation& reservation : reservationRepository.listAll()) {
        std::cout << reservation << '\n';
    }

    return 0;
}