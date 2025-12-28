#!/bin/bash
echo "正在编译第1课代码..."
cd "$(dirname "$0")"
mkdir -p build
cd build
cmake ..
cmake --build .
if [ -f lesson01 ]; then
    echo ""
    echo "编译成功！正在运行..."
    echo ""
    ./lesson01
else
    echo "编译失败，请检查CMake和编译器是否安装"
fi

