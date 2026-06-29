from app.core.versions import CURRENT_SAVE_FORMAT_VERSION, SUPPORTED_CONTRACT_VERSION
from app.schemas.game import EntityAttrs, EntityState, EntityStatus, GameState, TurnState
from app.schemas.save import SaveCreateRequest


SAVE_SERVICE_PATH = "app.services.save_service"


def get_create_request(id: int) -> SaveCreateRequest:
    return SaveCreateRequest(
        contract_version=SUPPORTED_CONTRACT_VERSION,
        request_id=f"create_save_{id}",
        name=f"save_{id}",
        state=GameState(
            turn=TurnState(
                round=id,
                active_entity_id=f"entity_{id}",
            ),
            entities=[
                EntityState(
                    id=f"entity_{id}",
                    name="abel",
                    type="player",
                    attrs=EntityAttrs(
                        hp=100,
                        max_hp=100,
                        physical_attack=10,
                        physical_defense=5,
                    ),
                    status=EntityStatus(alive=True),
                )
            ],
        ),
    )


def test_create_save_endpoint_returns_ok_and_save_format_version(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    request_payload = get_create_request(1).model_dump(mode="json")

    response = client.post("/api/saves", json=request_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert payload["contract_version"] == SUPPORTED_CONTRACT_VERSION
    assert payload["request_id"] == "create_save_1"
    assert payload["save"]["name"] == "save_1"
    assert payload["save"]["save_format_version"] == CURRENT_SAVE_FORMAT_VERSION


def test_create_save_endpoint_rejects_unsupported_contract_version(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    request_payload = get_create_request(1).model_dump(mode="json")
    request_payload["contract_version"] = 999

    response = client.post("/api/saves", json=request_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is False
    assert payload["save"] is None
    assert payload["error"]["code"] == "UNSUPPORTED_VERSION"
    assert payload["error"]["details"]["contract_version"] == 999


def test_list_saves_endpoint_returns_summaries_without_state(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    for i in range(3):
        client.post("/api/saves", json=get_create_request(i).model_dump(mode="json"))

    response = client.get("/api/saves")
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert len(payload["saves"]) == 3
    for summary in payload["saves"]:
        assert "save_id" in summary
        assert "name" in summary
        assert "save_format_version" in summary
        assert "state" not in summary