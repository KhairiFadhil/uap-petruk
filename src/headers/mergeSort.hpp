#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>
#include <stock_data.hpp>

void mergeSort(std::vector<StockPoint> &data, int leftIndex, int rightIndex);
void merge(std::vector<StockPoint> &data, int leftIndex, int midIndex, int rightIndex);
bool isBigger(const StockPoint& leftParam, const StockPoint& rightParam);

#endif