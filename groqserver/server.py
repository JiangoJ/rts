from groq import Groq
from flask import Flask, request, jsonify
import json
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
Keep note that the boundaries for the movable state are x [0, 800] and y [0, 600]
You are currently representing the "activePlayerId", so please only update the entities belonging to that "playerId". A move is updating the active player's "entities" "position"s, which are x,y coordinate on a 2D plane.
Your GOAL to defeat all enemy entities relative to the current active player by ENCIRCLING THE POSITIONS OF THE ENEMY ENTITIES. An enemy entity is any entity belonging to another player id.
Here are some rules you MUST follow regarding your response: DO NOT EXPLAIN, DO NOT ADD ANY CODE. Ensure that the entire JSON structure remains but UPDATE the positions of ALL the entities belonging to the active player. Do not surround the JSON in anything, just the JSON.

Your Move:
"""

client = Groq()

def get_groq_update(contexted_prompt):
    messages = list([{"role": "user", "content": contexted_prompt}])

    completion = client.chat.completions.create(
       model="meta-llama/llama-4-maverick-17b-128e-instruct",
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
        entity['position']['x'] = math.floor(entity['position']['x'] + (random.random() * 500.0 - 250))
        entity['position']['y'] = math.floor(entity['position']['y'] + (random.random() * 500.0 - 250))
    return game_state

@app.route("/rts", methods=["POST"])
def get_rts():
    data = request.get_json()
    print("DATA: ", data)
    contexted_prompt = prompt.format(data)
    if data:
        # groq_update_str = get_groq_update(contexted_prompt)
        # print(groq_update_str)
        # groq_update = json.loads(groq_update_str)
        # return jsonify(groq_update), 200
        return jsonify(dummy_update(data)), 200
    else:
        print("Error")
        return jsonify({"error": "No game state received"}), 400


if __name__ == "__main__":
    app.run(debug=True)
