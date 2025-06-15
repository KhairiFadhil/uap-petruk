#include <iostream>
#include <string>
#include "headers/stock_data.hpp"
#include "headers/csv_reader.hpp"
#include "headers/regression.hpp"
#include "headers/ui_manager.hpp"

int main(int argc, char* argv[]) {
    UIManager ui;
    std::string csvFile;
    //bin/stock_forecast.exe data/{file_csv} gini wak runnya di cmd
    try {
        //Mana main menunya bjir
        ui.init();
        int choice = ui.showMainMenu();
        if (choice == '1') {
            csvFile = ui.selectFile();
            if (csvFile.empty()) {
                ui.cleanup();
                return 0;
            }
        } else if (choice == '2') {
            ui.cleanup();
            return 0;
        }
        
        CSVReader reader;
        std::string errorMessage;
        if (!reader.validateFile(csvFile, errorMessage)) {
            ui.showError("Format file tidak valid: " + errorMessage);
            // printw("\nTekan sembarang tombol untuk keluar...");
            // refresh();
            // getch();
            ui.cleanup();
            return 1;
        }
         
        // if (argc != 2) {
        //     ui.showError("Cara pakai: " + std::string(argv[0]) + " <file_csv>");
        //     printw("\nTekan sembarang tombol untuk keluar...");
        //     refresh();
        //     getch();
        //     ui.cleanup();
        //     return 1;
        // }
        
        // std::string csvFile = argv[1];
        // // Load abis kelar validasi
        std::vector<StockPoint> stockData;
        // if (!reader.loadData(csvFile, stockData)) {
        //     ui.renderTopHeader();
        //     ui.showError("Gagal buka file CSV: " + csvFile);
        //     printw("\nTekan sembarang tombol untuk keluar...");
        //     refresh();
        //     getch();
        //     ui.cleanup();
        //     return 1;
        // }
        // ui.renderTopHeader();
        // printw("Data berhasil dimuat! Total records: %zu\n", stockData.size());
        // printw("\nData Stock Points:\n");
        // printw("==================\n");
        // for (const auto& point : stockData) {
        //     printw("Date: %s | Open: %.2f | High: %.2f | Low: %.2f | Close: %.2f | Volume: %ld\n", 
        //            point.date.c_str(), point.open, point.high, point.low, point.close, point.volume);
        // }
        // refresh();
        // printw("\n\nTekan sembarang tombol untuk keluar...");
        // refresh();
        // getch();
         if (!reader.loadData(csvFile, stockData)) {
            ui.showError("Gagal buka file CSV: " + csvFile);
            ui.cleanup();
            return 1;
        }

        if (stockData.empty()) {
            ui.showError("File CSV tidak terisi data yang valid.");
            ui.cleanup();
            return 1;
        }
        
        // Langkah 3: Olah data (ubah tanggal jadi angka, normalisasi) (gausah)
        StockDataProcessor processor;
        processor.preprocessData(stockData);
        LinearRegression regression;
        regression.trainModel(stockData);
        std::vector<double> predictions = regression.generatePredictions(stockData.size() + 10);
        ui.displayDataTable(stockData);

        bool running = true;
        while (running) {
            ui.renderAll(stockData, regression, predictions);
            int input = ui.waitForInput();
            if (input == 'q' || input == 'Q' || input == 27) { 
                running = false;
            }
        }
        ui.cleanup();
    } catch (const std::exception& e) {
        ui.showError("Ada error nih: " + std::string(e.what()));
        printw("\nTekan sembarang tombol untuk keluar...");
        refresh();
        getch();
        ui.cleanup();
        return 1;
    }
    
    return 0;
} 
