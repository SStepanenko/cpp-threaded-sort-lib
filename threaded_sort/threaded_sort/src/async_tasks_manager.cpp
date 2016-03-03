////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file async_tasks_manager.cpp
/// @brief Implementation of the Async_tasks_manager class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"

namespace My_cpp_libs
{

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Async_tasks_manager class members.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Async_tasks_manager::Async_tasks_manager()
{

}

Async_tasks_manager::~Async_tasks_manager()
{

}

void Async_tasks_manager::add_task(const shared_ptr<Async_task>& async_task)
{
  assert(async_task != nullptr);

  lock_guard<mutex> lock(m_common_mutex);

  m_active_tasks_map[async_task->get_task_id()] = async_task; // exception
}

int32_t Async_tasks_manager::active_tasks_count()
{
  lock_guard<mutex> lock(m_common_mutex);

  return m_active_tasks_map.size();
}

shared_ptr<exception> Async_tasks_manager::get_error()
{
  lock_guard<mutex> lock(m_common_mutex);

  return m_error;
}

bool Async_tasks_manager::is_error_occurred()
{
  lock_guard<mutex> lock(m_common_mutex);

  return (m_error != nullptr);
}

void Async_tasks_manager::handle_task_completion(int32_t task_id, const std::shared_ptr<std::exception>& error)
{
  assert(task_id >= 0);

  lock_guard<mutex>lock(m_common_mutex);

  try
  {
    // Join completed tasks (which can be joined) and remove them. 
    // Otherwise deleting of completed tasks causes error!
    _join_and_remove_completed_tasks_which_can_be_joined(); // exception

    // Find task by ID.
    auto& task_iterator = m_active_tasks_map.find(task_id);

    if (task_iterator != m_active_tasks_map.end())
    { // Task was found.

      shared_ptr<Async_task> async_task = task_iterator->second;

      // Remove task from map.
      m_active_tasks_map.erase(task_iterator);

      m_completed_tasks_list.push_back(async_task); // exception
    }
    else
    {
      // Task with given ID not found: unexpected case.
      assert(false);
    }

    if (m_error == nullptr && error != nullptr)
    {
      m_error = error;
    }
  }
  catch (bad_alloc& error)
  {
    shared_ptr<exception> bad_alloc_error = make_shared<bad_alloc>(error); // exception

    m_error = bad_alloc_error;
  }

  // Check if all tasks are completed and signal to waiting thread.
  _check_completion_condition();
}

bool Async_tasks_manager::are_all_tasks_completed()
{
  lock_guard<mutex> common_mutext_lock(m_common_mutex);

  bool result = (m_active_tasks_map.size() == 0);

  return result;
}

void Async_tasks_manager::wait_for_all_tasks_completion()
{
  unique_lock<mutex> lock(m_completion_condition_mutex);

  while (!are_all_tasks_completed())
  {
    m_completion_condition_variable.wait(lock);
  }
}

void Async_tasks_manager::_join_and_remove_completed_tasks_which_can_be_joined()
{
  // Join threads of completed tasks (which can be joined) and remove them. 
  // Otherwise deleting of completed tasks causes error!
  for (auto iterator = m_completed_tasks_list.begin(); iterator != m_completed_tasks_list.end();)
  {
    if ((*iterator)->get_thread().joinable())
    {
      (*iterator)->get_thread().join(); // exception 

      iterator = m_completed_tasks_list.erase(iterator);
    }
    else
    {
      iterator++;
    }
  } // for
}

void Async_tasks_manager::_check_completion_condition()
{
  unique_lock<mutex> lock(m_completion_condition_mutex);

  bool all_tasks_completed = (m_active_tasks_map.size() == 0);

  if (all_tasks_completed)
  {
    m_completion_condition_variable.notify_all();
  }  
}

} // My_cpp_libs
