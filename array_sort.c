#include "array_sort.h"
#include <stdlib.h>
#include <stdHeader.h>
#include <stdHeader.h>

// Kontrollerar om arrayen är sorterad i stigande ordning
// Returnerar 1 om sorterad, annars 0
int is_sorted(const int *array, int size) {
    for (int i = 1; i < size; i++) {
        if (array[i] < array[i - 1]) {
            return 0; // Inte sorterad
        }
    }
    return 1; // Sorterad
}

// Utför bogo sort (slumpmässig sortering tills arrayen är sorterad)
// Extremt ineffektiv sorteringsalgoritm, används mest som skämt
void bogo_sort(int *array, int size) {
    while (!is_sorted(array, size)) {
        // Blandar arrayen slumpmässigt
        for (int i = 0; i < size; i++) {
            int j = rand() % size; // Välj slumpmässigt index
            // Byt plats på array[i] och array[j]
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

// Utför bubble sort (bubblar upp största värdet varje varv)
// Jämför intilliggande element och byter plats om de är i fel ordning
void bubble_sort(int *array, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                // Byter plats på elementen
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}

// Utför stalin sort (tar bort element som bryter den stigande ordningen)
// Endast de element som är i ordning får vara kvar
void stalin_sort(int *array, int size) {
    int *sorted_array = (int *)malloc(size * sizeof(int)); // Temporär array
    int index = 0;
    sorted_array[index++] = array[0]; // Första elementet är alltid kvar
    for (int i = 1; i < size; i++) {
        if (array[i] >= sorted_array[index - 1]) {
            sorted_array[index++] = array[i]; // Lägg till om i ordning
        }
    }
    // Kopierar tillbaka de sorterade elementen till original-arrayen
    for (int i = 0; i < index; i++) {
        array[i] = sorted_array[i];
    }
    free(sorted_array);
}

// Utför insertion sort (infogar varje element på rätt plats)
// Flyttar varje element bakåt tills det hamnar rätt
void insertion_sort(int *array, int size) {
    for (int i = 1; i < size; i++) {
        int key = array[i];
        int j = i - 1;
        // Flytta större element ett steg framåt
        while (j >= 0 && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key; // Sätt in elementet på rätt plats
    }
}

// Utför selection sort (hittar minsta elementet och byter plats)
// Går igenom arrayen och letar efter minsta elementet för varje position
void selection_sort(int *array, int size) {
    for (int i = 0; i < size - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < size; j++) {
            if (array[j] < array[min_index]) {
                min_index = j; // Hittat nytt minsta värde
            }
        }
        // Byter plats på element
        int temp = array[i];
        array[i] = array[min_index];
        array[min_index] = temp;
    }
}

// Hjälpfunktion för att slå ihop två sorterade delmängder (merge sort)
// Används av merge sort för att kombinera två sorterade delar till en
static void merge(int *array, int *temp, int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        temp[k++] = (array[i] <= array[j]) ? array[i++] : array[j++];
    }
    while (i <= mid) temp[k++] = array[i++];
    while (j <= right) temp[k++] = array[j++];

    for (int x = left; x <= right; x++) {
        array[x] = temp[x];
    }
}

// Rekursiv funktion för merge sort
// Delar upp arrayen i mindre delar och slår ihop dem sorterat
static void merge_sort_recursive(int *array, int *temp, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    merge_sort_recursive(array, temp, left, mid);
    merge_sort_recursive(array, temp, mid + 1, right);
    merge(array, temp, left, mid, right);
}

// Utför merge sort (delar upp och slår ihop sorterat)
// Startar den rekursiva sorteringen
void merge_sort(int *array, int size) {
    int *temp = malloc(size * sizeof(int));
    merge_sort_recursive(array, temp, 0, size - 1);
    free(temp);
}

// Hjälpfunktion för partitionering (quick sort)
// Delar upp arrayen kring ett pivot-element
static int partition(int *array, int low, int high) {
    int pivot = array[high]; // Välj sista elementet som pivot
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            // Byt plats på array[i] och array[j]
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
    // Sätt pivot på rätt plats
    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;
    return i + 1;
}

// Rekursiv funktion för quick sort
// Sorterar delarna före och efter pivot
static void quick_sort_recursive(int *array, int low, int high) {
    if (low < high) {
        int pivot_index = partition(array, low, high);
        quick_sort_recursive(array, low, pivot_index - 1);
        quick_sort_recursive(array, pivot_index + 1, high);
    }
}

// Utför quick sort (delar upp kring pivot)
// Startar den rekursiva sorteringen
void quick_sort(int *array, int size) {
    if (size <= 1) {
        return;
    }
    quick_sort_recursive(array, 0, size - 1);
}

// Nedan följer tomma funktioner för olika sorteringsalgoritmer
// Dessa är bara mallar och saknar implementation
void heap_sort(int *array, int size) {
    // Implementation av heap sort
}

void cocktail_sort(int *array, int size) {
    // Implementation av cocktail sort
}

void gnome_sort(int *array, int size) {
    // Implementation av gnome sort
}

void comb_sort(int *array, int size) {
    // Implementation av comb sort
}

void pigeonhole_sort(int *array, int size) {
    // Implementation av pigeonhole sort
}

void cycle_sort(int *array, int size) {
    // Implementation av cycle sort
}

void tim_sort(int *array, int size) {
    // Implementation av tim sort
}

void radix_sort(int *array, int size) {
    // Implementation av radix sort
}

void bucket_sort(int *array, int size) {
    // Implementation av bucket sort
}

void shell_sort(int *array, int size) {
    // Implementation av shell sort
}

void counting_sort(int *array, int size) {
    // Implementation av counting sort
}

void flash_sort(int *array, int size) {
    // Implementation av flash sort
}

void strand_sort(int *array, int size) {
    // Implementation av strand sort
}

void bitonic_sort(int *array, int size) {
    // Implementation av bitonic sort
}

void pancake_sort(int *array, int size) {
    // Implementation av pancake sort
}

void sleep_sort(int *array, int size) {
    // Implementation av sleep sort
}

void tree_sort(int *array, int size) {
    // Implementation av tree sort
}

void communist_sort(int *array, int size) {
    // Implementation av communist sort
}