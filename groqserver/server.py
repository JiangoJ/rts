from groq import Groq
from flask import Flask, request, jsonify
import random
import math

"""
Creating a simple response server
"""
app = Flask(__name__)

prompt = """ 
You are a game-playing AI specialized in real time strategy. You receive the current game state as a JSON input, and your job is to return ONLY a JSON object with your updated game state after making ONE move.
Here is the current game state:
{0}
You are currently representing the "activePlayerId", so please only update the entities belonging to that "playerId". A move is updating the active player's "entities" "position"s, which are x,y coordinate on a 2D plane.
Your GOAL to defeat all enemy entities relative to the current active player. The active player's entities can attack enemy entities when it's position intersects an enemy entity's position. Be as strategic as possible.
Here are some rules you MUST follow regarding your response: DO NOT EXPLAIN, DO NOT ADD ANY CODE. Ensure that the entire JSON structure remains but UPDATE the positions of ALL the entities belonging to the active player.

Your Move:
"""

client = Groq()


def get_groq_update(contexted_prompt):
    messages = list([{"role": "user", "content": contexted_prompt}])

    completion = client.chat.completions.create(
       model="meta-llama/llama-4-scout-17b-16e-instruct",
       messages=messages,
       temperature=1,
       max_completion_tokens=8192,
       top_p=1,
       stream=True,
       stop=None,
    )

    response = ""
    for chunk in completion:
       response += chunk.choices[0].delta.content or ""   

    return response

def dummy_update(game_state):
    active_player = game_state["activePlayer"]
    print("ACTIVE:", active_player)
    for entity in list(game_state["players"])[active_player]['entities']:
        entity['position']['x'] = math.floor(entity['position']['x'] + (random.random() * 100.0 - 50))
        entity['position']['y'] = math.floor(entity['position']['y'] + (random.random() * 100.0 - 50))
    return game_state

@app.route("/rts", methods=["POST"])
def get_rts():
    data = request.get_json()
    contexted_prompt = prompt.format(data)
    if data:
        # return jsonify(get_groq_update(contexted_prompt)), 200
        return jsonify(dummy_update(data)), 200
    else:
        print("Error")
        return jsonify({"error": "No game state received"}), 400


if __name__ == "__main__":
    app.run(debug=True)
