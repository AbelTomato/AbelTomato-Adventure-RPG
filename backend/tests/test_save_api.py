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


def test_get_save_endpoint_returns_full_save(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    create_response = client.post("/api/saves", json=get_create_request(1).model_dump(mode="json"))
    save_id = create_response.json()["save"]["save_id"]

    response = client.get(f"/api/saves/{save_id}")
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert payload["save"]["save_id"] == save_id
    assert payload["save"]["state"]["turn"]["round"] == 1
    assert payload["save"]["state"]["entities"][0]["id"] == "entity_1"


def test_update_save_endpoint_overwrites_save(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    create_response = client.post("/api/saves", json=get_create_request(1).model_dump(mode="json"))
    original = create_response.json()["save"]
    save_id = original["save_id"]
    update_payload = get_create_request(2).model_dump(mode="json")
    update_payload["request_id"] = "update_save_2"
    update_payload["name"] = "updated_save"
    update_payload["meta"] = {"source": "test"}

    response = client.put(f"/api/saves/{save_id}", json=update_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert payload["request_id"] == "update_save_2"
    assert payload["save"]["save_id"] == save_id
    assert payload["save"]["created_at"] == original["created_at"]
    assert payload["save"]["name"] == "updated_save"
    assert payload["save"]["state"]["turn"]["round"] == 2
    assert payload["save"]["meta"] == {"source": "test"}

    get_response = client.get(f"/api/saves/{save_id}")
    assert get_response.json()["save"]["name"] == "updated_save"


def test_update_save_endpoint_rejects_unsupported_contract_version(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    create_response = client.post("/api/saves", json=get_create_request(1).model_dump(mode="json"))
    save_id = create_response.json()["save"]["save_id"]
    update_payload = get_create_request(2).model_dump(mode="json")
    update_payload["contract_version"] = 999

    response = client.put(f"/api/saves/{save_id}", json=update_payload)
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is False
    assert payload["save"] is None
    assert payload["error"]["code"] == "UNSUPPORTED_VERSION"
    assert payload["error"]["details"]["contract_version"] == 999


def test_delete_save_endpoint_deletes_save(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    create_response = client.post("/api/saves", json=get_create_request(1).model_dump(mode="json"))
    save_id = create_response.json()["save"]["save_id"]

    response = client.delete(f"/api/saves/{save_id}")
    payload = response.json()

    assert response.status_code == 200
    assert payload["ok"] is True
    assert payload["deleted"] is True
    assert payload["save_id"] == save_id

    get_response = client.get(f"/api/saves/{save_id}")
    get_payload = get_response.json()
    assert get_payload["ok"] is False
    assert get_payload["error"]["code"] == "SAVE_NOT_FOUND"


def test_save_endpoints_return_save_not_found(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    update_payload = get_create_request(1).model_dump(mode="json")

    get_response = client.get("/api/saves/missing_save")
    update_response = client.put("/api/saves/missing_save", json=update_payload)
    delete_response = client.delete("/api/saves/missing_save")

    assert get_response.json()["error"]["code"] == "SAVE_NOT_FOUND"
    assert update_response.json()["error"]["code"] == "SAVE_NOT_FOUND"
    assert delete_response.json()["error"]["code"] == "SAVE_NOT_FOUND"


def test_save_endpoints_return_invalid_save_id(client, tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    update_payload = get_create_request(1).model_dump(mode="json")

    get_response = client.get("/api/saves/bad.id")
    update_response = client.put("/api/saves/bad.id", json=update_payload)
    delete_response = client.delete("/api/saves/bad.id")

    assert get_response.json()["error"]["code"] == "INVALID_SAVE_ID"
    assert update_response.json()["error"]["code"] == "INVALID_SAVE_ID"
    assert delete_response.json()["error"]["code"] == "INVALID_SAVE_ID"