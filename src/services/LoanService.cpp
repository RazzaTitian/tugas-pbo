#include "services/LoanService.hpp"

LoanService::LoanService(
    CsvBookRepository& bookRepository,
    CsvLoanRepository& loanRepository
)
    : bookRepository_(bookRepository),
      loanRepository_(loanRepository) {}

bool LoanService::borrowBook(
    int bookId,
    const std::string& memberId,
    const std::string& borrowDate,
    const std::string& dueDate
) {
    auto bookOpt = bookRepository_.findById(bookId);

    if (!bookOpt.has_value()) {
        return false;
    }

    Book book = bookOpt.value();

    if (!book.isAvailable()) {
        return false;
    }

    Loan loan(
        loanRepository_.nextId(),
        bookId,
        memberId,
        borrowDate,
        dueDate,
        "",
        LoanStatus::Active
    );

    book.setAvailable(false);

    bool loanSaved = loanRepository_.save(loan);
    bool bookSaved = bookRepository_.save(book);

    return loanSaved && bookSaved;
}

bool LoanService::returnBook(
    int loanId,
    const std::string& returnDate
) {
    auto loanOpt = loanRepository_.findById(loanId);

    if (!loanOpt.has_value()) {
        return false;
    }

    Loan loan = loanOpt.value();

    if (!loan.isActive()) {
        return false;
    }

    auto bookOpt = bookRepository_.findById(loan.bookId());

    if (!bookOpt.has_value()) {
        return false;
    }

    Book book = bookOpt.value();

    loan.setReturnDate(returnDate);
    loan.setStatus(LoanStatus::Returned);
    book.setAvailable(true);

    bool loanSaved = loanRepository_.save(loan);
    bool bookSaved = bookRepository_.save(book);

    return loanSaved && bookSaved;
}

std::vector<Loan> LoanService::listActiveLoans() const {
    return loanRepository_.listActive();
}

std::vector<Loan> LoanService::listLoanHistoryByMember(const std::string& memberId) const {
    return loanRepository_.listByMemberId(memberId);
}