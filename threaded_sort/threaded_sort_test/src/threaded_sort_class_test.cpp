////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file threaded_sort_class_test.cpp
/// @brief Test of Threaded_sort class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "threaded_sort_class_test.h"

using namespace std;

using namespace My_cpp_libs;

TEST(ThreadedSortClassTest, QuickSort)
{
  vector<int32_t> int_vector;
  const int32_t max_recursion_depth = 15;

  ASSERT_NO_THROW(_generate_vector_descent_order<int32_t>(10000000, int_vector)); // exception

  ASSERT_NO_THROW(Threaded_sort::quick_sort<int32_t>(int_vector, max_recursion_depth)); // exception

  bool is_vector_sorted = ThreadedSortClassTest::check_vector_for_ascending_sort(int_vector);

  EXPECT_EQ(true, is_vector_sorted);  

  // ToDo: add additional test cases...
}
