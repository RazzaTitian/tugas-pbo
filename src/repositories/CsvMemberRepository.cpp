#include "repositories/CsvMemberRepository.hpp"

#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

CsvMemberRepository::CsvMemberRepository(std::string filePath)
    : filePath_(std::move(filePath)) {}

Member CsvMemberRepository::fromCsvLine(const std::string &line) {
  std::stringstream ss(line);

  std::string memberId;
  std::string username;
  std::string passwordHash;
  std::string name;
  std::string email;

  std::getline(ss, memberId, ',');
  std::getline(ss, username, ',');
  std::getline(ss, passwordHash, ',');
  std::getline(ss, name, ',');
  std::getline(ss, email, ',');

  return Member(memberId, username, passwordHash, name, email);
}

std::string CsvMemberRepository::toCsvLine(const Member &member) {
  std::stringstream ss;

  ss << member.memberId() << "," << member.username() << ","
     << member.passwordHash() << "," << member.name() << "," << member.email();

  return ss.str();
}

bool CsvMemberRepository::save(const Member &member) {
  std::vector<Member> members = listAll();
  bool updated = false;

  for (Member &existingMember : members) {
    if (existingMember.memberId() == member.memberId()) {
      existingMember = member;
      updated = true;
      break;
    }
  }

  if (!updated) {
    members.push_back(member);
  }

  std::ofstream file(filePath_, std::ios::trunc);
  if (!file.is_open()) {
    return false;
  }

  for (const Member &currentMember : members) {
    file << toCsvLine(currentMember) << '\n';
  }

  return true;
}

std::optional<Member>
CsvMemberRepository::findById(const std::string &id) const {
  std::vector<Member> members = listAll();

  for (const Member &member : members) {
    if (member.memberId() == id) {
      return member;
    }
  }

  return std::nullopt;
}

std::vector<Member> CsvMemberRepository::listAll() const {
  std::vector<Member> members;
  std::ifstream file(filePath_);

  if (!file.is_open()) {
    return members;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    members.push_back(fromCsvLine(line));
  }

  return members;
}

bool CsvMemberRepository::remove(const std::string &id) {
  std::vector<Member> members = listAll();
  std::vector<Member> remainingMembers;

  bool removed = false;

  for (const Member &member : members) {
    if (member.memberId() == id) {
      removed = true;
      continue;
    }

    remainingMembers.push_back(member);
  }

  if (!removed) {
    return false;
  }

  std::ofstream file(filePath_, std::ios::trunc);
  if (!file.is_open()) {
    return false;
  }

  for (const Member &member : remainingMembers) {
    file << toCsvLine(member) << '\n';
  }

  return true;
}