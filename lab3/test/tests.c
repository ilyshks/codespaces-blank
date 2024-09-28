#include "unity.h"
#include <limits.h>
#include "../src/utils.h"
#include "../src/find_min_max.h"

// Настройка тестов
void setUp(void) {
    // Выполняется перед каждым тестом
}

void tearDown(void) {
    // Выполняется после каждого теста
}

// Тесты
void test_GetMinMax_EmptyArray(void) {
    int array[] = {};
    struct MinMax result = GetMinMax(array, 0, 0);
    TEST_ASSERT_EQUAL_INT(INT_MAX, result.min);
    TEST_ASSERT_EQUAL_INT(INT_MIN, result.max);
}

void test_GetMinMax_SingleElementArray(void) {
    int array[] = {42};
    struct MinMax result = GetMinMax(array, 0, 1);
    TEST_ASSERT_EQUAL_INT(42, result.min);
    TEST_ASSERT_EQUAL_INT(42, result.max);
}

void test_GetMinMax_MultipleElementsArray(void) {
    int array[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    struct MinMax result = GetMinMax(array, 0, 11);
    TEST_ASSERT_EQUAL_INT(1, result.min);
    TEST_ASSERT_EQUAL_INT(9, result.max);
}

void test_GetMinMax_SubArray(void) {
    int array[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    struct MinMax result = GetMinMax(array, 2, 7);
    TEST_ASSERT_EQUAL_INT(1, result.min);
    TEST_ASSERT_EQUAL_INT(9, result.max);
}

void test_GetMinMax_NegativeNumbers(void) {
    int array[] = {-3, -1, -4, -1, -5, -9, -2, -6, -5, -3, -5};
    struct MinMax result = GetMinMax(array, 0, 11);
    TEST_ASSERT_EQUAL_INT(-9, result.min);
    TEST_ASSERT_EQUAL_INT(-1, result.max);
}

void test_GetMinMax_MixedNumbers(void) {
    int array[] = {-3, 1, -4, 1, 5, -9, 2, 6, -5, 3, -5};
    struct MinMax result = GetMinMax(array, 0, 11);
    TEST_ASSERT_EQUAL_INT(-9, result.min);
    TEST_ASSERT_EQUAL_INT(6, result.max);
}

// Основная функция запуска тестов
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_GetMinMax_EmptyArray);
    RUN_TEST(test_GetMinMax_SingleElementArray);
    RUN_TEST(test_GetMinMax_MultipleElementsArray);
    RUN_TEST(test_GetMinMax_SubArray);
    RUN_TEST(test_GetMinMax_NegativeNumbers);
    RUN_TEST(test_GetMinMax_MixedNumbers);
    return UNITY_END();
}