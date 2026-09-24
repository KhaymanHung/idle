@echo off
cd /d "%~dp0"
if not exist "out" mkdir out
dir /s /b src\main\java\com\idle\chat\*.java > sources.txt
javac -d out @sources.txt
del sources.txt
java -cp out com.idle.chat.Server
