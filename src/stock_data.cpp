#include "stock_data.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>

void StockDataProcessor::preprocessData(std::vector<StockPoint>& data) {
  for (size_t i = 0; i < data.size(); i++){
    data[i].m_dayIndex = static_cast<int>(i);
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

    summary.m_minPrice = data[0].m_close;
    summary.m_maxPrice = data[0].m_close;
    summary.m_minVolume = data[0].m_volume;
    summary.m_maxVolume = data[0].m_volume;

    for (const auto& point : data){
      sumClose += point.m_close;
      sumVolume += point.m_volume;

      if (point.m_close < summary.m_minPrice) summary.m_minPrice = point.m_close;
      if (point.m_close > summary.m_maxPrice) summary.m_maxPrice = point.m_close;

      if (point.m_volume < summary.m_minVolume) summary.m_minVolume = point.m_volume;
      if (point.m_volume > summary.m_maxVolume) summary.m_maxVolume = point.m_volume;

    }

  summary.m_avgPrice = sumClose / data.size();
  summary.m_avgVolume = static_cast<double>(sumVolume) / data.size();
  summary.m_totalDays = static_cast<int>(data.size());    
    return summary;
}
