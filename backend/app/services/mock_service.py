import json
from pathlib import Path
from typing import Any


PROJECT_ROOT = Path(__file__).resolve().parents[3]
EXAMPLES_DIR = PROJECT_ROOT / "examples"


def load_example_action(action_name: str) -> dict[str, Any]:
    return _load_json(EXAMPLES_DIR / "requests" / f"{action_name}.json")


def load_mock_action_response(response_name: str) -> dict[str, Any]:
    return _load_json(EXAMPLES_DIR / "responses" / f"{response_name}.json")


def _load_json(path: Path) -> dict[str, Any]:
    with path.open("r", encoding="utf-8") as file:
        return json.load(file)