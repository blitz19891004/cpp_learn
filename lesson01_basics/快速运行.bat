@echo off
chcp 65001 >nul
echo 正在编译第1课代码...
cd /d %~dp0
if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles" 2>nul || cmake ..
cmake --build .
if exist lesson01.exe (
    echo.
    echo 编译成功！正在运行...
    echo.
    chcp 65001 >nul
    lesson01.exe
) else if exist lesson01 (
    echo.
    echo 编译成功！正在运行...
    echo.
    ./lesson01
) else (
    echo 编译失败，请检查CMake和编译器是否安装
)
pause

