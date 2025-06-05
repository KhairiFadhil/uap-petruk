#include "stock_data.hpp"
#include <algorithm>
#include <numeric>

void StockDataProcessor::preprocessData(std::vector<StockPoint>& data) {
    // Set dayIndex buat setiap data point
    // for (size_t i = 0; i < data.size(); ++i) {
    //     data[i].dayIndex = static_cast<int>(i);
    // }
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