@echo off
cd /d "%~dp0"

if not exist "out" mkdir out

dir /s /b src\main\java\com\idle\chat\*.java > sources.txt
javac -d out @sources.txt
del sources.txt

powershell -NoProfile -ExecutionPolicy Bypass -Command "$p = Start-Process -FilePath 'java.exe' -ArgumentList '-cp','out','com.idle.chat.Server' -WorkingDirectory '%CD%' -PassThru -RedirectStandardOutput 'server.log' -RedirectStandardError 'server.err' -WindowStyle Hidden; Write-Host 'Server started in background. PID:' $p.Id"
