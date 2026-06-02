#include "repositories/CsvLoanRepository.hpp"
#include "repositories/CsvMemberRepository.hpp"
#include "services/MemberService.hpp"

#include <iostream>

int main() {
    CsvMemberRepository memberRepository("data/members.csv");
    CsvLoanRepository loanRepository("data/loans.csv");

    MemberService memberService(memberRepository, loanRepository);

    bool deleteM003 = memberService.deleteMember("M003");
    bool deleteM001 = memberService.deleteMember("M001");

    std::cout << "Delete M003: " << (deleteM003 ? "success" : "failed") << '\n';
    std::cout << "Delete M001: " << (deleteM001 ? "success" : "failed") << '\n';

    std::cout << "\nMembers:\n";
    for (const Member& member : memberService.listMembers()) {
        std::cout << member << '\n';
    }

    return 0;
}