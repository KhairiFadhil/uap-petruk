#include "mergeSort.hpp"

#include <vector>

void mergeSort(std::vector<StockPoint> &data, int leftIndex, int rightIndex) {
    if (leftIndex < rightIndex) {
        int midIndex = leftIndex + (rightIndex - leftIndex) / 2;
        mergeSort(data, leftIndex, midIndex);
        mergeSort(data, midIndex + 1, rightIndex);
        merge(data, leftIndex, midIndex, rightIndex);
    }
}

bool isSmaller(const StockPoint& leftParam, const StockPoint& rightParam) {
    return leftParam.m_close < rightParam.m_close;
}

bool isBigger(const StockPoint& leftParam, const StockPoint& rightParam) {
    return leftParam.m_close > rightParam.m_close;
}

void merge(std::vector<StockPoint> &data, int leftIndex, int midIndex, int rightIndex) {
    std::vector<StockPoint> leftTemp(data.begin() + leftIndex, data.begin() + midIndex + 1);
    std::vector<StockPoint> rightTemp(data.begin() + midIndex + 1, data.begin() + rightIndex + 1);
    int leftIdx = 0, rightIdx = 0, mergeIdx = leftIndex;
    while(leftIdx < leftTemp.size() && rightIdx < rightTemp.size()){
        if(isBigger(leftTemp[leftIdx], rightTemp[rightIdx])){
            data[mergeIdx] = leftTemp[leftIdx];
            leftIdx++;
        }else{
            data[mergeIdx] = rightTemp[rightIdx];
            rightIdx++;
        }
        mergeIdx++;
    }
    while(leftIdx < leftTemp.size()){
        data[mergeIdx] = leftTemp[leftIdx];
        leftIdx++;
        mergeIdx++;
    }
    while(rightIdx < rightTemp.size()){
        data[mergeIdx] = rightTemp[rightIdx];
        rightIdx++;
        mergeIdx++;
    }
}