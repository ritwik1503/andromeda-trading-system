#pragma once

#include <string>

namespace util {

// Reads an API key by first checking the environment variable `env_var`,
// then falling back to parsing a KEY=VALUE secrets file at `secrets_path`.
// Returns empty string if not found.
std::string read_api_key(const char* env_var, const char* secrets_path);

}  // namespace util

