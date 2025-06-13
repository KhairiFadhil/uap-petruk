#include "stock_data.hpp"
#include <algorithm>
#include <numeric>
#include <cmath>

void StockDataProcessor::preprocessData(std::vector<StockPoint>& data) {
  for (size_t i = 0; i < data.size(); i++){
    data[i].dayIndex = static_cast<int>(i);
  }
}


void StockDataProcessor::normalizeData(std::vector<StockPoint>& data) {
    
}

bool StockDataProcessor::validateData(const std::vector<StockPoint>& data) {
    return !data.empty();
}

StockDataProcessor::DataSummary StockDataProcessor::getSummary(const std::vector<StockPoint>& data) {
    DataSummary summary;
    
    if (data.empty()) {
        return summary;
    }

    double sumClose = 0.0;
    long sumVolume = 0;

    summary.minPrice = data[0].close;
    summary.maxPrice = data[0].close;
    summary.minVolume = data[0].volume;
    summary.maxVolume = data[0].volume;

    for (const auto& point : data){
      sumClose += point.close;
      sumVolume += point.volume;

      if (point.close < summary.minPrice) summary.minPrice = point.close;
      if (point.close > summary.maxPrice) summary.maxPrice = point.close;

      if (point.volume < summary.minVolume) summary.minVolume = point.volume;
      if (point.volume > summary.maxVolume) summary.maxVolume = point.volume;

    }

  summary.avgPrice = sumClose / data.size();
  summary.avgVolume = static_cast<double>(sumVolume) / data.size();
  summary.totalDays = static_cast<int>(data.size());    
    return summary;
} 
