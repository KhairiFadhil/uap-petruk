#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>
#include <stock_data.hpp>

void mergeSort(std::vector<StockPoint> &data, int left, int right);
void merge(std::vector<StockPoint> &data, int left, int mid, int right);
bool isBigger(const StockPoint& left, const StockPoint& right);

#endif