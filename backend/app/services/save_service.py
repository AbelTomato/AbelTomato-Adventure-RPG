import json
import re
from datetime import datetime, timezone, timedelta
from pathlib import Path
from uuid import uuid4

from app.core.versions import CURRENT_SAVE_FORMAT_VERSION, is_supported_save_format_version
from app.schemas.save import SaveCreateRequest, SaveRecord, SaveSummary, SaveUpdateRequest


PROJECT_ROOT = Path(__file__).resolve().parents[3]
SAVE_DIR = PROJECT_ROOT / "backend" / "storage" / "saves"
SAVE_ID_PATTERN = re.compile(r"^[a-zA-Z0-9_-]+$")


class SaveServiceError(Exception):
    code = "SAVE_SERVICE_ERROR"
    
class InvalidSaveIdError(SaveServiceError):
    code = "INVALID_SAVE_ID"
    
class SaveNotFoundError(SaveServiceError):
    code = "SAVE_NOT_FOUND"
    
class SaveIOError(SaveServiceError):
    code = "SAVE_IO_ERROR"


class UnsupportedSaveFormatVersionError(SaveServiceError):
    code = "UNSUPPORTED_SAVE_FORMAT_VERSION"
    
    
def _now() -> datetime:
    tz_beijing = timezone(timedelta(hours=8))
    return datetime.now(tz_beijing)

    
def _generate_save_id() -> str:
    timestamp = datetime.now(timezone.utc).strftime("%Y%m%d_%H%M%S")
    suffix = uuid4().hex[:8]
    return f"save_{timestamp}_{suffix}"


def _validate_save_id(save_id: str) -> None:
    if not SAVE_ID_PATTERN.fullmatch(save_id):
        raise InvalidSaveIdError(f"Invalid save_id: {save_id}")


def _get_save_path(save_id: str) -> Path:
    _validate_save_id(save_id)
    SAVE_DIR.mkdir(parents=True, exist_ok=True)
    path = SAVE_DIR / f"{save_id}.json"
    
    resolved_dir = SAVE_DIR.resolve()
    resolved_path = path.resolve()
    
    if not resolved_path.is_relative_to(resolved_dir):
        raise InvalidSaveIdError(f"Invalid save path: {save_id}")
    
    return path


def _write_save(record: SaveRecord) -> None:
    path = _get_save_path(record.save_id)
    try:
        path.write_text(
            record.model_dump_json(indent=2),
            encoding="utf-8"
        )
    except OSError as exc:
        raise SaveIOError(f"Failed to write save: {record.save_id}") from exc


def _read_save(path: Path) -> SaveRecord:
    try:
        data = json.loads(path.read_text(encoding="utf-8"))
        record = SaveRecord.model_validate(data)
        if not is_supported_save_format_version(record.save_format_version):
            raise UnsupportedSaveFormatVersionError(
                f"Unsupported save format version: {record.save_format_version}"
            )
        return record
    except (OSError, json.JSONDecodeError, ValueError) as exc:
        raise SaveIOError(f"Failed to read save: {path.name}") from exc


def create_save(request: SaveCreateRequest) -> SaveRecord:
    now = _now()
    record = SaveRecord(
        save_id =_generate_save_id(),
        name = request.name,
        created_at=now,
        updated_at=now,
        save_format_version=CURRENT_SAVE_FORMAT_VERSION,
        state=request.state,
        meta=request.meta,
    )
    _write_save(record)
    return record


def list_saves() -> list[SaveSummary]:
    SAVE_DIR.mkdir(parents=True, exist_ok=True)
    summaries = []
    
    for path in SAVE_DIR.glob("*.json"):
        record = _read_save(path)
        summaries.append(SaveSummary(
            save_id=record.save_id,
            name = record.name,
            created_at=record.created_at,
            updated_at=record.updated_at,
            save_format_version=record.save_format_version
        ))
        
    return sorted(summaries, key=lambda item: item.updated_at, reverse=True)


def get_save(save_id: str) -> SaveRecord:
    path = _get_save_path(save_id)
    if not path.exists():
        raise SaveNotFoundError(f"Save not found: {save_id}")
    
    return _read_save(path)


def update_save(save_id: str, request: SaveUpdateRequest) -> SaveRecord:
    old_record = get_save(save_id)
    record = SaveRecord(
        save_id=old_record.save_id,
        name=request.name,
        created_at=old_record.created_at,
        updated_at=_now(),
        save_format_version=CURRENT_SAVE_FORMAT_VERSION,
        state=request.state,
        meta=request.meta
    )
    _write_save(record)
    return record


def delete_save(save_id: str) -> None:
    path = _get_save_path(save_id)
    try:
        path.unlink()
    except FileNotFoundError as exc:
        raise SaveNotFoundError(f"Save not found: {save_id}") from exc