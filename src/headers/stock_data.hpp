#ifndef STOCK_DATA_HPP
#define STOCK_DATA_HPP

#include <string>
#include <vector>

struct StockPoint {
    std::string m_date;    
    double m_open;         
    double m_high;          
    double m_low;           
    double m_close;        
    long m_volume;         
    int m_dayIndex = 0;  

    StockPoint() : m_open(0), m_high(0), m_low(0), m_close(0), m_volume(0) {}
    
    StockPoint(const std::string& dateParam, double openParam, double highParam, double lowParam, double closeParam, long volumeParam) 
        : m_date(dateParam), m_open(openParam), m_high(highParam), m_low(lowParam), m_close(closeParam), m_volume(volumeParam) {}
};

class StockDataProcessor {
public:
    void preprocessData(std::vector<StockPoint>& data);
    void normalizeData(std::vector<StockPoint>& data);
    bool validateData(const std::vector<StockPoint>& data);
    struct DataSummary {
        double m_minPrice, m_maxPrice, m_avgPrice;
        long m_minVolume, m_maxVolume, m_avgVolume;
        int m_totalDays;
    };
    DataSummary getSummary(const std::vector<StockPoint>& data);
};

#endif
