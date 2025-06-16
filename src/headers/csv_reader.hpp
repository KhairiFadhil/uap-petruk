#ifndef CSV_READER_HPP
#define CSV_READER_HPP

#include <string>
#include <vector>
#include <fstream>
#include "stock_data.hpp"

class CSVReader {
private:
    std::string m_filename;
    char m_delimiter;
    StockPoint parseLine(const std::string& line);
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delim);
    double parseDouble(const std::string& str);
    long parseLong(const std::string& str);
    
public:
    CSVReader(char delimParam = ',') : m_delimiter(delimParam) {}
    bool loadData(const std::string& filename, std::vector<StockPoint>& data);
    bool validateFile(const std::string& filename, std::string& errorMessage);
    int getLineCount(const std::string& filename);
    std::vector<std::string> previewFile(const std::string& filename, int lines = 5);
};

#endif