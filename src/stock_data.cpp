#include "stock_data.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

void StockDataProcessor::preprocessData(std::vector<StockPoint>& data) {
    // Konversi tanggal ke nilai numerk (hari ke-n)
  for (size_t i = 0; i < data.size(); i++){
    data[i].dayIndex = static_cast<int>(i);
  }
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

    double sumClose = 0.0;
    long sumVolume = 0;

    summary.minClose = data[0].close;
    summary.maxClose = data[0].close;
    summary.minVolume = data[0].volume;
    summary.maxVolume = data[0].volume;

    for (const auto& point : data){
      sumClose += point.close;
      sumVolume += point.volume;

      if (point.close < summary.minClose) summary.minClose = point.close;
      if (point.close > summary.maxClose) summary.maxClose = point.close;

      if (point.volume < summary.minVolume) summary.minVolume = point.volume;
      if (point.volume > summary.maxVolume) summary.maxVolume = point.volume;

    }

  summary.avgClose = sumClose / data.size();
  summary.avgVolume = static_cast<double>(sumVolume) / data.size();
  summary.totalDays = data.size();
    // Hitung min, max, rata-rata harga
      
    // Hitung min, max, rata-rata volume
    // Set total hari
    
    return summary;
} 
