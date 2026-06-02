#include "services/LoanService.hpp"

LoanService::LoanService(
    CsvBookRepository& bookRepository,
    CsvLoanRepository& loanRepository,
    CsvMemberRepository& memberRepository,
    CsvReservationRepository& reservationRepository
)
    : bookRepository_(bookRepository),
      loanRepository_(loanRepository),
      memberRepository_(memberRepository),
      reservationRepository_(reservationRepository) {}

bool LoanService::borrowBook(
    int bookId,
    const std::string& memberId,
    const std::string& borrowDate,
    const std::string& dueDate
) {
    auto memberOpt = memberRepository_.findById(memberId);

    if (!memberOpt.has_value()) {
        return false;
    }

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
    const std::string& returnDate,
    const std::string& nextDueDate
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

    bool loanSaved = loanRepository_.save(loan);

    if (!loanSaved) {
        return false;
    }

    auto reservationOpt = reservationRepository_.findFirstWaitingByBookId(book.id());

    if (reservationOpt.has_value()) {
        Reservation reservation = reservationOpt.value();

        auto memberOpt = memberRepository_.findById(reservation.memberId());

        if (!memberOpt.has_value()) {
            return false;
        }

        Loan nextLoan(
            loanRepository_.nextId(),
            book.id(),
            reservation.memberId(),
            returnDate,
            nextDueDate,
            "",
            LoanStatus::Active
        );

        reservation.setStatus(ReservationStatus::Fulfilled);
        book.setAvailable(false);

        bool nextLoanSaved = loanRepository_.save(nextLoan);
        bool reservationSaved = reservationRepository_.save(reservation);
        bool bookSaved = bookRepository_.save(book);

        return nextLoanSaved && reservationSaved && bookSaved;
    }

    book.setAvailable(true);

    bool bookSaved = bookRepository_.save(book);

    return bookSaved;
}

bool LoanService::reserveBook(
    int bookId,
    const std::string& memberId,
    const std::string& reservedAt
) {
    auto memberOpt = memberRepository_.findById(memberId);

    if (!memberOpt.has_value()) {
        return false;
    }

    auto bookOpt = bookRepository_.findById(bookId);

    if (!bookOpt.has_value()) {
        return false;
    }

    for (const Reservation& reservation : reservationRepository_.listWaitingByBookId(bookId)) {
        if (reservation.memberId() == memberId) {
            return false;
        }
    }

    Reservation reservation(
        reservationRepository_.nextId(),
        bookId,
        memberId,
        reservedAt,
        ReservationStatus::Waiting
    );

    return reservationRepository_.save(reservation);
}

std::vector<Loan> LoanService::listActiveLoans() const {
    return loanRepository_.listActive();
}

std::vector<Loan> LoanService::listLoanHistoryByMember(const std::string& memberId) const {
    return loanRepository_.listByMemberId(memberId);
}