#include "../include/hash_table.h"

#include <stdint.h>
#include <assert.h>
#include <string.h>

#define INITIAL_CAPACITY 16  // must not be zero
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct {
    const char* key; //NULL Key = empty slot
    void* value;
} HashTableEntry;

// Hash table structure: create with ht_create, free with ht_destroy.
struct HashTable {
    HashTableEntry *entries; //hash slots
    size_t capacity; //size of _entries
    size_t length; //current number of items
};

HashTable *createTable(void) {
    HashTable *table = malloc(sizeof(HashTable));

    if(table == NULL) {
        return NULL;
    }

    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    table->entries = calloc(table->capacity, sizeof(HashTableEntry));

    if(table->entries == NULL) {
        free(table);
        return NULL;
    }

    return table;

}

void destroyTable(HashTable *table) {
    for(int i = 0; i < table->capacity; i++) {
        free((void*)table->entries[i].key);
        free(table->entries[i].value);
    }
    free(table->entries);
    free(table);
}

void* getValue(HashTable *table, const char* key) {
    uint64_t hash = hashKeyGenerator(key);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));
    // Loop till we find an empty entry.
    while(table->entries[index].key != NULL) {
        if(strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return table->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if(index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

// FNV-1a algorithm, transforms a given key into a hash value
static uint64_t hashKeyGenerator(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for(const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash; 
}

// Internal function to set an entry (without expanding table).
static const char* setEntry(HashTableEntry* entries, size_t capacity, const char* key, void* value, size_t* plength) {
    uint64_t hash = hashKeyGenerator(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));
    while(entries[index].key != NULL) {
        if(strcmp(key, entries[index].key) == 0) {
            // Found key (it already exists), update value.
            entries[index].value = value;
            return entries[index].key;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if(index >= capacity) {
            index = 0;
        }
    }
    // Didn't find key, allocate+copy if needed, then insert it.
    if(plength != NULL) { 
        key = strdup(key); //duplicate memory
        if(key == NULL) {
            return NULL;
        }
        (*plength)++;
    } 

    entries[index].key = (char*)key;
    entries[index].value = value;
    
    return key;
}

const char* setValue(HashTable* table, const char* key, void* value) {
    assert(value != NULL);
    if(value == NULL) {
        return NULL;
    }
    // If length will exceed half of current capacity, expand it.
    if(table->length >= table->capacity / 2) {
        if(!tableExpand(table)) {
            return NULL;
        }
    }
    // Set entry and update length.
    return setEntry(table->entries, table->capacity, key, value, &table->length);
}

//allocates a new entries array of double the current capacity
static int tableExpand(HashTable* table) {
    // Allocate new entries array.
    size_t new_capacity = table->capacity * 2;
    if (new_capacity < table->capacity) {
        return 0;  // overflow (capacity would be too big)
    }
    HashTableEntry* newEntries = calloc(new_capacity, sizeof(HashTableEntry));
    if (newEntries == NULL) {
        return 0;
    }
    // Iterate entries, move all non-empty ones to new table's entries.
    for (size_t i = 0; i < table->capacity; i++) {
        HashTableEntry entry = table->entries[i];
        if (entry.key != NULL) {
            setEntry(newEntries, new_capacity, entry.key, entry.value, NULL);
        }
    }
    // Free old entries array and update this table's details.
    free(table->entries);
    table->entries = newEntries;
    table->capacity = new_capacity;
    return 1;
}

size_t lengthOfTable(HashTable *table) {
    return table->length;
}

HashTableIterator hashTableIteratorGenerator(HashTable *table) {
    HashTableIterator iterator;
    iterator._table = table;
    iterator._index = 0;
    return iterator;
}

int getNextIteration(HashTableIterator *iterator) {
    HashTable* table = iterator->_table;
    while(iterator->_index < table->capacity) {
        size_t i = iterator->_index;
        iterator->_index++;
        if(table->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            HashTableEntry entry = table->entries[i];
            iterator->key = entry.key;
            iterator->value = entry.value;
            return 1;
        }
    }
    return 0;
}

