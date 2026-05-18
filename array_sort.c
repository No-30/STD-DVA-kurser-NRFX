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

static void merge(int *array, int left, int mid, int right) {
    int left_size = mid - left + 1;
    int right_size = right - mid;
    int *left_array = (int *)malloc(left_size * sizeof(int));
    int *right_array = (int *)malloc(right_size * sizeof(int));

    for (int i = 0; i < left_size; i++) {
        left_array[i] = array[left + i];
    }
    for (int j = 0; j < right_size; j++) {
        right_array[j] = array[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;
    while (i < left_size && j < right_size) {
        if (left_array[i] <= right_array[j]) {
            array[k++] = left_array[i++];
        } else {
            array[k++] = right_array[j++];
        }
    }
    while (i < left_size) {
        array[k++] = left_array[i++];
    }
    while (j < right_size) {
        array[k++] = right_array[j++];
    }

    free(left_array);
    free(right_array);
}

static void merge_sort_recursive(int *array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_recursive(array, left, mid);
        merge_sort_recursive(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void merge_sort(int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    if (size <= 1) {
        return;
    }
    merge_sort_recursive(array, 0, size - 1);
}

static int partition(int *array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;
    return i + 1;
}

static void quick_sort_recursive(int *array, int low, int high) {
    if (low < high) {
        int pivot_index = partition(array, low, high);
        quick_sort_recursive(array, low, pivot_index - 1);
        quick_sort_recursive(array, pivot_index + 1, high);
    }
}

void quick_sort(int *array) {
    int size = sizeof(array) / sizeof(array[0]);
    if (size <= 1) {
        return;
    }
    quick_sort_recursive(array, 0, size - 1);
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