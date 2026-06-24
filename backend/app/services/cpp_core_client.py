import subprocess
import json
import os


DEFAULT_EXE_PATH = os.path.join(
    os.path.dirname(__file__),
    "..", "..", "..",
    "build-ninja",
    "abel_core_cli.exe"
)


def call_cpp_core(input_json: dict) -> dict:
    json_str = json.dumps(input_json, ensure_ascii=True)
    exe_path = os.environ.get("CPP_CORE_EXE_PATH") or DEFAULT_EXE_PATH
    if not os.path.exists(exe_path):
        raise RuntimeError(f"C++ Core executable not found: {exe_path}")

    cmd = [exe_path]

    result = subprocess.run(
        cmd,
        input=json_str,
        capture_output=True,
        text=True,
        timeout=5,
        encoding="utf-8",
    )

    if result.returncode != 0:
        raise RuntimeError(f"C++ Core failed: {result.stderr}")

    return json.loads(result.stdout)