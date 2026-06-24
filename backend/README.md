# Backend

FastAPI backend for the Abel Tomato Soul JSON Contract. It can run against either mock responses or the C++ Core CLI.

## Setup

```bash
cd backend
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements.txt
```

## Run

Mock mode is the default:

```bash
uvicorn app.main:app --reload
```

Real C++ Core mode uses `USE_MOCK_CORE=false` and calls `build-ninja/abel_core_cli.exe` through `backend/app/services/cpp_core_client.py`:

```powershell
$env:USE_MOCK_CORE = "false"
uvicorn app.main:app --reload
```

Switch back to mock mode:

```powershell
$env:USE_MOCK_CORE = "true"
uvicorn app.main:app --reload
```

## Test

```bash
python -m pytest
```

Focused game action tests:

```bash
backend/.venv/Scripts/python.exe -m pytest backend/tests/test_game_action.py -v
```

## Endpoints

```txt
GET  /api/health
POST /api/game/action
GET  /api/debug/example-action
```

## Phase 6 Checkpoints

When entering end-to-end integration, verify:

```txt
1. USE_MOCK_CORE=false.
2. /api/game/action returns ok=true.
3. request_id is returned unchanged.
4. state.entities contains the updated target HP.
5. events.seq is continuous.
6. React Debug Console and Godot use the same endpoint without request changes.
```