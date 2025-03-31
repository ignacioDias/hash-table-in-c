#ifndef hash_table_h
#define hash_table_h

#include <stdint.h>
#include <stdlib.h>

// Hash table structure: create with ht_create, free with ht_destroy.
typedef struct HashTable HashTable;

//returns a pointer to the created hashing table, null if out of memory
HashTable* createTable(void);

//free the memory of a h_table, including allocated keys
void destroyTable(HashTable *table);

//gives a pointer of the value of a given key in a given hashing table, or null if couldn't find it 
void* getValue(HashTable *table, const char* key);

// Set item with given key (NUL-terminated) to value (which must not
// be NULL). If not already present in table, key is copied to newly
// allocated memory (keys are freed automatically when destroyTable is
// called). Return address of copied key, or NULL if out of memory.
const char* setValue(HashTable* table, const char* key, void* value);

// returns number of items in the table
size_t lengthOfTable(HashTable* table);

static int tableExpand(HashTable* table);
static uint64_t hashKeyGenerator(const char* key);

// Hash table iterator: create with ht_iterator, iterate with ht_next.
typedef struct {
    const char* key;
    void* value;

    //don't use this fields directly
    HashTable* _table; //reference to the ht being iterated
    size_t _index; //curent index

} HashTableIterator;

//creates a Iterator for a given table
HashTableIterator hashTableIteratorGenerator(HashTable *table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call ht_set during iteration.
// returns an int as an boolean
int getNextIteration(HashTableIterator* iterator);

#endif