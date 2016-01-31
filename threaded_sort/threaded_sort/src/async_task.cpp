////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file async_task.cpp
/// @brief Implementation of the Async_class class.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"

namespace My_cpp_libs
{

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Async_task class members.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

atomic<int32_t> Async_task::s_netxt_task_id = 0;

Async_task::Async_task() : 
  m_task_id(Async_task::_get_next_task_id()),
  m_status(Async_task::Status::not_started)
{
  
}

Async_task::~Async_task()
{

}

int32_t Async_task::get_task_id() const
{
  return m_task_id;
}

Async_task::Status Async_task::get_status() const
{
  return m_status;
}

thread& Async_task::get_thread()
{
  return m_thread;
}

void Async_task::execute()
{
  if (get_status() == Status::started)
  {
    return;
  }

  try
  {
    m_thread = thread(&Async_task::_do_in_background, this); // exception
  }
  catch (system_error& error)
  {
    std::shared_ptr<exception> error_ptr = std::make_shared<system_error>(error); // exception

    _on_error(error_ptr);
  }
  catch (exception& error)
  {
    std::shared_ptr<exception> error_ptr = std::make_shared<exception>(error); // exception

    _on_error(error_ptr);
  }
}

const shared_ptr<exception>& Async_task::get_error() const
{
  return m_error;
}

bool Async_task::is_failed() const
{
  return (m_error != nullptr);
}

void Async_task::_on_error(const shared_ptr<std::exception>& error)
{
  assert(error != nullptr);

  _set_error(error);

  _set_status(Status::completed);
}

int32_t Async_task::_get_next_task_id()
{
  int32_t task_id = s_netxt_task_id++;

  return task_id;
}

void Async_task::_set_status(Async_task::Status status)
{
  m_status = status;
}

void Async_task::_set_error(const shared_ptr<exception>& error)
{
  m_error = error;
}

} // My_cpp_libs
