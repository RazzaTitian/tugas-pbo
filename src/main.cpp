#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "services/LoanService.hpp"

#include <iostream>

int main() {
    CsvBookRepository bookRepository("data/books.csv");
    CsvLoanRepository loanRepository("data/loans.csv");

    LoanService loanService(bookRepository, loanRepository);

    bool borrowOk = loanService.borrowBook(
        3,
        "M003",
        "2026-06-02",
        "2026-06-16"
    );

    std::cout << "Borrow result: " << (borrowOk ? "success" : "failed") << '\n';

    bool returnOk = loanService.returnBook(
        1,
        "2026-06-02"
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

    return 0;
}