#include "LLMClient.h"

size_t writeCallback(char *contents, size_t size, size_t nmemb,
                     std::string *response) {
  // Clear the current read buffer
  response->clear();

  size_t total_size = size * nmemb;
  response->append(contents, total_size);
  return total_size;
}

LLMClient::LLMClient() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  assert(curl);

  curl_easy_setopt(curl, CURLOPT_URL, GROQ_URL_RTS);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

  curl_slist *headers{};
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
}

void LLMClient::queryRTS(const nlohmann::json &gameStateJson) {

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  // TODO: not sure if I want to dump the entire thing here
  std::string state = gameStateJson.dump();

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, state.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, state.size());
  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    std::cout << "ERROR: " << res << std::endl;
  }
}

nlohmann::json LLMClient::getRTSJsonResponse() {
  auto resJson = nlohmann::json().parse(readBuffer);
  return resJson;
}
