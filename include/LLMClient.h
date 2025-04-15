#pragma once
#include "Utils.h"

/**
 * The purpose of this LLMCLient is to send requests to the Python Groq Server
 * and receive JSON responses
 */

class GameContext;

class LLMClient {

public:
  std::string endpoint;
  CURL *curl;
  CURLcode res;

  std::string msgBody{};
  std::string readBuffer{};

  GameContext* gContext{nullptr};

  LLMClient();
  ~LLMClient() { curl_easy_cleanup(curl); }
  void queryRTS(nlohmann::json &gameStateJson);
  void setGameContext(GameContext* gContext);
};
