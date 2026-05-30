#!/usr/bin/env bash
set -e

COMMAND=${1:-run}

echo "Building NovaShell (Command: $COMMAND)..."

if [ "$COMMAND" = "clean" ]; then
    make clean
elif [ "$COMMAND" = "debug" ]; then
    make debug
    ./build/novashell
elif [ "$COMMAND" = "sanitize" ]; then
    make sanitize
    ./build/novashell
elif [ "$COMMAND" = "test" ]; then
    make test
else
    make all
    ./build/novashell
fi
