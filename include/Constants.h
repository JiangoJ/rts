#pragma once

constexpr static int WIN_HEIGHT = 600;
constexpr static int WIN_WIDTH = 800;

constexpr static int RENDER_TOOLBUTTON_WIDTH = 45;
constexpr static int RENDER_TOOLBUTTON_HEIGHT = 30;

constexpr static int TROOP_RENDER_RADIUS = 10;
constexpr static int TROOP_HEALTH_PADDING = 10;

constexpr static const char* GROQ_URL_RTS = "127.0.0.1:5000/rts";
constexpr static float QUERY_DELTA_S = 2;

constexpr static const char* TINY_LLAMA = "/Users/justin/dev/rts/models/tinyllama-1.1b-chat-v1.0.Q4_K_M.gguf";
constexpr static const char* PHI = "/Users/justin/dev/rts/models/phi-2.Q8_0.gguf";
constexpr static const char* MISTRAL = "/Users/justin/dev/rts/models/mistral-7b-instruct-v0.1.Q5_K_M.gguf";

constexpr static int MAX_CTX = 2048;
constexpr static int MAX_N_BATCH = 2048;
constexpr static int MAX_RESPONSE_TOKENS = 2048;
