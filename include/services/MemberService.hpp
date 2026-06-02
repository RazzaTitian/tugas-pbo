#pragma once

#include "models/Member.hpp"
#include "repositories/CsvLoanRepository.hpp"
#include "repositories/CsvMemberRepository.hpp"

#include <optional>
#include <string>
#include <vector>

class MemberService {
private:
    CsvMemberRepository& memberRepository_;
    CsvLoanRepository& loanRepository_;

public:
    MemberService(
        CsvMemberRepository& memberRepository,
        CsvLoanRepository& loanRepository
    );

    bool addMember(const Member& member);
    bool updateMember(const Member& member);
    bool deleteMember(const std::string& memberId);

    std::optional<Member> findMemberById(const std::string& memberId) const;
    std::vector<Member> listMembers() const;
    std::vector<Loan> listMemberLoans(const std::string& memberId) const;
};