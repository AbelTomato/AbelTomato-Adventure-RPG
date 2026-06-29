from typing import Any

from pydantic import BaseModel, Field

from app.core.versions import SUPPORTED_CONTRACT_VERSION


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
    entities: list[EntityState]     #TODO: 此处暂时使用列表以简化逻辑，后续考虑修改为字典或保留索引等其他方案


class GameActionRequest(BaseModel):
    contract_version: int
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

    model_config = {"extra": "allow"}   # TODO: 后续需要增加Pydantic校验模型


class GameActionResponse(BaseModel):
    ok: bool
    contract_version: int
    request_id: str
    state: GameState | None
    events: list[GameEvent]
    error: GameError | None

    @classmethod
    def unsupported_version(cls, request_id: str, contract_version: int) -> "GameActionResponse":
        return cls(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=request_id,
            state=None,
            events=[],
            error=GameError(
                code="UNSUPPORTED_VERSION",
                message=f"Unsupported contract version: {contract_version}",
                details={"contract_version": contract_version},
            ),
        )

    @classmethod
    def invalid_action(cls, request_id: str, action_type: str) -> "GameActionResponse":
        return cls(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=request_id,
            state=None,
            events=[],
            error=GameError(
                code="INVALID_ACTION",
                message=f"Unsupported action type: {action_type}",
                details={"action_type": action_type},
            ),
        )