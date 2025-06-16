#ifndef REGRESSION_HPP
#define REGRESSION_HPP

#include <vector>
#include <string>
#include "stock_data.hpp"
struct RegressionModel {
    double m_slope;        // Kemiringan garis (m)
    double m_intercept;    // Titik potong Y (b)
    double m_rSquared;     // R-squared (seberapa bagus modelnya)
    double m_correlation;  // Korelasi
    std::string m_equation; // Persamaan: y = mx + b
    
    RegressionModel() : m_slope(0), m_intercept(0), m_rSquared(0), m_correlation(0) {}
};

class LinearRegression {
private:
    RegressionModel m_model;
    std::vector<double> m_xValues;  // Index hari
    std::vector<double> m_yValues;  // Harga penutupan
    bool m_isTrained;
    
    double calculateMean(const std::vector<double>& values);
    
    void calculateCoefficients();
    void calculateRSquared();
    
    void generateEquation();
    
public:
    LinearRegression() : m_isTrained(false) {}
    double getSlope() const;
    double getIntercept() const;
    std::string getEquation() const;
    double getRSquared() const;
    bool trainModel(const std::vector<StockPoint>& data);
    const RegressionModel& getModel() const { return m_model; }
    double predict(int dayIndex) const;
    std::vector<double> generatePredictions(int totalDays);
    std::string getTrend() const;
    bool isModelValid() const;
    void clear();
};

#endif