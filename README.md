# Stock Forecast - Tool Prediksi Saham CLI

Stock Forecast adalah aplikasi command-line interface (CLI) untuk memprediksi pergerakan harga saham menggunakan metode regresi linear. Aplikasi ini dapat membaca data saham dari file CSV dan memberikan prediksi berdasarkan analisis data historis.

## Fitur

- Membaca data saham dari file CSV
- Analisis data menggunakan regresi linear
- Visualisasi data dan prediksi
- Sorting data menggunakan Merge Sort
- Antarmuka command-line yang mudah digunakan

## Prasyarat

- Windows 10 atau lebih baru
- MSYS2 (untuk kompilasi)
- Compiler C++ (GCC/G++)


## Instalasi

1. Clone repository ini:
```bash
git clone https://github.com/yourusername/stock-forecast.git
cd stock-forecast
```

2. Kompilasi program:
```bash

# Atau menggunakan build script
./build.bat # Windows
```

## Penggunaan
Compile dengan bash script kemudian hasil compile tersebut akan ada di /bin

Format file CSV yang didukung:
- Kolom pertama: Tanggal (format: YYYY-MM-DD)
- Kolom kedua: Harga saham (numerik)

## Struktur Proyek

```
stock-forecast/
├── bin/              # Binary files
├── data/             # Sample data files
├── src/              # Source code
│   ├── headers/      # Header files
│   ├── main.cpp      # Main program
│   ├── csv_reader.cpp # CSV file handling
│   ├── regression.cpp # Linear regression implementation
│   ├── stock_data.cpp # Stock data structures
│   ├── ui_manager.cpp # User interface
│   └── mergeSort.cpp # Sorting algorithm
└── build.bat         # Windows build script

```
