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
    
    double calculateMean(const std::vector<double>& values);
    
    void calculateCoefficients();
    void calculateRSquared();
    
    void generateEquation();
    
public:
    LinearRegression() : isTrained(false) {}
    double getSlope() const;
    double getIntercept() const;
    std::string getEquation() const;
    double getRSquared() const;
    bool trainModel(const std::vector<StockPoint>& data);
    const RegressionModel& getModel() const { return model; }
    double predict(int dayIndex) const;
    std::vector<double> generatePredictions(int totalDays);
    std::string getTrend() const;
    bool isModelValid() const;
    void clear();
};

#endif