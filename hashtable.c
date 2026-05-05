#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

// Initiera hashtabellen genom att sätta alla pekare till NULL
void initTable(HashTable* ht)
{
    if (ht == NULL)
    {
        return;
    }

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        ht->table[i] = NULL;
    }
}

// Enkel hashfunktion som beräknar en position i arrayen beroende på key
int hash(int key)
{
    int index = key % TABLE_SIZE;
    if (index < 0)
    {
        index += TABLE_SIZE;
    }
    return index;
}


// Insert
void insert(HashTable* ht, int key, const char* value)
{
    if (ht == NULL || value == NULL)
    {
        return;
    }

    int index = hash(key);
    Node* current = ht->table[index];

    while (current != NULL)
    {
        if (current->data.key == key)
        {
            strncpy(current->data.value, value, sizeof(current->data.value) - 1);
            current->data.value[sizeof(current->data.value) - 1] = '\0';
            return;
        }
        current = current->next;
    }

    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return;
    }

    newNode->data.key = key;
    strncpy(newNode->data.value, value, sizeof(newNode->data.value) - 1);
    newNode->data.value[sizeof(newNode->data.value) - 1] = '\0';
    newNode->next = ht->table[index];
    ht->table[index] = newNode;
}

// Get
char* get(HashTable* ht, int key)
{
    if (ht == NULL)
    {
        return NULL;
    }

    int index = hash(key);
    Node* current = ht->table[index];

    while (current != NULL)
    {
        if (current->data.key == key)
        {
            return current->data.value;
        }
        current = current->next;
    }

    return NULL;
}

// Remove
int removeKey(HashTable* ht, int key)
{
    if (ht == NULL)
    {
        return 0;
    }

    int index = hash(key);
    Node* current = ht->table[index];
    Node* previous = NULL;

    while (current != NULL)
    {
        if (current->data.key == key)
        {
            if (previous == NULL)
            {
                ht->table[index] = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            free(current);
            return 1;
        }
        previous = current;
        current = current->next;
    }

    return 0;
}

// Debugfunktion som skriver ut allt som finns i tabellen
void printTable(HashTable* ht)
{
    if (ht == NULL)
    {
        return;
    }

    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        printf("Bucket %d:", i);
        Node* current = ht->table[i];
        if (current == NULL)
        {
            printf(" (empty)\n");
            continue;
        }

        while (current != NULL)
        {
            printf(" [key=%d, value=%s]", current->data.key, current->data.value);
            current = current->next;
        }
        printf("\n");
    }
}