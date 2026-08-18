#!/usr/bin/env bash
set -euo pipefail

port=${1:?usage: scripts/bench.sh /dev/ttyACM0}
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$root"

uv run pio run --target upload
sleep 1
uvx --from git+https://github.com/jesdev-io/jescorecli.git \
  jescore -p "$port" tinyusb_audio_bench
