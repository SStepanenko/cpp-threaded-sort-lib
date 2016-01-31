# cpp-threaded-sort-lib

My C++ implementation of threaded quicksort algorithm. Project is implemented in Visual Studio 2013: C++ static library with tests.

I heard a lot (and jokes too :-) ) about such a popular test task for programmers as a multi-threaded implementation of quicksort algorithm and I decided to try to implement it by myself. This is just a first implementation, may be not so quick and not optimal but it works (for not very big arrays).

**Coding style:** http://source.android.com/source/code-style.html

## Not solved problems

Threads manager can't delete just completed threads, attempt to do it causes errors and threads are terminated with code 3. All threads created by algorithm are put to list of completed threads. Problem should be investigated and resolved otherwise it will be impossible to sort big arrays because of system resources limitation.
  
## Thoughts of algorithm optimization

* Think how algorithm can be enhanced. 
Algorithm should analyze whether it is worth to execute quicksort for part of array in new thread or it is better to call function recursively (depth of recursion should be controlled). Or may be for sorting small parts of array other sorting algorithms are more effective and should be used instead of quicksort algorithm. Think about other ways of optimization… 

* Current version sorts only std::vector<T> in ascending order.
    Implementation should be modified to support this prototype:
        template<class RandomIt, class Compare>
        void sort(RandomIt first, RandomIt last, Compare comp);

## Folders Structure

* Project of threaded_sort library: threaded_sort/threaded_sort
* Project of library tests: threaded_sort/threaded_sort_test
