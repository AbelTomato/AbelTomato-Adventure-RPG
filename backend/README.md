# Backend

FastAPI mock backend for the Abel Tomato Soul JSON Contract.

## Setup

```bash
cd backend
python -m venv .venv
.venv\Scripts\activate
pip install -r requirements.txt
```

## Run

```bash
uvicorn app.main:app --reload
```

## Test

```bash
python -m pytest
```

## Endpoints

```txt
GET  /api/health
POST /api/game/action
GET  /api/debug/example-action
```