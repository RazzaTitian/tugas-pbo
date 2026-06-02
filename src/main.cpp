#include "models/Book.hpp"
#include "repositories/CsvBookRepository.hpp"

#include <iostream>

int main() {
    CsvBookRepository bookRepository("data/books.csv");

    bookRepository.save(Book(1, "Clean Code", "Robert C. Martin", true));
    bookRepository.save(Book(2, "Effective Modern C++", "Scott Meyers", true));
    bookRepository.save(Book(3, "Design Patterns", "Gamma Helm Johnson Vlissides", true));

    std::cout << "Books currently stored:\n\n";

    for (const Book& book : bookRepository.listAll()) {
        std::cout << book << '\n';
    }

    return 0;
}