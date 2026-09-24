#!/bin/bash
set -e

cd "$(dirname "$0")"
mkdir -p out
find src/main/java/com/idle/chat -name "*.java" > sources.txt
javac -d out @sources.txt
rm -f sources.txt
nohup java -cp out com.idle.chat.Server > server.log 2>&1 &
echo "Server started in background."
echo "Log file: $(pwd)/server.log"
