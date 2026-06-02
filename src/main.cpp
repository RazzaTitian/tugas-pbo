#include "repositories/CsvBookRepository.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "services/LoanService.hpp"

#include <iostream>
#include <vector>

int main() {
    CsvBookRepository bookRepository("data/books.csv");
    CsvLoanRepository loanRepository("data/loans.csv");

    LoanService loanService(bookRepository, loanRepository);

    std::string memberId;

    std::cout << "Member ID: ";
    std::getline(std::cin, memberId);

    std::vector<Loan> history = loanService.listLoanHistoryByMember(memberId);

    std::cout << "\nLoan history for member " << memberId << ":\n\n";

    if (history.empty()) {
        std::cout << "No loan history found.\n";
        return 0;
    }

    for (const Loan& loan : history) {
        std::cout << loan << '\n';
    }

    return 0;
}