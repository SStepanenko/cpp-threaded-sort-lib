////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file sort_async_task.h
/// @brief Interface and implementation of the Sort_async_task<T> class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SORT_ASYNC_TASK_H_3B200A24_F0FD_4303_8F55_0DEB50D7A0AA
#define SORT_ASYNC_TASK_H_3B200A24_F0FD_4303_8F55_0DEB50D7A0AA

namespace My_cpp_libs
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class Sort_async_task<T>: asynchronous task which implements quick sort algorithm.
/// @param <T> Type of elements in vector.
/// ToDo: current version sorts only std::vector<T> in ascending order.
///       Should be modified to support this prototype:
///       template<class RandomIt, class Compare>
///       void sort(RandomIt first, RandomIt last, Compare comp);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
class Sort_async_task final : public Async_task
{
public:

  /// @brief Constructor.
  /// @param <T> Type of elements in vector.
  /// @param vector Reference to vector which should be sorted.
  /// @param max_recursion_depth Maximum recursion depth (should be positive value).
  /// @param left Index of element from which sorting range is started.
  /// @param right Index of element which encloses sorting range.
  /// @param tasks_manager Asynchronous tasks manager.
  Sort_async_task(std::vector<T>& vector, int32_t left, int32_t right,
    int32_t max_recursion_depth, std::shared_ptr<Async_tasks_manager> tasks_manager);

  /// @brief Destructor.
  virtual ~Sort_async_task();

private:

  // Nested classes.

  /// @brief Class which increments passed by reference variable in constructor and decrements in destructor.
  class Auto_incrementer final
  {
  public:

    Auto_incrementer(int32_t& counter) : m_counter(counter)
    {
      counter++;
    }

    ~Auto_incrementer()
    {
      m_counter--;
    }

  private:

    int32_t& m_counter;
  }; // class Auto_incrementer

  // Private methods.

  // Private copy constructor without implementation to prohibit using it.
  Sort_async_task(const Sort_async_task&);

  // Private assignment operator without implementation to prohibit using it.
  Sort_async_task& operator=(const Sort_async_task&);

  /// @brief Function which is executed in separate thread.
  ///        Should not throw exceptions.
  virtual void _do_in_background() override;

  /// @brief Function called when error occurred.
  /// @param error Exception occurred on execution.
  virtual void _on_error(const std::shared_ptr<std::exception>& error) override;

  /// @brief Starts new asynchronous task which implements quick sort algorithm.
  /// @param left Index of element from which sorting range is started.
  /// @param right Index of element which encloses sorting range.
  /// @exception std::bad_alloc
  void _start_new_sort_async_task(const int32_t left, const int32_t right);

  /// @brief Implements quick sort algorithm.
  /// @param left Index of element from which sorting range is started.
  /// @param right Index of element which encloses sorting range.
  /// @exception std::bad_alloc
  /// @exception std::system_error
  void _quick_sort(const int32_t left, const int32_t right);

  // Private static constants.

  // Private fields.

  // Reference to vector to be sorted.
  std::vector<T>& m_vector;

  // Index of element from which sorting range is started.
  const int32_t m_left;

  // Index of element which encloses sorting range.
  const int32_t m_right;

  // Maximum depth of recursion.
  const int32_t m_max_recursion_depth;

  // Asynchronous tasks manager.
  std::shared_ptr<Async_tasks_manager> m_tasks_manager;

  // Current level of recursion.
  int32_t m_recursion_level;
}; // class Sort_async_task

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implementation of the Sort_async_task<T> methods.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
Sort_async_task<T>::Sort_async_task(std::vector<T>& vector, int32_t left, int32_t right,
  int32_t max_recursion_depth, std::shared_ptr<Async_tasks_manager> tasks_manager) : 
    m_vector(vector),
    m_left(left),
    m_right(right),
    m_max_recursion_depth(max_recursion_depth),
    m_tasks_manager(tasks_manager),
    m_recursion_level(0)
{
  assert(left >= 0 && left < static_cast<int32_t>(vector.size()));
  assert(right >= 0 && right < static_cast<int32_t>(vector.size()));
  assert(left < right);
  assert(max_recursion_depth >= 0);
  assert(tasks_manager != nullptr);
}

template<class T>
Sort_async_task<T>:: ~Sort_async_task()
{
}

template<class T>
void Sort_async_task<T>::_do_in_background()
{
  bool is_result_ok = true;

  try
  {   
    _quick_sort(m_left, m_right); // exception
  }
  catch (std::exception& error)
  {
    std::shared_ptr<exception> error_ptr = std::make_shared<std::exception>(error);

    _on_error(error_ptr);

    is_result_ok = false;
  }

  if (is_result_ok)
  {
    m_tasks_manager->handle_task_completion(get_task_id(), nullptr);

    _set_status(Status::completed);
  }  
}

template<class T>
void Sort_async_task<T>::_on_error(const std::shared_ptr<std::exception>& error)
{
  assert(error != nullptr);

  Async_task::_on_error(error);

  m_tasks_manager->handle_task_completion(get_task_id(), error);
}

template<class T>
void Sort_async_task<T>::_start_new_sort_async_task(const int32_t left, const int32_t right)
{
  assert(left >= 0 && left < static_cast<int32_t>(m_vector.size()));
  assert(right >= 0 && right < static_cast<int32_t>(m_vector.size()));
  assert(left < right);

  std::shared_ptr<Async_task> sort_async_task = std::make_shared<Sort_async_task>(m_vector, left, right,
    m_max_recursion_depth, m_tasks_manager); // exception

  m_tasks_manager->add_task(sort_async_task); // exception

  sort_async_task->execute(); // exception
}

template<class T>
void Sort_async_task<T>::_quick_sort(const int32_t left, const int32_t right)
{ 
  Auto_incrementer recursion_level_incrementer(m_recursion_level);

  if(m_tasks_manager->is_error_occurred())
  {
    return;
  }

  assert(left >= 0 && left < static_cast<int32_t>(m_vector.size()));
  assert(right >= 0 && right < static_cast<int32_t>(m_vector.size()));
  assert(left < right);

  int32_t i = left;
  int32_t j = right;

  T comparand = m_vector[(left + right) / 2];

  do
  {
    while (m_vector[i] < comparand && i < right) i++;
    while (comparand < m_vector[j] && j > left) j--;

    if (i <= j)
    {
      std::swap(m_vector[i], m_vector[j]);
      i++;
      j--;
    }
  } while (i <= j);

  assert(j < i);

  if (left < j)
  {
    if (m_recursion_level <= m_max_recursion_depth)
    {
      _quick_sort(left, j); // exception
    }
    else
    {
      _start_new_sort_async_task(left, j); // exception
    }
  }

  if (i < right)
  {
    if (m_recursion_level <= m_max_recursion_depth)
    {
      _quick_sort(i, right); // exception
    }
    else
    {
      _start_new_sort_async_task(i, right); // exception
    }
  }
}

} // My_cpp_libs

#endif // SORT_ASYNC_TASK_H_3B200A24_F0FD_4303_8F55_0DEB50D7A0AA
