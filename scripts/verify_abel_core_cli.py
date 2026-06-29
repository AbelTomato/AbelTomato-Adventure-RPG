from __future__ import annotations

import argparse
import json
import subprocess
import sys
from copy import deepcopy
from pathlib import Path
from typing import Any


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_EXE = ROOT / "build-ninja" / "abel_core_cli.exe"
ATTACK_REQUEST = ROOT / "examples" / "requests" / "attack.json"


def run_core(exe: Path, payload: bytes) -> dict[str, Any]:
    result = subprocess.run(
        [str(exe)],
        input=payload,
        capture_output=True,
        check=False,
    )

    if result.returncode != 0:
        stderr = result.stderr.decode("utf-8", errors="replace")
        raise AssertionError(f"CLI exited with {result.returncode}: {stderr}")

    stdout = result.stdout.decode("utf-8")
    try:
        return json.loads(stdout)
    except json.JSONDecodeError as exc:
        raise AssertionError(f"CLI stdout is not valid JSON: {stdout}") from exc


def encode(payload: dict[str, Any]) -> bytes:
    return json.dumps(payload, ensure_ascii=False).encode("utf-8")


def load_attack_request() -> dict[str, Any]:
    return json.loads(ATTACK_REQUEST.read_text(encoding="utf-8"))


def assert_error(response: dict[str, Any], code: str) -> None:
    assert response["ok"] is False
    assert response["state"] is None
    assert response["events"] == []
    assert response["error"]["code"] == code


def test_normal_attack(exe: Path) -> None:
    response = run_core(exe, ATTACK_REQUEST.read_bytes())

    assert response["ok"] is True
    assert response["contract_version"] == 1
    assert response["request_id"] == "req_attack_001"
    assert response["error"] is None

    events = response["events"]
    assert [event["type"] for event in events] == [
        "attack_started",
        "damage",
        "hp_changed",
    ]
    assert events[1]["value"] == 18
    assert events[2]["from"] == 50
    assert events[2]["to"] == 32

    entities = response["state"]["entities"]
    slime = next(entity for entity in entities if entity["id"] == "slime_1")
    assert slime["attrs"]["hp"] == 32
    assert slime["status"]["alive"] is True


def test_invalid_json(exe: Path) -> None:
    response = run_core(exe, b"{bad json")
    assert_error(response, "INVALID_JSON")


def test_missing_schema(exe: Path) -> None:
    response = run_core(exe, encode({"contract_version": 1, "request_id": "req_missing"}))
    assert_error(response, "INVALID_SCHEMA")


def test_unsupported_version(exe: Path) -> None:
    request = load_attack_request()
    request["contract_version"] = 999

    response = run_core(exe, encode(request))
    assert_error(response, "UNSUPPORTED_VERSION")
    assert response["request_id"] == "req_attack_001"
    assert response["error"]["details"]["contract_version"] == 999


def test_invalid_action(exe: Path) -> None:
    request = load_attack_request()
    request["action"]["type"] = "dance"

    response = run_core(exe, encode(request))
    assert_error(response, "INVALID_ACTION")
    assert response["request_id"] == "req_attack_001"
    assert response["error"]["details"]["action_type"] == "dance"


def test_missing_actor(exe: Path) -> None:
    request = load_attack_request()
    request["action"]["actor_id"] = "missing_player"

    response = run_core(exe, encode(request))
    assert_error(response, "ENTITY_NOT_FOUND")
    assert response["request_id"] == "req_attack_001"
    assert response["error"]["details"]["actor_id"] == "missing_player"


def test_missing_target(exe: Path) -> None:
    request = load_attack_request()
    request["action"]["target_id"] = "missing_slime"

    response = run_core(exe, encode(request))
    assert_error(response, "ENTITY_NOT_FOUND")
    assert response["request_id"] == "req_attack_001"
    assert response["error"]["details"]["target_id"] == "missing_slime"


def test_death_event(exe: Path) -> None:
    request = load_attack_request()
    request["state"]["entities"][1]["attrs"]["hp"] = 10

    response = run_core(exe, encode(request))
    assert response["ok"] is True

    entities = response["state"]["entities"]
    slime = next(entity for entity in entities if entity["id"] == "slime_1")
    assert slime["attrs"]["hp"] == 0
    assert slime["status"]["alive"] is False

    events = response["events"]
    assert events[-1]["type"] == "death"
    assert events[-1]["entity_id"] == "slime_1"


TESTS = [
    ("normal attack", test_normal_attack),
    ("invalid json", test_invalid_json),
    ("missing schema", test_missing_schema),
    ("unsupported version", test_unsupported_version),
    ("invalid action", test_invalid_action),
    ("missing actor", test_missing_actor),
    ("missing target", test_missing_target),
    ("death event", test_death_event),
]


def main() -> int:
    parser = argparse.ArgumentParser(description="Verify abel_core_cli JSON contract behavior.")
    parser.add_argument("--exe", type=Path, default=DEFAULT_EXE, help="Path to abel_core_cli executable.")
    args = parser.parse_args()

    exe = args.exe
    if not exe.exists():
        print(f"ERROR: CLI executable not found: {exe}", file=sys.stderr)
        print("Build it first: cmake --build build-ninja --target abel_core_cli", file=sys.stderr)
        return 1

    failed = 0
    for name, test_func in TESTS:
        try:
            test_func(exe)
            print(f"PASS {name}")
        except Exception as exc:  # noqa: BLE001 - this script reports assertion context.
            failed += 1
            print(f"FAIL {name}: {exc}")

    if failed:
        print(f"{failed} failed, {len(TESTS) - failed} passed")
        return 1

    print(f"{len(TESTS)} passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
