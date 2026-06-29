from app.services.save_service import create_save, list_saves, get_save, update_save, delete_save, SaveNotFoundError
from app.schemas.save import SaveCreateRequest, SaveUpdateRequest
from app.schemas.game import GameState, TurnState, EntityState, EntityAttrs, EntityStatus
from app.core.versions import CURRENT_SAVE_FORMAT_VERSION, SUPPORTED_CONTRACT_VERSION

import pytest


SAVE_SERVICE_PATH = "app.services.save_service"


def get_create_request(id: int):
    return SaveCreateRequest(
        contract_version = SUPPORTED_CONTRACT_VERSION,
        request_id = "create_save",
        name = f"save_{id}",
        state = GameState(
            turn = TurnState(
                round = id,
                active_entity_id = f"entity_{id}"
            ),
            entities = [
                EntityState(
                    id = f"entity_{id}",
                    name = "abel",
                    type = "player",
                    attrs = EntityAttrs(
                        hp = 100,
                        max_hp = 100,
                        physical_attack = 10,
                        physical_defense = 5
                    ),
                    status = EntityStatus(
                        alive = True
                    )
                )
            ]
        )
    )
    

def get_update_request(id: int):
    return SaveUpdateRequest(
        contract_version = SUPPORTED_CONTRACT_VERSION,
        request_id = "update_save",
        name = f"save_{id}",
        state = GameState(
            turn = TurnState(
                round = id,
                active_entity_id = f"entity_{id}"
            ),
            entities = [
                EntityState(
                    id = f"entity_{id}",
                    name = "abel",
                    type = "player",
                    attrs = EntityAttrs(
                        hp = 100,
                        max_hp = 100,
                        physical_attack = 10,
                        physical_defense = 5
                    ),
                    status = EntityStatus(
                        alive = True
                    )
                )
            ]
        )
    )


def test_create_and_file_exist_and_read_success(tmp_path, monkeypatch):
    request = get_create_request(1)
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    record = create_save(request)
    path = tmp_path / f"{record.save_id}.json"
    
    assert path.exists()
    assert record.save_format_version == CURRENT_SAVE_FORMAT_VERSION
    
    old_record = record
    record = get_save(old_record.save_id)
    
    assert record == old_record
    
    
def test_summary_saves_not_include_state(tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    for i in range(3):
        request = get_create_request(i)
        create_save(request) 
    
    summaries = list_saves()
    
    assert len(summaries) == 3
    for summary in summaries:
        data = summary.model_dump()
        assert "state" not in data
        
        
def test_update_save_service(tmp_path, monkeypatch):
    create_request = get_create_request(1)
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    record = create_save(create_request)
    save_id = record.save_id
    
    update_request = get_update_request(2)
    update_save(save_id, update_request)
    
    new_record = get_save(save_id)
    
    assert new_record.name == "save_2"
    assert new_record.save_format_version == CURRENT_SAVE_FORMAT_VERSION
    assert new_record.state.turn.round == 2
    assert new_record.state.entities[0].id == "entity_2"
    

def test_delete_save_service(tmp_path, monkeypatch):
    monkeypatch.setattr(f"{SAVE_SERVICE_PATH}.SAVE_DIR", tmp_path)
    create_request = get_create_request(1)
    record = create_save(create_request)
    save_id = record.save_id
    path = tmp_path / f"{save_id}.json"
    
    assert path.exists()
    
    delete_save(save_id)
    
    assert not path.exists()
    
    with pytest.raises(SaveNotFoundError):
        delete_save(save_id)


