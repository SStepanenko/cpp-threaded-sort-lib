////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file threaded_sort_class_test.h
/// @brief Test of Threaded_sort class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef THREADED_SORT_CLASS_TEST_H
#define THREADED_SORT_CLASS_TEST_H

#include "gtest/gtest.h"

class ThreadedSortClassTest : public ::testing::Test 
{
public:

  template <class T>
  static bool check_vector_for_ascending_sort(const std::vector<T>& vector);

private:
  
  template <class T>
  static void _generate_vector_descent_order(size_t size, std::vector<T>& vector);

};

template <class T>
bool ThreadedSortClassTest::check_vector_for_ascending_sort(const std::vector<T>& some_vector)
{
  bool sort_check_result = true;

  for (size_t i = 1; i < some_vector.size() && sort_check_result; i++)
  {
    sort_check_result = (some_vector[i - 1] <= some_vector[i]);
  }

  return sort_check_result;
}

template <class T>
static void _generate_vector_descent_order(int size, std::vector<T>& vector)
{
  vector.clear();

  vector.reserve(size);

  for (int i = 0; i < size; i++)
  {
    vector.push_back(static_cast<T>(size - i)); // exception
  }
}

#endif // THREADED_SORT_CLASS_TEST_H

