@echo off
chcp 65001 >nul
cd /d %~dp0
if exist build\lesson01.exe (
    build\lesson01.exe
) else if exist lesson01.exe (
    lesson01.exe
) else (
    echo 请先编译程序
    pause
)

