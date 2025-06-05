#!/bin/bash
# Build script untuk Stock Forecast di Linux/WSL

echo "Building Stock Forecast..."

# Buat direktori kalau belum ada
mkdir -p obj bin data

# Compile test UI
echo "Compiling UI test..."
g++ -Wall -Wextra -std=c++17 -g -Isrc/headers test/test_ui_init.cpp src/ui_manager.cpp -o bin/test_ui -lncurses

if [ $? -eq 0 ]; then
    echo "UI test berhasil dikompile: bin/test_ui"
    echo ""
    echo "Jalanin dengan: ./bin/test_ui"
    
    # Set executable permission
    chmod +x bin/test_ui
else
    echo "Error: Gagal compile UI test"
    echo ""
    echo "Pastikan kamu udah install:"
    echo "- build-essential (gcc/g++)"
    echo "- libncurses5-dev libncursesw5-dev"
    echo ""
    echo "Install dengan:"
    echo "sudo apt update"
    echo "sudo apt install build-essential libncurses5-dev libncursesw5-dev"
fi 