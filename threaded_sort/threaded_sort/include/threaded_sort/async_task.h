////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file async_task.h
/// @brief Interface of the Async_task class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ASYNC_TASK_H_63ABC161_084C_4440_875E_FB8308E812F2
#define ASYNC_TASK_H_63ABC161_084C_4440_875E_FB8308E812F2

namespace My_cpp_libs
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Class Async_task: represents asynchronous task (wraps std::thread).
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Async_task
{
public:

  // Nested enumerations.
  
  /// @brief Status of asynchronous task.
  enum class Status
  {
    not_started, // Task is not started (set just after task construction).
    started,     // Task is started.
    completed    // Task is completed.
  };

  // Public methods.

  /// @brief Constructor.
  Async_task();

  /// @brief Destructor.
  virtual ~Async_task();

  /// @brief Gets task identifier (not a thread identifier).
  int32_t get_task_id() const;

  /// @brief Gets status of asynchronous task.
  Status get_status() const;

  /// @brief Gets reference to underlying thread.
  std::thread& get_thread();

  /// @brief Executes asynchronous task.
  /// @exception std::bad_alloc
  void execute();

  /// @brief Gets error: exception raised on task execution.
  ///        If task executed successfully function returns null.
  const std::shared_ptr<std::exception>& get_error() const;

  /// @brief Checks if error is occurred on task execution.
  bool is_failed() const;

protected:

  /// @brief Function which is passed to thread.
  ///        Should be overridden in derived class.
  ///        Function should not throw exceptions.
  virtual void _do_in_background() = 0;

  /// @brief Function called when error occurred.
  /// @param error Exception occurred on execution.
  virtual void _on_error(const std::shared_ptr<std::exception>& error);

  /// @brief Sets asynchronous task status.
  void _set_status(Status status);

  /// @brief Sets error.
  /// @param error Exception occurred on execution.
  void _set_error(const std::shared_ptr<std::exception>& error);

private:

  // Private static methods.

  /// @brief Gets next task identifier.
  static int32_t _get_next_task_id();

  // Private methods.

  // Private copy constructor without implementation to prohibit using it.
  Async_task(const Async_task&);

  // Private assignment operator without implementation to prohibit using it.
  Async_task& operator=(const Async_task&);

  // Private static fields.

  // Next task identifier.
  static std::atomic<int32_t> s_netxt_task_id;

  // Private fields.

  // Asynchronous task identifier.
  const int32_t m_task_id;

  // Asynchronous task status.
  std::atomic<Status> m_status;

  // Underlying thread used to execute this task.
  std::thread m_thread;

  // Exception occurred on task execution.
  std::shared_ptr<std::exception> m_error;
};

} // My_cpp_libs

#endif // ASYNC_TASK_H_63ABC161_084C_4440_875E_FB8308E812F2