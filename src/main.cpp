#include <iostream>
#include <string>

#include "headers/csv_reader.hpp"
#include "headers/regression.hpp"
#include "headers/stock_data.hpp"
#include "headers/ui_manager.hpp"

int main(int argc, char* argv[]) {
    UIManager ui;
    std::string csvFile;
    
    try {
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
            ui.cleanup();
            return 1;
        }
        
        std::vector<StockPoint> stockData;
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
