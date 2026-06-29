from fastapi import APIRouter

from app.core.versions import SUPPORTED_CONTRACT_VERSION, is_supported_contract_version
from app.schemas.save import SaveCreateRequest, SaveCreateResponse, SaveListResponse, SaveSummary
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