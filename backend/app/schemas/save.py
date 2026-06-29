from typing import Any
from datetime import datetime

from pydantic import BaseModel, Field

from app.core.versions import SUPPORTED_CONTRACT_VERSION
from app.schemas.game import GameError, GameState


class SaveRecord(BaseModel):
    save_id: str
    name: str
    created_at: datetime
    updated_at: datetime
    save_format_version: int
    state: GameState
    meta: dict[str, Any] = Field(default_factory=dict)


class SaveSummary(BaseModel):
    save_id: str
    name: str
    created_at: datetime
    updated_at: datetime
    save_format_version: int


class SaveCreateRequest(BaseModel):
    contract_version: int
    request_id: str
    name: str
    state: GameState
    meta: dict[str, Any] = Field(default_factory=dict)


class SaveCreateResponse(BaseModel):
    ok: bool
    contract_version: int
    request_id: str
    save: SaveRecord | None
    error: GameError | None

    @classmethod
    def unsupported_version(cls, request_id: str, contract_version: int) -> "SaveCreateResponse":
        return cls(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=request_id,
            save=None,
            error=GameError(
                code="UNSUPPORTED_VERSION",
                message=f"Unsupported contract version: {contract_version}",
                details={"contract_version": contract_version},
            ),
        )


class SaveListResponse(BaseModel):
    ok: bool
    contract_version: int
    request_id: str | None
    saves: list[SaveSummary]
    error: GameError | None


class SaveGetSingleResponse(BaseModel):
    ok: bool
    contract_version: int
    request_id: str | None
    save: SaveRecord | None
    error: GameError | None


class SaveUpdateRequest(BaseModel):
    contract_version: int
    request_id: str
    name: str
    state: GameState
    meta: dict[str, Any] = Field(default_factory=dict)


class SaveUpdateResponse(BaseModel):
    ok: bool
    contract_version: int
    request_id: str
    save: SaveRecord | None
    error: GameError | None
    
    @classmethod
    def unsupported_version(cls, request_id: str, contract_version: int) -> "SaveUpdateResponse":
        return cls(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=request_id,
            save=None,
            error=GameError(
                code="UNSUPPROTED_VERSION",
                message=f"Unsupported contract version: {contract_version}",
                details={"contract_version": contract_version},
            )
        )


class SaveDeleteResponse(BaseModel):
    ok: bool
    contract_version: int
    request_id: str | None
    deleted: bool
    save_id: str
    error: GameError | None