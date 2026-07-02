from fastapi import APIRouter

from app.core.versions import SUPPORTED_CONTRACT_VERSION, is_supported_contract_version
from app.schemas.save import (
    SaveCreateRequest,
    SaveCreateResponse,
    SaveDeleteResponse,
    SaveGetSingleResponse,
    SaveListResponse,
    SaveSummary,
    SaveUpdateRequest,
    SaveUpdateResponse,
)
from app.services import save_service
from app.schemas.game import GameError


router = APIRouter(tags=["save"], prefix="/saves")


@router.post("", response_model=SaveCreateResponse)
def create_save_endpoint(request: SaveCreateRequest) -> SaveCreateResponse:
    if not is_supported_contract_version(request.contract_version):
        return SaveCreateResponse.unsupported_version(
            request.request_id,
            request.contract_version,
        )

    record = save_service.create_save(request)
    return SaveCreateResponse(
        ok=True,
        contract_version=SUPPORTED_CONTRACT_VERSION,
        request_id=request.request_id,
        save=record,
        error=None,
    )
    

@router.get("", response_model=SaveListResponse)
def get_save_list_endpoint() -> SaveListResponse:
    try:
        saves: list[SaveSummary] = save_service.list_saves()
        return SaveListResponse(
            ok=True,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=None,
            saves=saves,
            error=None
        )
    except (save_service.UnsupportedSaveFormatVersionError, save_service.SaveIOError) as exc:
        return SaveListResponse(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=None,
            saves=[],
            error=GameError(
                code=exc.code,
                message=f"Have problems in getting saves list: {exc}"
            )
        )


@router.get("/{save_id}", response_model=SaveGetSingleResponse)
def get_save_endpoint(save_id: str) -> SaveGetSingleResponse:
    try:
        record = save_service.get_save(save_id)
        return SaveGetSingleResponse(
            ok=True,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=None,
            save=record,
            error=None,
        )
    except save_service.SaveServiceError as exc:
        return SaveGetSingleResponse(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=None,
            save=None,
            error=GameError(
                code=exc.code,
                message=str(exc),
            ),
        )


@router.put("/{save_id}", response_model=SaveUpdateResponse)
def update_save_endpoint(save_id: str, request: SaveUpdateRequest) -> SaveUpdateResponse:
    if not is_supported_contract_version(request.contract_version):
        return SaveUpdateResponse.unsupported_version(
            request.request_id,
            request.contract_version,
        )

    try:
        record = save_service.update_save(save_id, request)
        return SaveUpdateResponse(
            ok=True,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=request.request_id,
            save=record,
            error=None,
        )
    except save_service.SaveServiceError as exc:
        return SaveUpdateResponse(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=request.request_id,
            save=None,
            error=GameError(
                code=exc.code,
                message=str(exc),
            ),
        )


@router.delete("/{save_id}", response_model=SaveDeleteResponse)
def delete_save_endpoint(save_id: str) -> SaveDeleteResponse:
    try:
        save_service.delete_save(save_id)
        return SaveDeleteResponse(
            ok=True,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=None,
            deleted=True,
            save_id=save_id,
            error=None,
        )
    except save_service.SaveServiceError as exc:
        return SaveDeleteResponse(
            ok=False,
            contract_version=SUPPORTED_CONTRACT_VERSION,
            request_id=None,
            deleted=False,
            save_id=save_id,
            error=GameError(
                code=exc.code,
                message=str(exc),
            ),
        )