////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file async_tasks_manager.h
/// @brief Interface of the Async_tasks_manager class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ASYNC_TASKS_MANAGER_H_45F52716_F730_4541_9DC2_5BEB812CFE4E
#define ASYNC_TASKS_MANAGER_H_45F52716_F730_4541_9DC2_5BEB812CFE4E

namespace My_cpp_libs
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class Async_tasks_manager: asynchronous tasks manager.
///
/// ToDo: attempts to remove completed tasks cause exception and abnormal termination.
///       Problem must be solved otherwise it will be impossible to sort big arrays
///       because count of threads which can be created is limited.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Async_tasks_manager final
{
public:

  /// @brief Constructor.
  Async_tasks_manager();

  /// @brief Destructor.
  ~Async_tasks_manager();

  /// @brief Adds asynchronous task (bot doesn't start it).
  /// @param async_task New asynchronous task (should be not null).
  /// @exception bad_alloc
  void add_task(const std::shared_ptr<Async_task>& async_task);

  /// @brief Gets count of active tasks.
  int32_t active_tasks_count();

  /// @brief Gets exception occurred on some task execution.
  ///        If no errors occurred function returns null.
  std::shared_ptr<std::exception> get_error();

  /// @brief Checks if error occurred on some task execution.
  bool is_error_occurred();

  /// @brief Function should be called on task completion.
  /// @param task_id Asynchronous task identifier.
  /// @param error Error occurred on task execution or null if task completed successfully.
  /// @exception bad_alloc
  /// @exception system_error
  void handle_task_completion(int32_t task_id, const std::shared_ptr<std::exception>& error);

  /// @brief Checks if all tasks are completed.
  bool are_all_tasks_completed();

  /// @brief Waits for all tasks completion.
  void wait_for_all_tasks_completion();

private:

  // Private methods.

  // Private copy constructor without implementation to prohibit using it.
  Async_tasks_manager(const Async_tasks_manager&);

  // Private assignment operator without implementation to prohibit using it.
  Async_tasks_manager& operator=(const Async_tasks_manager&);

  /// @brief Joins threads of completed tasks (which can be joined) and removes them.
  /// @exception system_error
  void _join_and_remove_completed_tasks_which_can_be_joined();

  /// @brief Checks all tasks completion condition and signals if it is true.
  void _check_completion_condition();  

  // Private fields.

  // Map of active asynchronous tasks (key is task ID).
  std::map<int32_t, std::shared_ptr<Async_task>> m_active_tasks_map;

  // Vector of completed tasks.
  std::list<std::shared_ptr<Async_task>> m_completed_tasks_list;

  // Exception occurred on some task execution.
  std::shared_ptr<std::exception> m_error;

  // Mutex used for synchronization purposes.
  std::mutex m_common_mutex;

  // Mutex used for check of completion condition.
  std::mutex m_completion_condition_mutex;

  // Condition variable used to signal all task completion event.
  std::condition_variable m_completion_condition_variable;
  
}; // class Async_tasks_manager

} // My_cpp_libs

#endif // ASYNC_TASKS_MANAGER_H_45F52716_F730_4541_9DC2_5BEB812CFE4E
