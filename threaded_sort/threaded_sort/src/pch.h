////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file pch.h
/// @brief Precompiled Header File.
/// @author Sergey Stepanenko (sergey.stepanenko.27@gmail.com)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_3649401B_54DF_4902_B7FD_B96319AAF96A
#define PCH_H_3649401B_54DF_4902_B7FD_B96319AAF96A

#include <stdint.h>
#include <vector>
#include <list>
#include <atomic>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <system_error>

#include "threaded_sort/async_task.h"
#include "threaded_sort/async_tasks_manager.h"
#include "threaded_sort/sort_async_task.h"
#include "threaded_sort/threaded_sort.h"

// Debug info.
#include <Windows.h>
#include <iostream>
#include <sstream>

#define OUTPUT_DEBUG( s ) \
do { \
   std::wostringstream os_; \
   os_ << s; \
   OutputDebugStringW( os_.str().c_str() ); \
} while(false)

#endif // PCH_H_3649401B_54DF_4902_B7FD_B96319AAF96A