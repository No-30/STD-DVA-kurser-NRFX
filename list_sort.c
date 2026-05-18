#include <list.h>
#include <list_sort.h>
#include <stdHeader.h>

int is_sorted_list(const Node *list)
{
    if (list == NULL || list->next == NULL)
        return 1; // En tom lista eller en lista med en nod är sorterad

    if (list->data > list->next->data)
        return 0; // Om nuvarande nod är större än nästa, är listan inte sorterad

    return is_sorted_list(list->next); // Kontrollera resten av listan
}

/*void bogo_sort(Node *list)
{
    while (!is_sorted_list(list))
    {
        // Shuffle the list
        //use rand();
    }
}*/

void bubble_sort_list(Node *list)
{
    if (list == NULL || list->next == NULL)
    {
        return;
    }

    if(is_sorted_list(list))
    {
        return;
    }

    Node *current = list;
    print_list(current);
    while (current->next != NULL)
    {
        if (current->data > current->next->data)
        {
            switchNode(&current); // Byt plats på current och current->next
            // Efter att ha bytt plats, måste vi gå tillbaka en nod för att kontrollera ordningen
            if (current->previous != NULL)
            {
                current = current->previous;
            }
        }
        else
        {
            current = current->next; // Gå vidare till nästa nod
        }
    }
}

/*void stalin_sort(Node *list)
{
    // Implement stalin sort for linked list
}

void insertion_sort(Node *list)
{
    // Implement insertion sort for linked list
}

void selection_sort(Node *list)
{
    // Implement selection sort for linked list
}

void merge_sort(Node **list)
{
    // Implement merge sort for linked list
}

void quick_sort(Node **list)
{
    // Implement quick sort for linked list
}

void heap_sort(Node **list)
{
    // Implement heap sort for linked list
}

void cocktail_sort(Node *list)
{
    // Implement cocktail sort for linked list
}

void gnome_sort(Node *list)
{
    // Implement gnome sort for linked list
}

void comb_sort(Node *list)
{
    // Implement comb sort for linked list
}

void pigeonhole_sort(Node *list)
{
    // Implement pigeonhole sort for linked list
}

void cycle_sort(Node *list)
{
    // Implement cycle sort for linked list
}

void tim_sort(Node *list)
{
    // Implement tim sort for linked list
}

void radix_sort(Node *list)
{
    // Implement radix sort for linked list
}

void bucket_sort(Node *list)
{
    // Implement bucket sort for linked list
}

void shell_sort(Node *list)
{
    // Implement shell sort for linked list
}

void counting_sort(Node *list)
{
    // Implement counting sort for linked list
}

void flash_sort(Node *list)
{
    // Implement flash sort for linked list
}

void strand_sort(Node *list)
{
    // Implement strand sort for linked list
}

void bitonic_sort(Node *list)
{
    // Implement bitonic sort for linked list
}

void pancake_sort(Node *list)
{
    // Implement pancake sort for linked list
}

void sleep_sort(Node *list)
{
    // Implement sleep sort for linked list
}

void tree_sort(Node *list)
{
    // Implement tree sort for linked list
}

void communist_sort(Node *list)
{
    // Implement communist sort for linked list
}*/