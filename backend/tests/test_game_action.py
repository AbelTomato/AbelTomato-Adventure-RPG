from copy import deepcopy
from pathlib import Path
import pytest


def test_debug_example_action_returns_attack_request(client):
    response = client.get("/api/debug/example-action")

    assert response.status_code == 200
    assert response.json()["action"]["type"] == "attack"


def test_game_action_returns_mock_attack_success(client):
    request_payload = client.get("/api/debug/example-action").json()

    response = client.post("/api/game/action", json=request_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert payload["request_id"] == request_payload["request_id"]
    assert payload["events"][1]["type"] == "damage"
    assert payload["events"][1]["value"] == 18


def test_game_action_rejects_unsupported_version(client):
    request_payload = client.get("/api/debug/example-action").json()
    request_payload["version"] = 999

    response = client.post("/api/game/action", json=request_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is False
    assert payload["error"]["code"] == "UNSUPPORTED_VERSION"


def test_game_action_rejects_invalid_action(client):
    request_payload = deepcopy(client.get("/api/debug/example-action").json())
    request_payload["action"]["type"] = "dance"

    response = client.post("/api/game/action", json=request_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is False
    assert payload["error"]["code"] == "INVALID_ACTION"


def test_execute_game_action_can_call_cpp_core(monkeypatch, client):
    cli_path = Path(__file__).resolve().parents[2] / "build-ninja" / "abel_core_cli.exe"
    if not cli_path.exists():
        pytest.skip("abel_core_cli.exe not built")

    from app.services import core_service

    monkeypatch.setattr(core_service, "USE_MOCK_CORE", False)

    request_payload = client.get("/api/debug/example-action").json()
    response = client.post("/api/game/action", json=request_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert payload["request_id"] == request_payload["request_id"]
    assert payload["events"][1]["type"] == "damage"
    assert payload["events"][1]["value"] == 18