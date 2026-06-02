#include "repositories/CsvBookRepository.hpp"

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

CsvBookRepository::CsvBookRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

Book CsvBookRepository::fromCsvLine(const std::string& line) {
    std::stringstream ss(line);

    std::string idStr;
    std::string title;
    std::string author;
    std::string availableStr;

    std::getline(ss, idStr, ',');
    std::getline(ss, title, ',');
    std::getline(ss, author, ',');
    std::getline(ss, availableStr, ',');

    return Book(
        std::stoi(idStr),
        title,
        author,
        availableStr == "1"
    );
}

std::string CsvBookRepository::toCsvLine(const Book& book) {
    std::stringstream ss;

    ss << book.id() << ","
       << book.title() << ","
       << book.author() << ","
       << (book.isAvailable() ? "1" : "0");

    return ss.str();
}

bool CsvBookRepository::save(const Book& book) {
    std::vector<Book> books = listAll();
    bool updated = false;

    for (Book& existingBook : books) {
        if (existingBook.id() == book.id()) {
            existingBook = book;
            updated = true;
            break;
        }
    }

    if (!updated) {
        books.push_back(book);
    }

    std::ofstream file(filePath_, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const Book& currentBook : books) {
        file << toCsvLine(currentBook) << '\n';
    }

    return true;
}

std::optional<Book> CsvBookRepository::findById(const int& id) const {
    std::vector<Book> books = listAll();

    for (const Book& book : books) {
        if (book.id() == id) {
            return book;
        }
    }

    return std::nullopt;
}

std::vector<Book> CsvBookRepository::listAll() const {
    std::vector<Book> books;
    std::ifstream file(filePath_);

    if (!file.is_open()) {
        return books;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        books.push_back(fromCsvLine(line));
    }

    return books;
}

bool CsvBookRepository::remove(const int& id) {
    std::vector<Book> books = listAll();
    std::vector<Book> remainingBooks;

    bool removed = false;

    for (const Book& book : books) {
        if (book.id() == id) {
            removed = true;
            continue;
        }

        remainingBooks.push_back(book);
    }

    if (!removed) {
        return false;
    }

    std::ofstream file(filePath_, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const Book& book : remainingBooks) {
        file << toCsvLine(book) << '\n';
    }

    return true;
}