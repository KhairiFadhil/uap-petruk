#include "stock_data.hpp"
#include <algorithm>
#include <numeric>

void StockDataProcessor::preprocessData(std::vector<StockPoint>& data) {
  
}


void StockDataProcessor::normalizeData(std::vector<StockPoint>& data) {
    // Implement normalisasi kalo butuh
}

bool StockDataProcessor::validateData(const std::vector<StockPoint>& data) {
    // Cek data kosong atau invalid
    return !data.empty();
}

StockDataProcessor::DataSummary StockDataProcessor::getSummary(const std::vector<StockPoint>& data) {
    DataSummary summary;
    
    if (data.empty()) {
        return summary;
    }
    
    // Hitung min, max, rata-rata harga
    // Hitung min, max, rata-rata volume
    // Set total hari
    
    return summary;
} 