#include "array_sort.h"
#include <stdlib.h>
#include <stdHeader.h>

int is_sorted(const int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    for (int i = 1; i < size; i++) {
        if (array[i] < array[i - 1]) {
            return 0; // Not sorted
        }
    }
    return 1; // Sorted
}

void bogo_sort(int *array) {
    while (!is_sorted(array)) {
        // Shuffle the array randomly
        int size = sizeof(array) / sizeof(array[0]);
        for (int i = 0; i < size; i++) {
            int j = rand() % size;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

void bubble_sort(int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Swap
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

void stalin_sort(int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    int *sorted_array = (int *)malloc(size * sizeof(int));
    int index = 0;
    sorted_array[index++] = array[0];
    for (int i = 1; i < size; i++) {
        if (array[i] >= sorted_array[index - 1]) {
            sorted_array[index++] = array[i];
        }
    }
    // Copy sorted array back to original array
    for (int i = 0; i < index; i++) {
        array[i] = sorted_array[i];
    }
    free(sorted_array);
}

void insertion_sort(int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    for (int i = 1; i < size; i++) {
        int key = array[i];
        int j = i - 1;
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void selection_sort(int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    for (int i = 0; i < size - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[min_index]) {
                min_index = j;
            }
        }
        // Swap
        int temp = array[i];
        array[i] = array[min_index];
        array[min_index] = temp;
    }
}

void merge_sort(int *array) {
    // Implementation of merge sort
}

void quick_sort(int *array) {
    // Implementation of quick sort
}

void heap_sort(int *array) {
    // Implementation of heap sort
}

void cocktail_sort(int *array) {
    // Implementation of cocktail sort
}

void gnome_sort(int *array) {
    // Implementation of gnome sort
}

void comb_sort(int *array) {
    // Implementation of comb sort
}

void pigeonhole_sort(int *array) {
    // Implementation of pigeonhole sort
}

void cycle_sort(int *array) {
    // Implementation of cycle sort
}

void tim_sort(int *array) {
    // Implementation of tim sort
}

void radix_sort(int *array) {
    // Implementation of radix sort
}

void bucket_sort(int *array) {
    // Implementation of bucket sort
}

void shell_sort(int *array) {
    // Implementation of shell sort
}

void counting_sort(int *array) {
    // Implementation of counting sort
}

void flash_sort(int *array) {
    // Implementation of flash sort
}

void strand_sort(int *array) {
    // Implementation of strand sort
}

void bitonic_sort(int *array) {
    // Implementation of bitonic sort
}

void pancake_sort(int *array) {
    // Implementation of pancake sort
}

void sleep_sort(int *array) {
    // Implementation of sleep sort
}

void tree_sort(int *array) {
    // Implementation of tree
}

void communist_sort(int *array) {
    // Implementation of communist sort
}