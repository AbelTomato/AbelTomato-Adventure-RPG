from fastapi import FastAPI

from app.api.game import router as game_router
from app.api.health import router as health_router
from app.api.config import router as config_router
from app.api.saves import router as saves_router


app = FastAPI(
    title="Abel Tomato Soul Backend",
    description="FastAPI mock backend for the Abel Tomato Soul JSON contract.",
    version="0.1.0",
)

app.include_router(health_router, prefix="/api")
app.include_router(game_router, prefix="/api")
app.include_router(config_router, prefix="/api")
app.include_router(saves_router, prefix="/api")