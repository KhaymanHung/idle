@echo off
setlocal enabledelayedexpansion

for /f "tokens=5" %%p in ('netstat -ano ^| findstr :9000 2^>nul') do (
    set PID=%%p
    echo Stopping Java server with PID !PID! ...
    taskkill /PID !PID! /F >nul 2>&1
    if not errorlevel 1 (
        echo Server stopped.
    ) else (
        echo Failed to stop process !PID!.
    )
    exit /b 0
)

echo No server is listening on port 9000.
