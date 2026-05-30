#!/bin/bash
# nova_wslpath.sh
# Usage: nova_wslpath.sh <path-to-tmpfile>
# Reads a Windows path from file, strips CRLF, outputs WSL path.
if [ -z "$1" ] || [ ! -f "$1" ]; then
    echo "nova_wslpath: ERROR: file not provided or not found: $1" >&2
    exit 1
fi
WIN_PATH=$(cat "$1" | tr -d '\r\n')
wslpath -u "$WIN_PATH"
