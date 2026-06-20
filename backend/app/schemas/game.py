from typing import Any

from pydantic import BaseModel, Field


class GameAction(BaseModel):
    type: str
    actor_id: str
    target_id: str | None = None
    params: dict[str, Any] = Field(default_factory=dict)


class TurnState(BaseModel):
    round: int
    active_entity_id: str


class EntityAttrs(BaseModel):
    hp: int
    max_hp: int
    physical_attack: int
    physical_defense: int


class EntityStatus(BaseModel):
    alive: bool


class EntityState(BaseModel):
    id: str
    name: str
    type: str
    attrs: EntityAttrs
    status: EntityStatus


class GameState(BaseModel):
    turn: TurnState
    entities: list[EntityState]


class GameActionRequest(BaseModel):
    version: int
    request_id: str
    action: GameAction
    state: GameState
    meta: dict[str, Any] = Field(default_factory=dict)


class GameError(BaseModel):
    code: str
    message: str
    details: dict[str, Any] = Field(default_factory=dict)


class GameEvent(BaseModel):
    seq: int
    type: str

    model_config = {"extra": "allow"}


class GameActionResponse(BaseModel):
    ok: bool
    version: int
    request_id: str
    state: GameState | None
    events: list[GameEvent]
    error: GameError | None

    @classmethod
    def unsupported_version(cls, request_id: str, version: int) -> "GameActionResponse":
        return cls(
            ok=False,
            version=1,
            request_id=request_id,
            state=None,
            events=[],
            error=GameError(
                code="UNSUPPORTED_VERSION",
                message=f"Unsupported contract version: {version}",
                details={"version": version},
            ),
        )

    @classmethod
    def invalid_action(cls, request_id: str, action_type: str) -> "GameActionResponse":
        return cls(
            ok=False,
            version=1,
            request_id=request_id,
            state=None,
            events=[],
            error=GameError(
                code="INVALID_ACTION",
                message=f"Unsupported action type: {action_type}",
                details={"action_type": action_type},
            ),
        )