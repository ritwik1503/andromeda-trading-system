#include <ingestion/finnhub_http.h>

#include <curl/curl.h>

#include <stdexcept>
#include <string>

namespace ingestion {

namespace {
size_t write_to_string(void* contents, size_t size, size_t nmemb, void* userp) {
  auto total = size * nmemb;
  static_cast<std::string*>(userp)->append(static_cast<char*>(contents), total);
  return total;
}
}

FinnhubHttpClient::FinnhubHttpClient(std::string api_key, std::string base_url)
    : api_key_(std::move(api_key)), base_url_(std::move(base_url)) {}

std::string FinnhubHttpClient::get_quote(const std::string& symbol) const {
  CURL* curl = curl_easy_init();
  if (!curl) throw std::runtime_error("curl_easy_init failed");

  std::string response;

  // URL-encode query params
  char* esc_symbol = curl_easy_escape(curl, symbol.c_str(), static_cast<int>(symbol.size()));
  char* esc_token = curl_easy_escape(curl, api_key_.c_str(), static_cast<int>(api_key_.size()));
  if (!esc_symbol || !esc_token) {
    if (esc_symbol) curl_free(esc_symbol);
    if (esc_token) curl_free(esc_token);
    curl_easy_cleanup(curl);
    throw std::runtime_error("curl_easy_escape failed");
  }

  std::string url = base_url_ + "/quote?symbol=" + esc_symbol + "&token=" + esc_token;
  curl_free(esc_symbol);
  curl_free(esc_token);

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

  // Safe defaults: verify TLS
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

  CURLcode res = curl_easy_perform(curl);
  long status = 0;
  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK) {
    throw std::runtime_error(std::string("curl_easy_perform failed: ") + curl_easy_strerror(res));
  }
  if (status < 200 || status >= 300) {
    throw std::runtime_error("HTTP error: status=" + std::to_string(status));
  }

  return response;
}

}  // namespace ingestion

