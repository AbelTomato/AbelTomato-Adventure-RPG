import subprocess
import json
import os


EXE_PATH = os.path.join(
    os.path.dirname(__file__),
    "..", "..", "..",
    "build-ninja",
    "abel_core_cli.exe"
)


def call_cpp_core(input_json: dict) -> dict:
    json_str = json.dumps(input_json, ensure_ascii=True)
    cmd = [EXE_PATH]

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