#include "web/WebServer.hpp"

#include "external/httplib.h"
#include "models/Book.hpp"
#include "models/Loan.hpp"
#include "models/Member.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::string escapeJson(const std::string &text) {
  std::string escaped;

  for (char character : text) {
    if (character == '"') {
      escaped += "\\\"";
    } else if (character == '\\') {
      escaped += "\\\\";
    } else if (character == '\n') {
      escaped += "\\n";
    } else if (character == '\r') {
      escaped += "\\r";
    } else if (character == '\t') {
      escaped += "\\t";
    } else {
      escaped += character;
    }
  }

  return escaped;
}
} // namespace

WebServer::WebServer(BookService &bookService, MemberService &memberService,
                     LoanService &loanService)
    : bookService_(bookService), memberService_(memberService),
      loanService_(loanService) {}

void WebServer::run(int port) {
  httplib::Server server;

  server.Get("/", [this](const httplib::Request &request,
                         httplib::Response &response) {
    constexpr int booksPerPage = 10;

    int currentPage = 1;

    if (request.has_param("page")) {
      try {
        currentPage = std::stoi(request.get_param_value("page"));
      } catch (const std::invalid_argument &) {
        currentPage = 1;
      } catch (const std::out_of_range &) {
        currentPage = 1;
      }
    }

    if (currentPage < 1) {
      currentPage = 1;
    }

    std::vector<Book> allBooks = bookService_.listBooks();
    std::vector<Book> availableBooks;

    for (const Book &book : allBooks) {
      if (book.isAvailable()) {
        availableBooks.push_back(book);
      }
    }

    int totalPages = 1;

    if (!availableBooks.empty()) {
      totalPages = static_cast<int>((availableBooks.size() + booksPerPage - 1) /
                                    booksPerPage);
    }

    if (currentPage > totalPages) {
      currentPage = totalPages;
    }

    int startIndex = (currentPage - 1) * booksPerPage;
    int endIndex = startIndex + booksPerPage;

    if (endIndex > static_cast<int>(availableBooks.size())) {
      endIndex = static_cast<int>(availableBooks.size());
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>UGM Library</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; max-width: 900px; margin: "
            "40px auto; padding: 0 20px; }";
    html += "h1 { margin-bottom: 4px; }";
    html += ".subtitle { color: #555; margin-top: 0; }";
    html += ".nav-box { display: flex; gap: 16px; flex-wrap: wrap; margin: "
            "24px 0; }";
    html += ".card { border: 1px solid #ddd; border-radius: 8px; padding: "
            "16px; margin-bottom: 16px; }";
    html += ".book-card { border: 1px solid #ddd; border-radius: 8px; padding: "
            "16px; margin-bottom: 14px; }";
    html += ".available { color: green; font-weight: bold; }";
    html += "input { padding: 6px; margin: 4px 0; }";
    html += "button { padding: 6px 10px; cursor: pointer; }";
    html += "form { margin-top: 8px; }";
    html += "a { text-decoration: none; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Library Management System</h1>";

    html += "<p class='subtitle'>";
    html += "Object-Oriented Programming Mini Project";
    html += "</p>";

    html += "<p class='subtitle'>";
    html += "Departemen Teknik Elektro dan Teknologi Informasi (DTETI)";
    html += "</p>";

    html += "<p class='subtitle'>";
    html += "Universitas Gadjah Mada";
    html += "</p>";

    html += "<div class='nav-box'>";

    html += "<div class='card'>";
    html += "<h3>Search Books</h3>";
    html += "<form action='/search' method='get'>";
    html += "<input type='text' name='q' placeholder='Title or author'>";
    html += "<br>";
    html += "<button type='submit'>Search</button>";
    html += "</form>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>My Loans</h3>";
    html += "<form action='/me' method='get'>";
    html += "<input type='text' name='id' placeholder='Member ID'>";
    html += "<br>";
    html += "<button type='submit'>View My Loans</button>";
    html += "</form>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>Developer API</h3>";
    html += "<p>View available JSON endpoints.</p>";
    html += "<a href='/api'>Open API Docs</a>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>Loan Export</h3>";
    html += "<p>Download active loan data as CSV.</p>";
    html += "<a href='/export/loans'>Download CSV</a>";
    html += "</div>";

    html += "</div>";

    std::vector<Book> allBooksForStats = bookService_.listBooks();
    std::vector<Member> membersForStats = memberService_.listMembers();
    std::vector<Loan> activeLoansForStats = loanService_.listActiveLoans();

    int availableBookCount = 0;

    for (const Book &book : allBooksForStats) {
      if (book.isAvailable()) {
        ++availableBookCount;
      }
    }

    html += "<h2>Quick Access</h2>";

    html += "<div class='nav-box'>";

    html += "<div class='card'>";
    html += "<h3>Member Portal</h3>";
    html += "<p>View your loan history.</p>";
    html += "<a href='/me?id=M003'>View Example Member</a>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>API Documentation</h3>";
    html += "<p>Browse all available API endpoints.</p>";
    html += "<a href='/api'>Open API Docs</a>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>Export Loans</h3>";
    html += "<p>Download active loan data.</p>";
    html += "<a href='/export/loans'>Download CSV</a>";
    html += "</div>";

    html += "</div>";

    html += "<h2>Library Statistics</h2>";

    html += "<div class='nav-box'>";

    html += "<div class='card'>";
    html += "<h3>Total Books</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(allBooksForStats.size());
    html += "</p>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>Available Books</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(availableBookCount);
    html += "</p>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>Members</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(membersForStats.size());
    html += "</p>";
    html += "</div>";

    html += "<div class='card'>";
    html += "<h3>Active Loans</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(activeLoansForStats.size());
    html += "</p>";
    html += "</div>";

    html += "</div>";

    html += "<h2>Available Books</h2>";

    if (availableBooks.empty()) {
      html += "<p>No available books at the moment.</p>";
    } else {
      for (int index = startIndex; index < endIndex; ++index) {
        const Book &book = availableBooks[index];

        html += "<div class='book-card'>";

        html += "<h3>";
        html += book.title();
        html += "</h3>";

        html += "<p>Author: ";
        html += book.author();
        html += "</p>";

        html += "<p class='available'>Available</p>";

        html += "<form action='/borrow' method='post'>";
        html += "<input type='hidden' name='bookId' value='";
        html += std::to_string(book.id());
        html += "'>";
        html += "<input type='text' name='memberId' placeholder='Member ID'>";
        html += "<br>";
        html += "<button type='submit'>Borrow</button>";
        html += "</form>";

        html += "</div>";
      }

      html += "<div style='margin-top: 24px;'>";

      if (currentPage > 1) {
        html += "<a href='/?page=";
        html += std::to_string(currentPage - 1);
        html += "'>Previous</a> ";
      }

      html += "Page ";
      html += std::to_string(currentPage);
      html += " of ";
      html += std::to_string(totalPages);

      if (currentPage < totalPages) {
        html += " <a href='/?page=";
        html += std::to_string(currentPage + 1);
        html += "'>Next</a>";
      }

      html += "</div>";
    }

    html += "<hr>";

    html += "<div style='margin-top: 16px; color: #666;'>";
    html += "<strong>Library Management System</strong><br>";
    html += "Developed by Muhammad Razza Titian Jiwani<br>";
    html += "Object-Oriented Programming Mini Project<br>";
    html += "DTETI - Universitas Gadjah Mada<br>";
    html += "Powered by C++17, CSV, and cpp-httplib";
    html += "</div>";

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
  });

  server.Get("/search", [this](const httplib::Request &request,
                               httplib::Response &response) {
    std::string keyword;

    if (request.has_param("q")) {
      keyword = request.get_param_value("q");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Search Results</title>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Search Results</h1>";

    html += "<form action='/search' method='get'>";
    html += "<input type='text' name='q' value='";
    html += keyword;
    html += "'>";
    html += "<button type='submit'>Search</button>";
    html += "</form><br>";

    if (keyword.empty()) {
      html += "<p>No search keyword provided.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    html += "<p>Keyword: <strong>";
    html += keyword;
    html += "</strong></p>";

    std::vector<Book> books = bookService_.searchBooks(keyword);

    if (books.empty()) {
      html += "<p>No matching books found.</p>";
    } else {
      html += "<table border='1' cellpadding='8' cellspacing='0'>";
      html += "<thead>";
      html += "<tr>";
      html += "<th>ID</th>";
      html += "<th>Title</th>";
      html += "<th>Author</th>";
      html += "<th>Status</th>";
      html += "</tr>";
      html += "</thead>";
      html += "<tbody>";

      for (const Book &book : books) {
        html += "<tr>";

        html += "<td>";
        html += std::to_string(book.id());
        html += "</td>";

        html += "<td>";
        html += book.title();
        html += "</td>";

        html += "<td>";
        html += book.author();
        html += "</td>";

        html += "<td>";

        if (book.isAvailable()) {
          html += "<span style='color:green;'>Available</span>";
        } else {
          html += "<span style='color:red;'>On Loan</span>";
        }

        html += "</td>";

        html += "</tr>";
      }

      html += "</tbody>";
      html += "</table>";
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
  });

  server.Get("/me", [this](const httplib::Request &request,
                           httplib::Response &response) {
    std::string memberId;

    if (request.has_param("id")) {
      memberId = request.get_param_value("id");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>My Loans</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; max-width: 1000px; margin: "
            "40px auto; padding: 0 20px; }";
    html +=
        "table { border-collapse: collapse; width: 100%; margin-top: 16px; }";
    html +=
        "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }";
    html += "th { background: #f4f4f4; }";
    html += ".active { color: green; font-weight: bold; }";
    html += ".returned { color: #555; font-weight: bold; }";
    html += "button { padding: 6px 10px; cursor: pointer; }";
    html += "a { text-decoration: none; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";

    html += "<h1>My Loans</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    if (memberId.empty()) {
      html += "<p>No member ID provided.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    auto memberOpt = memberService_.findMemberById(memberId);

    if (!memberOpt.has_value()) {
      html += "<p>Member not found.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    Member member = memberOpt.value();

    html += "<p>Member ID: <strong>";
    html += member.memberId();
    html += "</strong></p>";

    html += "<p>Name: <strong>";
    html += member.name();
    html += "</strong></p>";

    html += "<p>Email: ";
    html += member.email();
    html += "</p>";

    std::vector<Loan> loans = memberService_.listMemberLoans(memberId);

    int activeLoanCount = 0;
    int returnedLoanCount = 0;

    for (const Loan &loan : loans) {
      if (loan.isActive()) {
        ++activeLoanCount;
      } else {
        ++returnedLoanCount;
      }
    }

    html += "<h2>Member Statistics</h2>";

    html += "<div style='display: flex; gap: 16px; flex-wrap: wrap; margin: "
            "16px 0;'>";

    html += "<div style='border: 1px solid #ddd; border-radius: 8px; padding: "
            "16px;'>";
    html += "<h3>Total Loans</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(loans.size());
    html += "</p>";
    html += "</div>";

    html += "<div style='border: 1px solid #ddd; border-radius: 8px; padding: "
            "16px;'>";
    html += "<h3>Active Loans</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(activeLoanCount);
    html += "</p>";
    html += "</div>";

    html += "<div style='border: 1px solid #ddd; border-radius: 8px; padding: "
            "16px;'>";
    html += "<h3>Returned Loans</h3>";
    html += "<p style='font-size: 24px; font-weight: bold;'>";
    html += std::to_string(returnedLoanCount);
    html += "</p>";
    html += "</div>";

    html += "</div>";

    if (loans.empty()) {
      html += "<p>No loan history found.</p>";
    } else {
      html += "<h2>Loan History</h2>";

      html += "<table>";
      html += "<thead>";
      html += "<tr>";
      html += "<th>Loan ID</th>";
      html += "<th>Book ID</th>";
      html += "<th>Borrow Date</th>";
      html += "<th>Due Date</th>";
      html += "<th>Return Date</th>";
      html += "<th>Status</th>";
      html += "<th>Action</th>";
      html += "</tr>";
      html += "</thead>";
      html += "<tbody>";

      for (const Loan &loan : loans) {
        html += "<tr>";

        html += "<td>";
        html += std::to_string(loan.loanId());
        html += "</td>";

        html += "<td>";
        html += std::to_string(loan.bookId());
        html += "</td>";

        html += "<td>";
        html += loan.borrowDate();
        html += "</td>";

        html += "<td>";
        html += loan.dueDate();
        html += "</td>";

        html += "<td>";
        html += loan.returnDate().empty() ? "-" : loan.returnDate();
        html += "</td>";

        html += "<td>";

        if (loan.isActive()) {
          html += "<span class='active'>";
          html += loan.statusText();
          html += "</span>";
        } else {
          html += "<span class='returned'>";
          html += loan.statusText();
          html += "</span>";
        }

        html += "</td>";

        html += "<td>";

        if (loan.isActive()) {
          html += "<form action='/return' method='post'>";
          html += "<input type='hidden' name='loanId' value='";
          html += std::to_string(loan.loanId());
          html += "'>";
          html += "<button type='submit'>Return</button>";
          html += "</form>";
        } else {
          html += "-";
        }

        html += "</td>";

        html += "</tr>";
      }

      html += "</tbody>";
      html += "</table>";
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
  });

  server.Post("/borrow", [this](const httplib::Request &request,
                                httplib::Response &response) {
    std::string bookIdValue;
    std::string memberId;

    if (request.has_param("bookId")) {
      bookIdValue = request.get_param_value("bookId");
    }

    if (request.has_param("memberId")) {
      memberId = request.get_param_value("memberId");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Borrow Book</title>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Borrow Book</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    if (bookIdValue.empty() || memberId.empty()) {
      html += "<p>Missing book ID or member ID.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    int bookId = 0;

    try {
      bookId = std::stoi(bookIdValue);
    } catch (const std::invalid_argument &) {
      html += "<p>Invalid book ID.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    } catch (const std::out_of_range &) {
      html += "<p>Invalid book ID.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    bool borrowOk =
        loanService_.borrowBook(bookId, memberId, "2026-06-03", "2026-06-17");

    if (borrowOk) {
      html += "<p style='color: green;'>Book borrowed successfully.</p>";
      html += "<br>";
      html += "<a href='/'>Return to Homepage</a>";
      html += "<p>Member ID: ";
      html += memberId;
      html += "</p>";
    } else {
      html += "<p style='color: red;'>Failed to borrow book.</p>";
      html += "<br>";
      html += "<a href='/'>Return to Homepage</a>";
      html += "<p>The book may be unavailable, or the member ID may be "
              "invalid.</p>";
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
  });

  server.Post("/return", [this](const httplib::Request &request,
                                httplib::Response &response) {
    std::string loanIdValue;

    if (request.has_param("loanId")) {
      loanIdValue = request.get_param_value("loanId");
    }

    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Return Book</title>";
    html += "</head>";
    html += "<body>";

    html += "<h1>Return Book</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    if (loanIdValue.empty()) {
      html += "<p>Missing loan ID.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    int loanId = 0;

    try {
      loanId = std::stoi(loanIdValue);
    } catch (const std::invalid_argument &) {
      html += "<p>Invalid loan ID.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    } catch (const std::out_of_range &) {
      html += "<p>Invalid loan ID.</p>";
      html += "</body>";
      html += "</html>";

      response.set_content(html, "text/html");
      return;
    }

    bool returnOk = loanService_.returnBook(loanId, "2026-06-03", "2026-06-17");

    if (returnOk) {
      html += "<p style='color: green;'>Book returned successfully.</p>";
      html += "<br>";
      html += "<a href='/'>Return to Homepage</a>";
      html += "<p>If there was a reservation queue, the book was assigned "
              "automatically.</p>";
    } else {
      html += "<p style='color: red;'>Failed to return book.</p>";
      html += "<br>";
      html += "<a href='/'>Return to Homepage</a>";
      html += "<p>The loan ID may be invalid or already returned.</p>";
    }

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
  });

  server.Get("/api", [](const httplib::Request &, httplib::Response &response) {
    std::string html;

    html += "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<meta charset='UTF-8'>";
    html += "<title>API Documentation</title>";
    html += "<style>";
    html += "body { font-family: Arial, sans-serif; max-width: 900px; margin: "
            "40px auto; padding: 0 20px; }";
    html +=
        "code { background: #f4f4f4; padding: 3px 6px; border-radius: 4px; }";
    html += ".endpoint { border: 1px solid #ddd; border-radius: 8px; padding: "
            "16px; margin-bottom: 14px; }";
    html += "</style>";
    html += "</head>";
    html += "<body>";

    html += "<h1>API Documentation</h1>";
    html += "<a href='/'>Back to Home</a><br><br>";

    html += "<p>This page lists the available JSON API endpoints for the OOP "
            "Borrowing System.</p>";

    html += "<div class='endpoint'>";
    html += "<h3>GET /api/books</h3>";
    html += "<p>Returns all books.</p>";
    html += "<code>http://localhost:8080/api/books</code>";
    html += "</div>";

    html += "<div class='endpoint'>";
    html += "<h3>GET /api/members</h3>";
    html += "<p>Returns all registered members.</p>";
    html += "<code>http://localhost:8080/api/members</code>";
    html += "</div>";

    html += "<div class='endpoint'>";
    html += "<h3>GET /api/loans</h3>";
    html += "<p>Returns all active loans.</p>";
    html += "<code>http://localhost:8080/api/loans</code>";
    html += "</div>";

    html += "<div class='endpoint'>";
    html += "<h3>GET /api/stats</h3>";
    html += "<p>Returns library statistics.</p>";
    html += "<code>http://localhost:8080/api/stats</code>";
    html += "</div>";

    html += "<div class='endpoint'>";
    html += "<h3>GET /export/loans</h3>";
    html += "<p>Downloads active loans as a CSV file.</p>";
    html += "<code>http://localhost:8080/export/loans</code>";
    html += "</div>";

    html += "</body>";
    html += "</html>";

    response.set_content(html, "text/html");
  });

  server.Get("/api/books",
             [this](const httplib::Request &, httplib::Response &response) {
               std::vector<Book> books = bookService_.listBooks();

               std::string json;

               json += "[";

               for (std::size_t index = 0; index < books.size(); ++index) {
                 const Book &book = books[index];

                 json += "{";

                 json += "\"id\":";
                 json += std::to_string(book.id());
                 json += ",";

                 json += "\"title\":\"";
                 json += escapeJson(book.title());
                 json += "\",";

                 json += "\"author\":\"";
                 json += escapeJson(book.author());
                 json += "\",";

                 json += "\"available\":";
                 json += book.isAvailable() ? "true" : "false";

                 json += "}";

                 if (index + 1 < books.size()) {
                   json += ",";
                 }
               }

               json += "]";

               response.set_content(json, "application/json");
             });

  server.Get("/api/members",
             [this](const httplib::Request &, httplib::Response &response) {
               std::vector<Member> members = memberService_.listMembers();

               std::string json;

               json += "[";

               for (std::size_t index = 0; index < members.size(); ++index) {
                 const Member &member = members[index];

                 json += "{";

                 json += "\"memberId\":\"";
                 json += escapeJson(member.memberId());
                 json += "\",";

                 json += "\"username\":\"";
                 json += escapeJson(member.username());
                 json += "\",";

                 json += "\"name\":\"";
                 json += escapeJson(member.name());
                 json += "\",";

                 json += "\"email\":\"";
                 json += escapeJson(member.email());
                 json += "\"";

                 json += "}";

                 if (index + 1 < members.size()) {
                   json += ",";
                 }
               }

               json += "]";

               response.set_content(json, "application/json");
             });

  server.Get("/api/loans",
             [this](const httplib::Request &, httplib::Response &response) {
               std::vector<Loan> loans = loanService_.listActiveLoans();

               std::string json;

               json += "[";

               for (std::size_t index = 0; index < loans.size(); ++index) {
                 const Loan &loan = loans[index];

                 json += "{";

                 json += "\"loanId\":";
                 json += std::to_string(loan.loanId());
                 json += ",";

                 json += "\"bookId\":";
                 json += std::to_string(loan.bookId());
                 json += ",";

                 json += "\"memberId\":\"";
                 json += escapeJson(loan.memberId());
                 json += "\",";

                 json += "\"borrowDate\":\"";
                 json += escapeJson(loan.borrowDate());
                 json += "\",";

                 json += "\"dueDate\":\"";
                 json += escapeJson(loan.dueDate());
                 json += "\",";

                 json += "\"returnDate\":\"";
                 json += escapeJson(loan.returnDate());
                 json += "\",";

                 json += "\"status\":\"";
                 json += escapeJson(loan.statusText());
                 json += "\"";

                 json += "}";

                 if (index + 1 < loans.size()) {
                   json += ",";
                 }
               }

               json += "]";

               response.set_content(json, "application/json");
             });

  server.Get("/api/stats",
             [this](const httplib::Request &, httplib::Response &response) {
               std::vector<Book> books = bookService_.listBooks();
               std::vector<Member> members = memberService_.listMembers();
               std::vector<Loan> activeLoans = loanService_.listActiveLoans();

               int availableBooks = 0;

               for (const Book &book : books) {
                 if (book.isAvailable()) {
                   ++availableBooks;
                 }
               }

               std::string json;

               json += "{";

               json += "\"totalBooks\":";
               json += std::to_string(books.size());
               json += ",";

               json += "\"availableBooks\":";
               json += std::to_string(availableBooks);
               json += ",";

               json += "\"members\":";
               json += std::to_string(members.size());
               json += ",";

               json += "\"activeLoans\":";
               json += std::to_string(activeLoans.size());

               json += "}";

               response.set_content(json, "application/json");
             });

  server.Get("/export/loans", [this](const httplib::Request &,
                                     httplib::Response &response) {
    std::vector<Loan> loans = loanService_.listActiveLoans();

    std::string csv;

    csv += "loanId,bookId,memberId,borrowDate,dueDate,returnDate,status\n";

    for (const Loan &loan : loans) {
      csv += std::to_string(loan.loanId());
      csv += ",";
      csv += std::to_string(loan.bookId());
      csv += ",";
      csv += loan.memberId();
      csv += ",";
      csv += loan.borrowDate();
      csv += ",";
      csv += loan.dueDate();
      csv += ",";
      csv += loan.returnDate();
      csv += ",";
      csv += loan.statusText();
      csv += "\n";
    }

    response.set_header("Content-Disposition",
                        "attachment; filename=\"active_loans_export.csv\"");

    response.set_content(csv, "text/csv");
  });

  std::cout << "Web server running at http://localhost:" << port << '\n';

  server.listen("0.0.0.0", port);
}