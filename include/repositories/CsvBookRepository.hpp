#pragma once

#include "models/Book.hpp"
#include "repositories/Repository.hpp"

#include <optional>
#include <string>
#include <vector>

class CsvBookRepository : public Repository<Book, int> {
private:
    std::string filePath_;

    static Book fromCsvLine(const std::string& line);
    static std::string toCsvLine(const Book& book);

public:
    explicit CsvBookRepository(std::string filePath);

    bool save(const Book& book) override;
    std::optional<Book> findById(const int& id) const override;
    std::vector<Book> listAll() const override;
    bool remove(const int& id) override;
};