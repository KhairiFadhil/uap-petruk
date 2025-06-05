#include "csv_reader.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

bool CSVReader::loadData(const std::string& filename, std::vector<StockPoint>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Gagal buka file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        StockPoint point = parseLine(line);
        data.push_back(point);
    }
    return true;
}

// parsing data masuk ke variable class
StockPoint CSVReader::parseLine(const std::string& line) {
    StockPoint point;
    std::stringstream ss(line);
    std::string token;
    
    try {
        if (line.empty() || line.find_first_not_of(",") == std::string::npos) {
            return point;
        }
        if (std::getline(ss, token, ',')) {
            point.date = trim(token);
        }
        if (std::getline(ss, token, ',')) {
            point.open = parseDouble(trim(token));
        }
        if (std::getline(ss, token, ',')) {
            point.high = parseDouble(trim(token));
        }        
        if (std::getline(ss, token, ',')) {
            point.low = parseDouble(trim(token));
        }        
        if (std::getline(ss, token, ',')) {
            point.close = parseDouble(trim(token));
        }
        if (std::getline(ss, token, ',')) {
            point.volume = parseLong(trim(token));
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    return point;
}

//ngilangin whitespace

std::string CSVReader::trim(const std::string& str) {
    if (str.empty()) return str;
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}


std::vector<std::string> CSVReader::split(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    
    
    return tokens;
}

double CSVReader::parseDouble(const std::string& str) {
    if (str.empty() || str == "null" || str == "N/A") {
        return 0.0;
    }
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        return 0.0;
    }
}
long CSVReader::parseLong(const std::string& str) {
    if (str.empty() || str == "null" || str == "N/A") {
        return 0;
    }
    try {
        return std::stol(str);
    } catch (const std::exception&) {
        return 0;
    }
}

//Cek Format File
bool CSVReader::validateFile(const std::string& filename, std::string& errorMessage) {
    if (filename.length() < 4) {
        errorMessage = "Nama file terlalu pendek";
        return false;
    }
    
    std::string extension = filename.substr(filename.length() - 4);
    if (extension != ".csv") {
        errorMessage = "File harus berekstensi .csv (ditemukan: " + extension + ")";
        return false;
    }


    std::ifstream file(filename);
    if (!file.is_open()) {
        errorMessage = "File tidak dapat dibuka: " + filename;
        return false;
    }

    std::string firstLine;
    if (!std::getline(file, firstLine) || firstLine.empty()) {
        errorMessage = "File kosong atau tidak dapat dibaca";
        file.close();
        return false;
    }


    if (firstLine.find(',') == std::string::npos) {
        errorMessage = "File bukan format CSV (tidak ada koma pemisah)";
        file.close();
        return false;
    }
    
    file.close();
    errorMessage = "";
    return true;
}

// Hitung berapa baris yang ada di file
int CSVReader::getLineCount(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        count++;
    }
    return count;
}

// buat liat preview beberapa baris pertama (nnti aj)
std::vector<std::string> CSVReader::previewFile(const std::string& filename, int lines) {
    std::vector<std::string> preview;
    
    
    return preview;
} 