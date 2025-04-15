from groq import Groq
from flask import Flask, request, jsonify
import json
import random
import math
import instructor
from pydantic import BaseModel

"""
Creating a simple response server
"""
app = Flask(__name__)

"""
Structured Output:
"""
class Position(BaseModel):
    x: float
    y: float

class Entity(BaseModel):
    entityId: str
    position: Position

class Response(BaseModel):
    activePlayer: int
    entities: list[Entity]
    reason: str


prompt = """ 
Here is the current game state:
{0}
Your Move:
"""

system_prompt = """
You are a expert real time strategy player. Your job is to position the active player's entities in the optimal positions to defeat the enemy players. To attack an enemy entity, your entity must be at the same position as the enemy entity. 
Keep note that the boundaries for the movable state are x [0, 800] and y [0, 600]. 
You are currently representing the "activePlayerId", so please only update the entities belonging to that "playerId". A move is updating the active player's "entities" "position"s, which are x,y coordinate on a 2D plane.
ONLY RETURN THE VALID JSON OUTPUT. The definition of a valid JSON: Contains a field for the activePlayer, a field called "entities" for the list of updated entity structures. Please also include a short "reason" for the move. 
"""

client = instructor.from_groq(Groq(), mode=instructor.Mode.JSON)

def get_groq_update(contexted_prompt):

    model = "meta-llama/llama-4-maverick-17b-128e-instruct"

    response = client.chat.completions.create (
       model=model,
       # Structured output
       response_model=Response,
       messages=[{"role": "system", "content": system_prompt},
                     {"role": "user", "content": contexted_prompt}],
       temperature=1,
       max_completion_tokens=8192,
       top_p=1,
    )
    print("RESPONSE:", type(response), response)

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
    data = request.get_data()
    print("DATA:", data)
    contexted_prompt = prompt.format(data)
    if data:
        groq_response = get_groq_update(contexted_prompt).model_dump_json()
        print("GROQ_RESP:", groq_response)
        return groq_response, 200

        # DUMMY
        # data = request.get_json()
        # return jsonify(dummy_update(data)), 200
    else:
        print("Error")
        return jsonify({"error": "No game state received"}), 400


if __name__ == "__main__":
    app.run(debug=True)
