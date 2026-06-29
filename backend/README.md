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

Real C++ Core mode uses `USE_MOCK_CORE=false` and calls the C++ CLI through `backend/app/services/cpp_core_client.py`.

By default, the backend looks for:

```txt
build-ninja/abel_core_cli.exe
```

For Phase 6 integration, prefer setting `CPP_CORE_EXE_PATH` explicitly so the backend calls the CLI rebuilt from the current source tree. For example, when using the MinGW build output:

```powershell
$env:USE_MOCK_CORE = "false"
$env:CPP_CORE_EXE_PATH = "D:\AbelTomato_Files\Developer\Projects\Abel-Tomato-Soul\build-mingw\abel_core_cli.exe"
uvicorn app.main:app --reload
```

Do not treat an old `build-ninja/abel_core_cli.exe` as valid Phase 6 evidence unless it was rebuilt from the current merged source.

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

Focused C++ Core client path tests:

```bash
backend/.venv/Scripts/python.exe -m pytest backend/tests/test_cpp_core_client.py -v
```

## Endpoints

```txt
GET  /api/health
POST /api/game/action
GET  /api/debug/example-action
GET  /api/config/{config_name}
```

Phase 7 planned / in progress:

```txt
GET    /api/saves
POST   /api/saves
GET    /api/saves/{save_id}
PUT    /api/saves/{save_id}
DELETE /api/saves/{save_id}
```

Current known issue: `backend/tests/test_save_service.py` is incomplete and currently blocks full backend test collection. Fix that before adding more Phase 7 behavior.

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