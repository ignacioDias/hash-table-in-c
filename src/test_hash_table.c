#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../include/hash_table.h"

// Test createTable and destroyTable
static void test_create_and_destroy_table(void **state) {
    HashTable *table = createTable();
    assert_non_null(table);
    assert_int_equal(lengthOfTable(table), 0);
    destroyTable(table);
}

// Test setValue and getValue
static void test_set_and_get_value(void **state) {
    HashTable *table = createTable();
    assert_non_null(table);

    const char *key1 = "key1";
    const char *value1 = "value1";
    const char *key2 = "key2";
    const char *value2 = "value2";

    setValue(table, key1, (void *)value1);
    setValue(table, key2, (void *)value2);

    assert_string_equal((char *)getValue(table, key1), value1);
    assert_string_equal((char *)getValue(table, key2), value2);

    destroyTable(table);
}

// Test lengthOfTable
static void test_table_length(void **state) {
    HashTable *table = createTable();
    assert_non_null(table);

    assert_int_equal(lengthOfTable(table), 0);

    setValue(table, "key1", "value1");
    assert_int_equal(lengthOfTable(table), 1);

    setValue(table, "key2", "value2");
    assert_int_equal(lengthOfTable(table), 2);

    destroyTable(table);
}

// Test hashTableIteratorGenerator and getNextIteration
static void test_iterator(void **state) {
    HashTable *table = createTable();
    assert_non_null(table);

    setValue(table, "key1", "value1");
    setValue(table, "key2", "value2");

    HashTableIterator iterator = hashTableIteratorGenerator(table);
    int count = 0;

    while (getNextIteration(&iterator)) {
        assert_non_null(iterator.key);
        assert_non_null(iterator.value);
        count++;
    }

    assert_int_equal(count, 2);

    destroyTable(table);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_create_and_destroy_table),
        cmocka_unit_test(test_set_and_get_value),
        cmocka_unit_test(test_table_length),
        cmocka_unit_test(test_iterator),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}