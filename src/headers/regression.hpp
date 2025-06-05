#ifndef REGRESSION_HPP
#define REGRESSION_HPP

#include <vector>
#include <string>
#include "stock_data.hpp"
struct RegressionModel {
    double slope;        // Kemiringan garis (m)
    double intercept;    // Titik potong Y (b)
    double rSquared;     // R-squared (seberapa bagus modelnya)
    double correlation;  // Korelasi
    std::string equation; // Persamaan: y = mx + b
    
    RegressionModel() : slope(0), intercept(0), rSquared(0), correlation(0) {}
};

class LinearRegression {
private:
    RegressionModel model;
    std::vector<double> xValues;  // Index hari
    std::vector<double> yValues;  // Harga penutupan
    bool isTrained;
    
    // Hitung rata-rata dari vector
    double calculateMean(const std::vector<double>& values);
    
    // Hitung slope sama intercept
    void calculateCoefficients();
    
    // Hitung R-squared (seberapa akurat)
    void calculateRSquared();
    
    // Bikin string persamaan
    void generateEquation();
    
public:
    LinearRegression() : isTrained(false) {}
    
    // Latih model pake data
    bool trainModel(const std::vector<StockPoint>& data);
    
    // Ambil hasil model
    const RegressionModel& getModel() const { return model; }
    
    // Prediksi harga buat hari tertentu
    double predict(int dayIndex);
    
    // Bikin prediksi buat beberapa hari
    std::vector<double> generatePredictions(int totalDays);
    
    // Analisis trend (naik/turun/datar)
    std::string getTrend() const;
    
    // Cek apakah model bagus atau ngga
    bool isModelValid() const;
    
    // Bersihin model
    void clear();
};

#endif // REGRESSION_HPP 