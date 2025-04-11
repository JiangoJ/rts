#include "LlamaEngine.h"
#include "Constants.h"

LlamaEngine::LlamaEngine(const std::string &modelPath, int ctxSize)
    : ctxSize(ctxSize) {
  // Use Path to load model
  llama_backend_init();
  llama_model_params modelParams = llama_model_default_params();
  model = llama_model_load_from_file(modelPath.c_str(), modelParams);
  if (!model) {
    throw std::runtime_error("Failed to load model");
  }

  llama_context_params ctxParams = llama_context_default_params();
  ctxParams.n_ctx = MAX_CTX;
  ctxParams.n_batch = MAX_N_BATCH;
  ctx = llama_init_from_model(model, ctxParams);
  if (!ctx) {
    throw std::runtime_error("Failed to create context");
  }

  vocab = llama_model_get_vocab(model);
}

LlamaEngine::~LlamaEngine() {
  llama_free(ctx);
  llama_model_free(model);
  llama_backend_free();
}

std::string LlamaEngine::complete(const std::string &prompt) {

  std::cout << "PROOMPTING: " << std::endl;

  // nPrompt gives you the size of the prompt
  const auto nPrompt = -llama_tokenize(vocab, prompt.c_str(), prompt.size(),
                                       NULL, 0, true, true);

  std::vector<llama_token> promptTokens(nPrompt);

  // Tokenize the prompt into promptTokens using vocab 
  if (llama_tokenize(vocab, prompt.c_str(), prompt.size(), promptTokens.data(),
                     promptTokens.size(), true, true) < 0) {

    std::cout << "error: failed to tokenize the prompt" << std::endl;
  }

  // Initialize Sampler: This is needed to turn the decoded embeddings into an actual word output
  auto sparams = llama_sampler_chain_default_params();
  llama_sampler *smpl = llama_sampler_chain_init(sparams);
  llama_sampler_chain_add(smpl, llama_sampler_init_greedy());

  // Printing the tokenized prompt 1 by 1
  for (auto id : promptTokens) {
    char buf[128];
    int n = llama_token_to_piece(vocab, id, buf, sizeof(buf), 0, true);
    if (n < 0) {
      std::cout << "error: failed to convert token to piece" << std::endl;
    }
    std::string s(buf, n);
    std::cout << s;
  }

  std::cout << "\n" << std::endl;

  // Pass the prompt data as a single batch into the model
  llama_batch batch =
      llama_batch_get_one(promptTokens.data(), promptTokens.size());

  std::string response;

  int predictedTokens = 0;
  llama_token newTokenId;

  // Each loop here will return a single response token
  while (predictedTokens <= MAX_RESPONSE_TOKENS) {

    if (llama_decode(ctx, batch)) {
      std::cout << "Failed to eval, return code" << __func__ << std::endl;
    }

    // Sampler will determine the next token from the models raw ouput (logits)
    newTokenId = llama_sampler_sample(smpl, ctx, -1);

    // End of the response is done
    if (llama_vocab_is_eog(vocab, newTokenId)) {
      break;
    }

    char buf[128];
    int n = llama_token_to_piece(vocab, newTokenId, buf, sizeof(buf), 0, true);
    if (n < 0) {
      std::cout << "failed to convert token to piece " << __func__ << std::endl;
    }

    std::string s(buf, n);
    std::cout << s;

    // Prepare the next response token
    batch = llama_batch_get_one(&newTokenId, 1);

    predictedTokens++;
  }

  std::cout << std::endl;

  return "END";
}

