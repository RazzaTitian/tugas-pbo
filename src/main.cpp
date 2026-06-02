#include "repositories/CsvBookRepository.hpp"

#include <iostream>

int main() {
    CsvBookRepository bookRepository("data/books.csv");

    std::string keyword;

    std::cout << "Search keyword: ";
    std::getline(std::cin, keyword);

    std::vector<Book> results = bookRepository.search(keyword);

    std::cout << "\nSearch results:\n\n";

    if (results.empty()) {
        std::cout << "No books found.\n";
        return 0;
    }

    for (const Book& book : results) {
        std::cout << book << '\n';
    }

    return 0;
}