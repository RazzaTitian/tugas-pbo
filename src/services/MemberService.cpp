#include "services/MemberService.hpp"

MemberService::MemberService(CsvMemberRepository &memberRepository,
                             CsvLoanRepository &loanRepository)
    : memberRepository_(memberRepository), loanRepository_(loanRepository) {}

bool MemberService::addMember(const Member &member) {
  return memberRepository_.save(member);
}

bool MemberService::updateMember(const Member &member) {
  auto existingMember = memberRepository_.findById(member.memberId());

  if (!existingMember.has_value()) {
    return false;
  }

  return memberRepository_.save(member);
}

bool MemberService::deleteMember(const std::string &memberId) {
  for (const Loan &loan : loanRepository_.listActive()) {
    if (loan.memberId() == memberId) {
      return false;
    }
  }

  return memberRepository_.remove(memberId);
}

std::optional<Member>
MemberService::findMemberById(const std::string &memberId) const {
  return memberRepository_.findById(memberId);
}

std::vector<Member> MemberService::listMembers() const {
  return memberRepository_.listAll();
}

std::vector<Loan>
MemberService::listMemberLoans(const std::string &memberId) const {
  return loanRepository_.listByMemberId(memberId);
}