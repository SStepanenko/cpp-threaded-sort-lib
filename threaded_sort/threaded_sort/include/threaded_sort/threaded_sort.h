////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file threaded_sort.h
/// @brief Interface of the Threaded_sort class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef THREADED_SORT_H_61D1D410_A9F5_46F5_A29E_6DD2A8C4BDEA
#define THREADED_SORT_H_61D1D410_A9F5_46F5_A29E_6DD2A8C4BDEA

namespace My_cpp_libs
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class Threaded_sort: implements threaded quick sort algorithm.
/// ToDo: current version sorts only std::vector<T> in ascending order.
///       Should be modified to support this prototype:
///       template<class RandomIt, class Compare>
///       void sort(RandomIt first, RandomIt last, Compare comp);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Threaded_sort
{
public:

  /// @brief Implements threaded quick sort algorithm.
  /// @param <T> Type of elements in vector.
  /// @param vector Reference to vector which should be sorted.
  /// @exception bad_alloc
  /// @exception system_error
  /// @exception exception 
  template<class T>
  static void quick_sort(std::vector<T>& input_vector);

private:

  // Private constructor without implementation to prohibit using it.
  Threaded_sort();

  // Private copy constructor without implementation to prohibit using it.
  Threaded_sort(const Threaded_sort&);

  // Private assignment operator without implementation to prohibit using it.
  Threaded_sort& operator=(const Threaded_sort&);
};

template<class T>
void Threaded_sort::quick_sort(std::vector<T>& input_vector)
{
  if (input_vector.size() < 2)
  {
    return;
  }

  // Create asynchronous tasks manager.
  shared_ptr<Async_tasks_manager> tasks_manager = make_shared<Async_tasks_manager>(); // exception

  // Create top level quick sort asynchronous task.
  shared_ptr<Sort_async_task<T>> sort_async_task = make_shared<Sort_async_task<T>>(input_vector, 0,
    input_vector.size() - 1, tasks_manager); // exception

  // Add task to tasks manager.
  tasks_manager->add_task(sort_async_task);

  // Execute sort synchronous task.
  sort_async_task->execute();

  // Wait while all asynchronous sort tasks are completed.
  tasks_manager->wait_for_all_tasks_completion();

  // Get execution error.
  shared_ptr<exception> execution_error = tasks_manager->get_error();

  // Throw error if it occurred on execution.
  if (execution_error != nullptr)
  {
    throw exception(*execution_error);
  }
}

} // My_cpp_libs

#endif // THREADED_QUICK_SORT_H_61D1D410_A9F5_46F5_A29E_6DD2A8C4BDEA
