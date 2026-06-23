from fastapi import APIRouter, HTTPException, status

from app.schemas.game import GameActionRequest, GameActionResponse
from app.services.core_service import execute_game_action
from app.services.mock_service import load_example_action

router = APIRouter(tags=["game"])


@router.post("/game/action", response_model=GameActionResponse)
def handle_game_action(request: GameActionRequest) -> GameActionResponse:
    if request.version != 1:
        return GameActionResponse.unsupported_version(request.request_id, request.version)
    if request.action.type != "attack":
        return GameActionResponse.invalid_action(request.request_id, request.action.type)
    
    return execute_game_action(request)


@router.get("/debug/example-action")
def get_example_action() -> dict:
    try:
        return load_example_action("attack")
    except FileNotFoundError as exc:
        raise HTTPException(
            status_code=status.HTTP_404_NOT_FOUND,
            detail="Example action not found"
        ) from exc