#!/bin/bash
set -e

PORT=9000
PIDS=""

if command -v lsof >/dev/null 2>&1; then
    PIDS=$(lsof -ti tcp:"$PORT" 2>/dev/null || true)
fi

if [ -z "$PIDS" ] && command -v ss >/dev/null 2>&1; then
    PIDS=$(ss -ltnp "sport = :$PORT" 2>/dev/null | awk 'NR>1 {print $NF}' | sed -n 's/.*pid=\([0-9][0-9]*\).*/\1/p' | sort -u || true)
fi

if [ -z "$PIDS" ] && command -v netstat >/dev/null 2>&1; then
    PIDS=$(netstat -ltnp 2>/dev/null | awk -v port=":$PORT" '$4 ~ port {print $7}' | sed -n 's#.*:\([0-9][0-9]*\)$#\1#p' | sort -u || true)
fi

if [ -z "$PIDS" ]; then
    echo "No server is listening on port $PORT."
    exit 0
fi

for PID in $PIDS; do
    if kill -0 "$PID" 2>/dev/null; then
        echo "Stopping Java server with PID $PID ..."
        kill -9 "$PID" 2>/dev/null || true
    fi
done

echo "Server stopped."
