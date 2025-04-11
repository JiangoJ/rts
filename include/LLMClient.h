#pragma once
#include "Utils.h"

/**
 * The purpose of this LLMCLient is to send requests to the Python Groq Server
 * and receive JSON responses
 */

class LLMClient {
private:
  std::string endpoint;
  CURL *curl;
  CURLcode res;
  std::string readBuffer{};

public:
  LLMClient();
  ~LLMClient() { curl_easy_cleanup(curl); }
  void queryRTS(const nlohmann::json &gameStateJson);
  nlohmann::json getRTSJsonResponse();
};
