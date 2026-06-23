import os
from app.schemas.game import GameActionRequest, GameActionResponse
from app.services.mock_service import load_mock_action_response
from app.services.cpp_core_client import call_cpp_core

USE_MOCK_CORE = os.environ.get("USE_MOCK_CORE", "true").lower() == "true"

def execute_game_action(request: GameActionRequest) -> GameActionResponse:
    if USE_MOCK_CORE:
        return GameActionResponse.model_validate(
            load_mock_action_response("attack_success")
        )
    else:
        response_dict = call_cpp_core(request.model_dump())
        return GameActionResponse.model_validate(response_dict)
    