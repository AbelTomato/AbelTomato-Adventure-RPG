from fastapi import APIRouter, HTTPException
from pathlib import Path
import json


router = APIRouter(tags=["config"], prefix="/config")

DATA_PATH = Path(__file__).parents[3] / "data"


def resolve_config_path(config_name: str) -> Path:
    try:
        config_path = (DATA_PATH / config_name).resolve()
    except Exception:
        raise HTTPException(status_code=400, detail="Invalid path format")
    
    if not config_path.is_relative_to(DATA_PATH):
        raise HTTPException(status_code=403, detail="Access denied: Path traversal detected")
    
    return config_path


@router.get("/{config_name}")
def get_config(config_name: str):
    config_path = resolve_config_path(config_name)
    
    if not config_path.exists() or not config_path.is_file():
        raise HTTPException(status_code=404, detail="File does not exist")
    
    with config_path.open("r", encoding="utf-8") as f:
        return json.load(f)