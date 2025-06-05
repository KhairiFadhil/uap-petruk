#include "regression.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

bool LinearRegression::trainModel(const std::vector<StockPoint>& data) {
    if (data.empty()) {
        return false;
    }
    
    // Extract x sama y values
    // xValues = index hari, yValues = harga penutupan
    
    // Hitung koefisien
    
    // Hitung R-squared
    
    // Bikin string persamaan
    
    isTrained = true;
    return true;
}

double LinearRegression::calculateMean(const std::vector<double>& values) {
    return 0.0;
}

void LinearRegression::calculateCoefficients() {
    // Implement metode least squares
    // slope = Σ((x - x̄)(y - ȳ)) / Σ((x - x̄)²)
    // intercept = ȳ - slope * x̄
}

void LinearRegression::calculateRSquared() {
    // Hitung koefisien determinasi
    // R² = 1 - (SS_res / SS_tot)
}

void LinearRegression::generateEquation() {
    // Format persamaan jadi string: "y = mx + b"
}

double LinearRegression::predict(int dayIndex) {
    if (!isTrained) {
        return 0.0;
    }
    
    // Return slope * dayIndex + intercept
    return 0.0;
}

std::vector<double> LinearRegression::generatePredictions(int totalDays) {
    std::vector<double> predictions;
    
    // Bikin prediksi buat tiap hari
    // for (int i = 0; i < totalDays; ++i) {
    //     predictions.push_back(predict(i));
    // }
    
    return predictions;
}

std::string LinearRegression::getTrend() const {
    if (!isTrained) {
        return "Unknown";
    }
    
    // Analisis slope buat tentuin trend
    // if (model.slope > 0.1) return "Naik";
    // else if (model.slope < -0.1) return "Turun";
    // else return "Datar";
    
    return "Unknown";
}

bool LinearRegression::isModelValid() const {
    // Cek R-squared masuk akal, ukuran data cukup, dll
    return isTrained;
}

void LinearRegression::clear() {
    xValues.clear();
    yValues.clear();
    model = RegressionModel();
    isTrained = false;
} 