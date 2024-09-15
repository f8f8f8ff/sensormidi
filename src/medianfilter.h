#ifndef _MEDIAN_FILTER_H_
#define _MEDIAN_FILTER_H_

#include <Arduino.h>

#include "util.h"

#define FILTER_SIZE 5
class median_filter_t {
    uint8_t i = 0;
    int values[FILTER_SIZE];
    void swap(int* a, int* b);
    int partition(int arr[], int low, int high);
    void quick_select(int arr[], int low, int high, int k);
    int quick_select_median();

   public:
    void add(int v);
    int read();
    void print();
    median_filter_t();
};

#endif