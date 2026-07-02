import json
from app.api.config import DATA_PATH
import pytest
from fastapi import HTTPException
from app.api.config import resolve_config_path


def test_get_config_api(tmp_path, monkeypatch, client):
    fake_config = {"status": "ok", "job": "witch"}
    fake_config_name = "config.json"
    
    fake_file = tmp_path / fake_config_name
    fake_file.write_text(json.dumps(fake_config), encoding="utf-8")
    
    monkeypatch.setattr("app.api.config.DATA_PATH", tmp_path)
    
    response = client.get(f"/api/config/{fake_config_name}")
    payload = response.json()
    
    assert response.status_code == 200
    assert payload == fake_config
    
    
def test_resolve_config_path_rejects_path_traversal(tmp_path, monkeypatch):
    data_dir = tmp_path / "data"
    data_dir.mkdir()
    
    monkeypatch.setattr("app.api.config.DATA_PATH", data_dir)
    
    with pytest.raises(HTTPException) as exc_info:
        resolve_config_path("../users.json")
        
    assert exc_info.value.status_code == 404
    
    
def test_respond_to_wrong_path_by_symlink(tmp_path, monkeypatch, client):
    data_dir = tmp_path / "data"
    outside_dir = tmp_path / "outside"
    data_dir.mkdir()
    outside_dir.mkdir()

    outside_file = outside_dir / "secret.json"
    outside_file.write_text("{}", encoding="utf-8")

    link_file = data_dir / "config.json"

    try:
        link_file.symlink_to(outside_file)
    except OSError as exc:
        if getattr(exc, "winerror", None) == 1314:
            pytest.skip("Windows 当前环境无权限创建 symlink，跳过路径逃逸测试")
        raise

    monkeypatch.setattr("app.api.config.DATA_PATH", data_dir)

    response = client.get("/api/config/config.json")

    assert response.status_code == 403


def test_get_config_api_rejects_non_whitelisted_json(tmp_path, monkeypatch, client):
    fake_config = {"status": "ok"}
    fake_file = tmp_path / "fake_config.json"
    fake_file.write_text(json.dumps(fake_config), encoding="utf-8")

    monkeypatch.setattr("app.api.config.DATA_PATH", tmp_path)

    response = client.get("/api/config/fake_config.json")

    assert response.status_code == 404


def test_get_config_version_returns_version_from_config(tmp_path, monkeypatch, client):
    fake_config = {"config_version": 7, "Settings": {}}
    fake_file = tmp_path / "config.json"
    fake_file.write_text(json.dumps(fake_config), encoding="utf-8")

    monkeypatch.setattr("app.api.config.DATA_PATH", tmp_path)

    response = client.get("/api/config/version")
    payload = response.json()

    assert response.status_code == 200
    assert payload == {
        "config_version": 7,
        "source": "config.json",
    }


def test_get_config_version_returns_default_when_config_has_no_version(tmp_path, monkeypatch, client):
    fake_config = {"Settings": {}}
    fake_file = tmp_path / "config.json"
    fake_file.write_text(json.dumps(fake_config), encoding="utf-8")

    monkeypatch.setattr("app.api.config.DATA_PATH", tmp_path)

    response = client.get("/api/config/version")
    payload = response.json()

    assert response.status_code == 200
    assert payload == {
        "config_version": 1,
        "source": "default",
    }
    
    
def test_respond_to_not_exits_path(tmp_path, monkeypatch, client):
    response = client.get("/ghost_config.json")
    
    assert response.status_code == 404