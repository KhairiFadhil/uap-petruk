#include "regression.hpp"

#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>

bool LinearRegression::trainModel(const std::vector<StockPoint>& data) {
    if (data.empty()) {
        return false;
    }
    
    for(size_t i = 0; i < data.size(); ++i){
        m_xValues.push_back(static_cast<double>(i));
        m_yValues.push_back(data[i].m_close);
    }

    calculateCoefficients();
    calculateRSquared();
    generateEquation();
    
    m_isTrained = true;
    return true;
}

double LinearRegression::calculateMean(const std::vector<double>& values) {
    if(values.empty()) return 0.0;
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}

void LinearRegression::calculateCoefficients() {
    double xMean = calculateMean(m_xValues);
    double yMean = calculateMean(m_yValues);

    double numerator = 0.0;
    double denominator = 0.0;

    for(size_t i = 0; i < m_xValues.size(); ++i){
        numerator += (m_xValues[i] - xMean) * (m_yValues[i] - yMean);
        denominator += (m_xValues[i] - xMean) * (m_xValues[i] - xMean);
    }

    m_model.m_slope = numerator / denominator;
    m_model.m_intercept = yMean - m_model.m_slope * xMean;
}

void LinearRegression::calculateRSquared() {
    double ssTotal = 0.0;
    double ssRes = 0.0;
    double yMean = calculateMean(m_yValues);

    for(size_t i = 0; i < m_yValues.size(); ++i){
        ssTotal += (m_yValues[i] - yMean) * (m_yValues[i] - yMean);
        double predicted = m_model.m_slope * m_xValues[i] + m_model.m_intercept;
        ssRes += (m_yValues[i] - predicted) * (m_yValues[i] - predicted);
    }

    m_model.m_rSquared = 1 - (ssRes / ssTotal);
}

void LinearRegression::generateEquation() {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "y = " << m_model.m_slope << "x + " << m_model.m_intercept;
    m_model.m_equation = ss.str();
}

std::string LinearRegression::getEquation() const {
    return m_model.m_equation;
}

double LinearRegression::getRSquared() const {
    return m_model.m_rSquared;
}

double LinearRegression::predict(int dayIndex) const {
    if (!m_isTrained) {
        return 0.0;
    }
    
    return m_model.m_slope * dayIndex + m_model.m_intercept;
}

std::vector<double> LinearRegression::generatePredictions(int totalDays) {
    std::vector<double> predictions;
    
    for(int i = 0; i < totalDays; ++i){
        predictions.push_back(predict(i));
    }
    
    return predictions;
}

std::string LinearRegression::getTrend() const {
    if (!m_isTrained) {
        return "Unknown";
    }
    
   if(m_model.m_slope > 0.1){
        return "Naik";
    }else if(m_model.m_slope < -0.1){
        return "Turun";
    }else{
        return "Datar";
    }
}


double LinearRegression::getSlope() const {
  return m_model.m_slope;
}

double LinearRegression::getIntercept() const {
  return m_model.m_intercept;
}


bool LinearRegression::isModelValid() const {
    return m_isTrained && m_model.m_rSquared >= 0 && m_model.m_rSquared <= 1;
}

void LinearRegression::clear() {
    m_xValues.clear();
    m_yValues.clear();
    m_model = RegressionModel();
    m_isTrained = false;
}
