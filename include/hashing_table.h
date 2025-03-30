#ifndef hashing_table_h
#define hashing_table_h

#include <stdlib.h>

// Hash table structure: create with ht_create, free with ht_destroy.
typedef struct HashingTable HashingTable;

//returns a pointer to the created hashing table, null if out of memory
HashingTable* createTable(void);

//free the memory of a h_table, including allocated keys
void destroyTable(HashingTable *table);

//gives a pointer of the value of a given key in a given hashing table, or null if couldn't find it 
void* getValue(HashingTable *table, const char* key);

// Set item with given key (NUL-terminated) to value (which must not
// be NULL). If not already present in table, key is copied to newly
// allocated memory (keys are freed automatically when destroyTable is
// called). Return address of copied key, or NULL if out of memory.
const char* setValue(HashingTable* table, const char* key, void* value);

// returns number of items in the table
size_t lengthOfTable(HashingTable* table);

// Hash table iterator: create with ht_iterator, iterate with ht_next.
typedef struct {
    const char* key;
    void* value;

    //don't use this fields directly
    HashingTable* _table; //reference to the ht being iterated
    size_t _index; //curent index

} HashingTableIterator;

//creates a Iterator for a given table
HashingTableIterator ht_iterator(HashingTable *table);

// Move iterator to next item in hash table, update iterator's key
// and value to current item, and return true. If there are no more
// items, return false. Don't call ht_set during iteration.
bool getNextIteration(HashingTableIterator* iterator);

#endif