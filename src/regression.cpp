#include "regression.hpp"
#include <cmath>
#include <sstream>
#include <numeric>
#include <iomanip>

bool LinearRegression::trainModel(const std::vector<StockPoint>& data) {
    if (data.empty()) {
        return false;
    }
    
    for(size_t i = 0; i < data.size(); ++i){
        xValues.push_back(static_cast<double>(i));
        yValues.push_back(data[i].close);
    }

    calculateCoefficients();
    calculateRSquared();
    generateEquation();
    
    isTrained = true;
    return true;
}

double LinearRegression::calculateMean(const std::vector<double>& values) {
    if(values.empty()) return 0.0;
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}

void LinearRegression::calculateCoefficients() {
    double xMean = calculateMean(xValues);
    double yMean = calculateMean(yValues);

    double numerator = 0.0;
    double denominator = 0.0;

    for(size_t i = 0; i < xValues.size(); ++i){
        numerator += (xValues[i] - xMean) * (yValues[i] - yMean);
        denominator += (xValues[i] - xMean) * (xValues[i] - xMean);
    }

    model.slope = numerator / denominator;
    model.intercept = yMean - model.slope * xMean;
}

void LinearRegression::calculateRSquared() {
    double ssTotal = 0.0;
    double ssRes = 0.0;
    double yMean = calculateMean(yValues);

    for(size_t i = 0; i < yValues.size(); ++i){
        ssTotal += (yValues[i] - yMean) * (yValues[i] - yMean);
        double predicted = model.slope * xValues[i] + model.intercept;
        ssRes += (yValues[i] - predicted) * (yValues[i] - predicted);
    }

    model.rSquared = 1 - (ssRes / ssTotal);
}

void LinearRegression::generateEquation() {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "y = " << model.slope << "x + " << model.intercept;
    model.equation = ss.str();
}

std::string LinearRegression::getEquation() const {
    return model.equation;
}

double LinearRegression::getRSquared() const {
    return model.rSquared;
}

double LinearRegression::predict(int dayIndex) const {
    if (!isTrained) {
        return 0.0;
    }
    
    return model.slope * dayIndex + model.intercept;
}

std::vector<double> LinearRegression::generatePredictions(int totalDays) {
    std::vector<double> predictions;
    
    for(int i = 0; i < totalDays; ++i){
        predictions.push_back(predict(i));
    }
    
    return predictions;
}

std::string LinearRegression::getTrend() const {
    if (!isTrained) {
        return "Unknown";
    }
    
   if(model.slope > 0.1){
        return "Naik";
    }else if(model.slope < -0.1){
        return "Turun";
    }else{
        return "Datar";
    }
}


double LinearRegression::getSlope() const {
  return model.slope;
}

double LinearRegression::getIntercept() const {
  return model.intercept;
}


bool LinearRegression::isModelValid() const {
    return isTrained && model.rSquared >= 0 && model.rSquared <= 1;
}

void LinearRegression::clear() {
    xValues.clear();
    yValues.clear();
    model = RegressionModel();
    isTrained = false;
} 
