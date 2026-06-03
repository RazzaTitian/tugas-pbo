#include "services/AdminService.hpp"
#include "utils/PasswordHasher.hpp"

AdminService::AdminService(CsvAdminRepository &adminRepository)
    : adminRepository_(adminRepository) {}

bool AdminService::authenticate(const std::string &username,
                                const std::string &password) const {
  auto adminOpt = adminRepository_.findById(username);

  if (!adminOpt.has_value()) {
    return false;
  }

  Admin admin = adminOpt.value();

  return PasswordHasher::verify(password, admin.passwordHash());
}

bool AdminService::createAdmin(const Admin &admin) {
  return adminRepository_.save(admin);
}

std::optional<Admin>
AdminService::findAdmin(const std::string &username) const {
  return adminRepository_.findById(username);
}