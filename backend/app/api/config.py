from fastapi import APIRouter, HTTPException
from pathlib import Path
import json


router = APIRouter(tags=["config"], prefix="/config")

DATA_PATH = Path(__file__).parents[3] / "data"
ALLOWED_CONFIG_FILES = {
    "config.json",
    "race.json",
    "job.json",
    "skill.json",
    "effect.json",
    "buff.json",
}
DEFAULT_CONFIG_VERSION = 1


def resolve_config_path(config_name: str) -> Path:
    if config_name not in ALLOWED_CONFIG_FILES:
        raise HTTPException(status_code=404, detail="Config file is not exposed")

    try:
        config_path = (DATA_PATH / config_name).resolve()
    except Exception:
        raise HTTPException(status_code=400, detail="Invalid path format")
    
    if not config_path.is_relative_to(DATA_PATH.resolve()):
        raise HTTPException(status_code=403, detail="Access denied: Path traversal detected")
    
    return config_path


@router.get("/version")
def get_config_version():
    config_path = resolve_config_path("config.json")

    if not config_path.exists() or not config_path.is_file():
        return {
            "config_version": DEFAULT_CONFIG_VERSION,
            "source": "default",
        }

    with config_path.open("r", encoding="utf-8") as f:
        config_data = json.load(f)

    config_version = config_data.get("config_version", DEFAULT_CONFIG_VERSION)
    return {
        "config_version": config_version,
        "source": "config.json" if "config_version" in config_data else "default",
    }


@router.get("/{config_name}")
def get_config(config_name: str):
    config_path = resolve_config_path(config_name)
    
    if not config_path.exists() or not config_path.is_file():
        raise HTTPException(status_code=404, detail="File does not exist")
    
    with config_path.open("r", encoding="utf-8") as f:
        return json.load(f)