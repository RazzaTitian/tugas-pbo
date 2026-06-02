#pragma once

#include <iostream>
#include <string>

class Book {
private:
    int id_;
    std::string title_;
    std::string author_;
    bool available_;

public:
    Book();
    Book(int id, std::string title, std::string author, bool available = true);

    int id() const;
    const std::string& title() const;
    const std::string& author() const;
    bool isAvailable() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setAvailable(bool available);

    std::string statusText() const;

    bool operator==(const Book& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};