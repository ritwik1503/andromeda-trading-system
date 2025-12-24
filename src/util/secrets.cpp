#include <util/secrets.h>

#include <cstdlib>
#include <fstream>
#include <string>

namespace util {

namespace {
std::string trim(const std::string& s) {
  const auto b = s.find_first_not_of(" \t\r\n");
  if (b == std::string::npos) return {};
  const auto e = s.find_last_not_of(" \t\r\n");
  return s.substr(b, e - b + 1);
}
}

std::string read_api_key(const char* env_var, const char* secrets_path) {
  if (!env_var) return {};
  // 1) environment
  if (const char* v = std::getenv(env_var)) {
    std::string s = trim(v);
    if (!s.empty()) return s;
  }
  // 2) secrets file (KEY=VALUE lines)
  if (!secrets_path) return {};
  std::ifstream in(secrets_path);
  if (!in) return {};
  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (line.empty() || line[0] == '#') continue;
    auto pos = line.find('=');
    if (pos == std::string::npos) continue;
    auto key = trim(line.substr(0, pos));
    auto val = trim(line.substr(pos + 1));
    if (key == env_var && !val.empty()) return val;
  }
  return {};
}

}  // namespace util

