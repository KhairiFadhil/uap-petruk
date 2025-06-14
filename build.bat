@echo off
REM Build script untuk Stock Forecast di Windows

echo ====================================
echo  Building Stock Forecast
echo ====================================
echo.

REM Cek apakah g++ tersedia
g++ --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: g++ tidak ditemukan!
    echo.
    echo Silakan install MSYS2 development tools dulu:
    echo 1. Jalanin: .\install_msys2.bat
    echo 2. Atau manual install dari https://www.msys2.org/
    echo.
    pause
    exit /b 1
)

echo Compiler ditemukan: 
g++ --version | findstr "gcc"

echo.
echo Membuat direktori...
if not exist obj mkdir obj
if not exist bin mkdir bin
if not exist data mkdir data
if not exist bin\debug mkdir bin\debug
if not exist bin\release mkdir bin\release

echo.
echo ====================================
echo  BUILD 1: DEBUG VERSION
echo ====================================
echo.
echo Compiling resources...
windres src/resource.rc -o obj/resource.o
if %ERRORLEVEL% NEQ 0 (
    echo Warning: Resource compilation failed, continuing without icon
    set RESOURCE_OBJ=
) else (
    echo ✅ Resources compiled successfully
    set RESOURCE_OBJ=obj/resource.o
)

echo.
echo Compiling Stock Forecast (Debug)...

g++ -Wall -Wextra -std=c++17 -g -O0 -DDEBUG ^
    -Isrc/headers -IC:/msys64/mingw64/include/ncursesw ^
    src/main.cpp src/csv_reader.cpp src/ui_manager.cpp src/regression.cpp src/stock_data.cpp src/mergeSort.cpp ^
    %RESOURCE_OBJ% ^
    -o bin/debug/stock_forecast_debug.exe -lncursesw

if %ERRORLEVEL% EQU 0 (
    echo ✅ SUCCESS: Debug build berhasil dikompile!
    echo Executable: bin\debug\stock_forecast_debug.exe
    echo.
) else (
    echo ❌ ERROR: Gagal compile debug build
    echo.
)

echo.
echo ====================================
echo  BUILD 2: RELEASE VERSION  
echo ====================================
echo.
echo Compiling Stock Forecast (Release)...

g++ -Wall -Wextra -std=c++17 -O3 -DNDEBUG -s ^
    -Isrc/headers -IC:/msys64/mingw64/include/ncursesw ^
    src/main.cpp src/csv_reader.cpp src/ui_manager.cpp src/regression.cpp src/stock_data.cpp ^
    %RESOURCE_OBJ% ^
    -o bin/release/stock_forecast.exe -lncursesw

if %ERRORLEVEL% EQU 0 (
    echo ✅ SUCCESS: Release build berhasil dikompile!
    echo Executable: bin\release\stock_forecast.exe
    echo.
) else (
    echo ❌ ERROR: Gagal compile release build
    echo.
)

echo.
echo ====================================
echo  BUILD SUMMARY
echo ====================================
echo.

if exist bin\debug\stock_forecast_debug.exe (
    echo ✅ Debug Build: BERHASIL
    echo    Location: bin\debug\stock_forecast_debug.exe
    echo    Features: Debugging symbols, no optimization, extra debugging info
) else (
    echo ❌ Debug Build: GAGAL
)

if exist bin\release\stock_forecast.exe (
    echo ✅ Release Build: BERHASIL  
    echo    Location: bin\release\stock_forecast.exe
    echo    Features: Optimized, stripped, production ready
) else (
    echo ❌ Release Build: GAGAL
)

echo.
echo Cara menjalankan:
echo   Debug:   bin\debug\stock_forecast_debug.exe data\your_file.csv
echo   Release: bin\release\stock_forecast.exe data\your_file.csv
echo.
echo Kemungkinan masalah jika gagal:
echo - NCurses library belum terinstall: pacman -S mingw-w64-x86_64-ncurses
echo - Missing header files di src/headers/
echo - Syntax error di source code
echo.
pause 