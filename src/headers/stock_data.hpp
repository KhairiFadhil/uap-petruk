#ifndef STOCK_DATA_HPP
#define STOCK_DATA_HPP

#include <string>
#include <vector>

struct StockPoint {
    std::string date;    
    double open;         
    double high;          
    double low;           
    double close;        
    long volume;         
    
    StockPoint() : open(0), high(0), low(0), close(0), volume(0) {}
    
    StockPoint(const std::string& d, double o, double h, double l, double c, long v) 
        : date(d), open(o), high(h), low(l), close(c), volume(v) {}
};

class StockDataProcessor {
public:
    void preprocessData(std::vector<StockPoint>& data);
    void normalizeData(std::vector<StockPoint>& data);
    bool validateData(const std::vector<StockPoint>& data);
    struct DataSummary {
        double minPrice, maxPrice, avgPrice;
        long minVolume, maxVolume, avgVolume;
        int totalDays;
    };
    DataSummary getSummary(const std::vector<StockPoint>& data);
};

#endif 