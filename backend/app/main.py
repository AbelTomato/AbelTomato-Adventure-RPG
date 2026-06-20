from fastapi import FastAPI

from app.api.game import router as game_router
from app.api.health import router as health_router


app = FastAPI(
    title="Abel Tomato Soul Backend",
    description="FastAPI mock backend for the Abel Tomato Soul JSON contract.",
    version="0.1.0",
)

app.include_router(health_router, prefix="/api")
app.include_router(game_router, prefix="/api")