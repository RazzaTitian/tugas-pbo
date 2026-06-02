#include "repositories/CsvLoanRepository.hpp"

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

CsvLoanRepository::CsvLoanRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

Loan CsvLoanRepository::fromCsvLine(const std::string& line) {
    std::stringstream ss(line);

    std::string loanIdStr;
    std::string bookIdStr;
    std::string memberId;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    std::string status;

    std::getline(ss, loanIdStr, ',');
    std::getline(ss, bookIdStr, ',');
    std::getline(ss, memberId, ',');
    std::getline(ss, borrowDate, ',');
    std::getline(ss, dueDate, ',');
    std::getline(ss, returnDate, ',');
    std::getline(ss, status, ',');

    return Loan(
        std::stoi(loanIdStr),
        std::stoi(bookIdStr),
        memberId,
        borrowDate,
        dueDate,
        returnDate,
        Loan::statusFromString(status)
    );
}

std::string CsvLoanRepository::toCsvLine(const Loan& loan) {
    std::stringstream ss;

    ss << loan.loanId() << ","
       << loan.bookId() << ","
       << loan.memberId() << ","
       << loan.borrowDate() << ","
       << loan.dueDate() << ","
       << loan.returnDate() << ","
       << loan.statusText();

    return ss.str();
}

bool CsvLoanRepository::save(const Loan& loan) {
    std::vector<Loan> loans = listAll();
    bool updated = false;

    for (Loan& existingLoan : loans) {
        if (existingLoan.loanId() == loan.loanId()) {
            existingLoan = loan;
            updated = true;
            break;
        }
    }

    if (!updated) {
        loans.push_back(loan);
    }

    std::ofstream file(filePath_, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const Loan& currentLoan : loans) {
        file << toCsvLine(currentLoan) << '\n';
    }

    return true;
}

std::optional<Loan> CsvLoanRepository::findById(const int& id) const {
    std::vector<Loan> loans = listAll();

    for (const Loan& loan : loans) {
        if (loan.loanId() == id) {
            return loan;
        }
    }

    return std::nullopt;
}

std::vector<Loan> CsvLoanRepository::listAll() const {
    std::vector<Loan> loans;
    std::ifstream file(filePath_);

    if (!file.is_open()) {
        return loans;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        loans.push_back(fromCsvLine(line));
    }

    return loans;
}

bool CsvLoanRepository::remove(const int& id) {
    std::vector<Loan> loans = listAll();
    std::vector<Loan> remainingLoans;

    bool removed = false;

    for (const Loan& loan : loans) {
        if (loan.loanId() == id) {
            removed = true;
            continue;
        }

        remainingLoans.push_back(loan);
    }

    if (!removed) {
        return false;
    }

    std::ofstream file(filePath_, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const Loan& loan : remainingLoans) {
        file << toCsvLine(loan) << '\n';
    }

    return true;
}

std::vector<Loan> CsvLoanRepository::listActive() const {
    std::vector<Loan> activeLoans;

    for (const Loan& loan : listAll()) {
        if (loan.isActive()) {
            activeLoans.push_back(loan);
        }
    }

    return activeLoans;
}

std::vector<Loan> CsvLoanRepository::listByMemberId(const std::string& memberId) const {
    std::vector<Loan> memberLoans;

    for (const Loan& loan : listAll()) {
        if (loan.memberId() == memberId) {
            memberLoans.push_back(loan);
        }
    }

    return memberLoans;
}

std::vector<Loan> CsvLoanRepository::listOverdue(const std::string& today) const {
    std::vector<Loan> overdueLoans;

    for (const Loan& loan : listAll()) {
        if (loan.isOverdue(today)) {
            overdueLoans.push_back(loan);
        }
    }

    return overdueLoans;
}

int CsvLoanRepository::nextId() const {
    int highestId = 0;

    for (const Loan& loan : listAll()) {
        if (loan.loanId() > highestId) {
            highestId = loan.loanId();
        }
    }

    return highestId + 1;
}