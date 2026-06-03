#include "utils/PasswordHasher.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace {
constexpr std::array<std::uint32_t, 64> K = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

std::uint32_t rotateRight(std::uint32_t value, std::uint32_t bits) {
  return (value >> bits) | (value << (32U - bits));
}

std::string sha256(const std::string &input) {
  std::vector<std::uint8_t> data(input.begin(), input.end());

  std::uint64_t bitLength = static_cast<std::uint64_t>(data.size()) * 8U;

  data.push_back(0x80);

  while ((data.size() % 64U) != 56U) {
    data.push_back(0x00);
  }

  for (int shift = 56; shift >= 0; shift -= 8) {
    data.push_back(static_cast<std::uint8_t>((bitLength >> shift) & 0xffU));
  }

  std::array<std::uint32_t, 8> hash = {0x6a09e667, 0xbb67ae85, 0x3c6ef372,
                                       0xa54ff53a, 0x510e527f, 0x9b05688c,
                                       0x1f83d9ab, 0x5be0cd19};

  for (std::size_t chunk = 0; chunk < data.size(); chunk += 64U) {
    std::array<std::uint32_t, 64> words{};

    for (std::size_t index = 0; index < 16U; ++index) {
      std::size_t offset = chunk + index * 4U;

      words[index] = (static_cast<std::uint32_t>(data[offset]) << 24U) |
                     (static_cast<std::uint32_t>(data[offset + 1U]) << 16U) |
                     (static_cast<std::uint32_t>(data[offset + 2U]) << 8U) |
                     static_cast<std::uint32_t>(data[offset + 3U]);
    }

    for (std::size_t index = 16U; index < 64U; ++index) {
      std::uint32_t s0 = rotateRight(words[index - 15U], 7U) ^
                         rotateRight(words[index - 15U], 18U) ^
                         (words[index - 15U] >> 3U);

      std::uint32_t s1 = rotateRight(words[index - 2U], 17U) ^
                         rotateRight(words[index - 2U], 19U) ^
                         (words[index - 2U] >> 10U);

      words[index] = words[index - 16U] + s0 + words[index - 7U] + s1;
    }

    std::uint32_t a = hash[0];
    std::uint32_t b = hash[1];
    std::uint32_t c = hash[2];
    std::uint32_t d = hash[3];
    std::uint32_t e = hash[4];
    std::uint32_t f = hash[5];
    std::uint32_t g = hash[6];
    std::uint32_t h = hash[7];

    for (std::size_t index = 0; index < 64U; ++index) {
      std::uint32_t s1 =
          rotateRight(e, 6U) ^ rotateRight(e, 11U) ^ rotateRight(e, 25U);

      std::uint32_t choice = (e & f) ^ ((~e) & g);

      std::uint32_t temp1 = h + s1 + choice + K[index] + words[index];

      std::uint32_t s0 =
          rotateRight(a, 2U) ^ rotateRight(a, 13U) ^ rotateRight(a, 22U);

      std::uint32_t majority = (a & b) ^ (a & c) ^ (b & c);

      std::uint32_t temp2 = s0 + majority;

      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
    }

    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
    hash[5] += f;
    hash[6] += g;
    hash[7] += h;
  }

  std::ostringstream output;

  for (std::uint32_t value : hash) {
    output << std::hex << std::setw(8) << std::setfill('0') << value;
  }

  return output.str();
}

std::string generateSalt() {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<int> distribution(0, 15);

  std::string salt;

  for (int index = 0; index < 32; ++index) {
    int value = distribution(generator);
    salt += "0123456789abcdef"[value];
  }

  return salt;
}

std::string legacyStdHash(const std::string &password) {
  std::hash<std::string> hasher;

  std::ostringstream output;
  output << hasher(password);

  return output.str();
}
} // namespace

std::string PasswordHasher::hash(const std::string &password) {
  std::string salt = generateSalt();
  std::string digest = sha256(salt + password);

  return salt + "$" + digest;
}

bool PasswordHasher::verify(const std::string &password,
                            const std::string &storedHash) {
  std::size_t separatorPosition = storedHash.find('$');

  if (separatorPosition == std::string::npos) {
    return legacyStdHash(password) == storedHash;
  }

  std::string salt = storedHash.substr(0, separatorPosition);
  std::string expectedDigest = storedHash.substr(separatorPosition + 1U);

  return sha256(salt + password) == expectedDigest;
}