#include <iostream>
#include <vector>
#include <mergeSort.hpp>

void mergeSort(std::vector<StockPoint> &data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

//ni modifikasi aja mergesortnya jangan tergantung ama close ajha bisa diubah ke open, high, low, volume (dibuat aja jadi class jangan fungsi)
bool isSmaller(const StockPoint& left, const StockPoint& right) {
    return left.close < right.close;
}

bool isBigger(const StockPoint& left, const StockPoint& right) {
    return left.close > right.close;
}

void merge(std::vector<StockPoint> &data, int left, int mid, int right) {
    std::vector<StockPoint> temp(data.begin() + left, data.begin() + mid + 1);
    std::vector<StockPoint> temp2(data.begin() + mid + 1, data.begin() + right + 1);
    int i = 0, j = 0, k = left;
    while(i < temp.size() && j < temp2.size()){
        if(isBigger(temp[i], temp2[j])){
            data[k] = temp[i];
            i++;
        }else{
            data[k] = temp2[j];
            j++;
        }
        k++;
    }
    while(i < temp.size()){
        data[k] = temp[i];
        i++;
        k++;
    }
    while(j < temp2.size()){
        data[k] = temp2[j];
        j++;
        k++;
    }
}