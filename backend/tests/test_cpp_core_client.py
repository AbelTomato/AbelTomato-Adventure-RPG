import subprocess
import pytest
import os

from app.services.cpp_core_client import call_cpp_core, DEFAULT_EXE_PATH


class FakeResult:
        returncode = 0
        stdout = '{"status": "success"}'
        stderr = ""


def test_call_cpp_core_exe_path_env(monkeypatch):
    fake_path = "C:/fake/abel_core_cli.exe"
    monkeypatch.setenv("CPP_CORE_EXE_PATH", fake_path)
    monkeypatch.setattr(os.path, "exists", lambda p: p == fake_path)
        
    actual_cmd = []
    def fake_run(cmd, **kwargs):
        nonlocal actual_cmd
        actual_cmd = cmd
        return FakeResult()
    
    monkeypatch.setattr(subprocess, "run", fake_run)
    
    payload = {"test": "data"}
    res = call_cpp_core(payload)
    
    assert actual_cmd[0] == fake_path
    assert res == {"status": "success"}
    
    
def test_call_cpp_core_uses_default_exe_path_when_env_not_set(monkeypatch):
    monkeypatch.delenv("CPP_CORE_EXE_PATH", raising=False)
    monkeypatch.setattr(os.path, "exists", lambda p: p == DEFAULT_EXE_PATH)
        
    actual_cmd = []
    def fake_run(cmd, **kwargs):
        nonlocal actual_cmd
        actual_cmd = cmd
        return FakeResult()
    
    monkeypatch.setattr(subprocess, "run", fake_run)
    
    payload = {"test": "data"}
    res = call_cpp_core(payload)
    
    assert actual_cmd[0] == DEFAULT_EXE_PATH
    assert res == {"status": "success"}
    
    
def test_call_cpp_core_raises_when_executable_not_found(monkeypatch):
    fake_path = "C:/fake/abel_core_cli.exe"
    monkeypatch.setenv("CPP_CORE_EXE_PATH", fake_path)
    payload = {"test": "data"}
    
    with pytest.raises(RuntimeError, match="C\+\+ Core executable not found"):
        call_cpp_core(payload)
    