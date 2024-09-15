#include "medianfilter.h"

median_filter_t::median_filter_t() {
    for (int i = 0; i < FILTER_SIZE; i++) {
        values[i] = 0;
    }
}

void median_filter_t::add(int v) {
    values[i] = v;
    i += 1;
    if (i >= FILTER_SIZE) i = 0;
}

int median_filter_t::read() { return quick_select_median(); }

void median_filter_t::print() {
    print_array(values, FILTER_SIZE);
}

// from https://github.com/rudrasohan/algorithms/blob/master/quickselect.c
void median_filter_t::swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int median_filter_t::partition(int arr[], int low, int high) {
    int pivot = arr[low];  // pivot
    int i = low, j;        // Index of smaller element

    for (j = low + 1; j <= high; j++) {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot) {
            i++;  // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i], &arr[low]);
    return i;
}

void median_filter_t::quick_select(int arr[], int low, int high, int k) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
        // int i;
        // Separately sort elements before
        // partition and after partition
        if (k <= pi) quick_select(arr, low, pi - 1, k);
        if (k == pi + 1)
            return;
        else
            quick_select(arr, pi + 1, high, k);
    }
}

int median_filter_t::quick_select_median() {
    int l[FILTER_SIZE];
    memcpy(l, values, sizeof(l));
    quick_select(l, 0, FILTER_SIZE - 1, 3);
    return l[FILTER_SIZE / 2];
}