#!/bin/bash
# wsl_translate.sh
# Reads a raw Windows path written to a temp file by run.bat,
# strips Windows CRLF, and outputs the WSL/Linux equivalent path.
TMPFILE="$1"
if [ -z "$TMPFILE" ] || [ ! -f "$TMPFILE" ]; then
    echo "[wsl_translate] ERROR: temp file not found: $TMPFILE" >&2
    exit 1
fi
WIN_PATH=$(cat "$TMPFILE" | tr -d '\r\n')
wslpath -u "$WIN_PATH"
