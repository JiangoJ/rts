#pragma once

#include "Utils.h"

class LlamaEngine {

public:
  explicit LlamaEngine(const std::string& modelPath, int ctxSize = 2048);
  ~LlamaEngine();
  std::string complete(const std::string& prompt);

private:
  llama_model* model{};
  llama_context* ctx{};
  const llama_vocab* vocab{};
  int ctxSize{};
};

