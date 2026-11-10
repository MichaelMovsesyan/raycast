#!/usr/bin/env bash
# Build (if needed) and run the raycast program.
set -euo pipefail

cd "$(dirname "$0")"
make
./raycast "$@"
