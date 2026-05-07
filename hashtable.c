#include <stdlib.h>
#include <string.h>
#include <hashtable.h>
#include <stdHeader.h>

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
        index += TABLE_SIZE;  // Hantera negativa nycklar
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
    Node_hash* current = ht->table[index];

    // Sök efter befintlig nyckel för att uppdatera värdet
    while (current != NULL)
    {
        if (current->data.key == key)
        {
            // Uppdatera värdet om nyckeln redan finns
            strncpy(current->data.value, value, sizeof(current->data.value) - 1);
            current->data.value[sizeof(current->data.value) - 1] = '\0';
            return;
        }
        current = current->next;
    }

    // Skapa en ny nod om nyckeln inte finns
    Node_hash* newNode = malloc(sizeof(Node_hash));
    if (newNode == NULL)
    {
        return;
    }

    newNode->data.key = key;
    strncpy(newNode->data.value, value, sizeof(newNode->data.value) - 1);
    newNode->data.value[sizeof(newNode->data.value) - 1] = '\0';
    // Sätt prev till NULL och next till nuvarande huvud
    newNode->prev = NULL;
    newNode->next = ht->table[index];
    // Om det finns en befintlig nod, sätt dess prev till newNode
    if (ht->table[index] != NULL)
    {
        ht->table[index]->prev = newNode;
    }
    // Uppdatera tabellen att peka på newNode
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
    Node_hash* current = ht->table[index];

    // Sök genom listan för nyckeln
    while (current != NULL)
    {
        if (current->data.key == key)
        {
            return current->data.value;
        }
        current = current->next;
    }

    return NULL;  // Nyckeln hittades inte
}

// Remove
int removeKey(HashTable* ht, int key)
{
    if (ht == NULL)
    {
        return 0;
    }

    int index = hash(key);
    Node_hash* current = ht->table[index];
    Node_hash* previous = NULL;

    // Sök efter noden att ta bort
    while (current != NULL)
    {
        if (current->data.key == key)
        {
            // Om det är första noden, uppdatera tabellen
            if (previous == NULL)
            {
                ht->table[index] = current->next;
            }
            else
            {
                // Länka om previous till next
                previous->next = current->next;
            }

            // Uppdatera prev för nästa nod om den finns
            if (current->next != NULL)
            {
                current->next->prev = previous;
            }

            free(current);  // Frigör minnet
            return 1;  // Lyckades
        }
        previous = current;
        current = current->next;
    }

    return 0;  // Nyckeln hittades inte
}

// Debugfunktion som skriver ut allt som finns i tabellen
void printTable(HashTable* ht)
{
    if (ht == NULL)
    {
        return;
    }

    // Iterera genom alla buckets i hashtabellen
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        // Skriv ut bucket-numret
        sprintf(sprintfBuffer, "Bucket %d:", i);
        uarte_write(sprintfBuffer);
        Node_hash* current = ht->table[i];
        
        // Om bucket är tom, skriv ut "(empty)"
        if (current == NULL)
        {
            sprintf(sprintfBuffer, " (empty)\n");
            uarte_write(sprintfBuffer);
            continue;  // Gå vidare till nästa bucket
        }

        // Skriv ut alla noder i bucket
        while (current != NULL)
        {
            sprintf(sprintfBuffer, "\n\r [key=%d, value=%s]", current->data.key, current->data.value);
            uarte_write(sprintfBuffer);
            current = current->next;
        }
        // Avsluta bucket med en ny rad
        sprintf(sprintfBuffer, "\n");
        uarte_write(sprintfBuffer);
    }
}