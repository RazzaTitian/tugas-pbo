#include "repositories/CsvBookRepository.hpp"

#include <fstream>
#include <sstream>
#include <utility>

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