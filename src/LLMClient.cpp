#include "LLMClient.h"
#include "GameContext.h"

size_t writeCallback(char *contents, size_t size, size_t nmemb,
                     void *userdata) {
  // Clear the current read buffer
  auto *client = static_cast<LLMClient *>(userdata);
  client->readBuffer.clear();

  size_t total_size = size * nmemb;
  client->readBuffer.append(contents, total_size);

  std::cout << client->readBuffer << std::endl;

  client->gContext->updateGameState(nlohmann::json().parse(client->readBuffer));

  return total_size;
}

LLMClient::LLMClient() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  assert(curl);

  curl_easy_setopt(curl, CURLOPT_URL, GROQ_URL_RTS);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

  curl_slist *headers{};
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
}

void LLMClient::queryRTS(nlohmann::json &gameStateJson) {

  if (!gContext) {
    return;
  }

  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  // TODO: not sure if I want to dump the entire thing here
  msgBody = gameStateJson.dump();

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, msgBody.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, msgBody.size());
  res = curl_easy_perform(curl);

  if (res != CURLE_OK) {
    std::cout << "ERROR: " << res << std::endl;
  }
}

nlohmann::json LLMClient::getRTSJsonResponse() { return nullptr; }

void LLMClient::setGameContext(GameContext *gContext) {
  this->gContext = gContext;
}
