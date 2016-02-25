# cpp-threaded-sort-lib

My C++ implementation of threaded quicksort algorithm. Project is implemented in Visual Studio 2013: C++ static library with tests.

I heard a lot (and jokes too :-) ) about such a popular test task for programmers as a multi-threaded implementation of quicksort algorithm and I decided to try to implement it by myself. This is just a first implementation, may be not so quick and not optimal but it works (for not very big arrays).

**Coding style:**  [JSF-AV rules](http://www.stroustrup.com/JSF-AV-rules.pdf)

## Not solved problems

Threads manager can't delete just completed threads, attempt to do it causes errors and threads are terminated with code 3. All threads created by algorithm are put to list of completed threads. Problem should be investigated and resolved otherwise it will be impossible to sort big arrays because of system resources limitation.
  
## Algorithm enhancement

* Think how algorithm can be enhanced. 
Algorithm should analyze whether it is worth to execute quicksort for part of array in new thread or it is better to call function recursively (depth of recursion should be controlled). Or may be for sorting small parts of array other sorting algorithms are more effective and should be used instead of quicksort algorithm. Think about other ways of optimization… 

* Current version sorts only std::vector<T> in ascending order.
    Implementation should be modified to support this prototype:
        template < class RandomIt, class Compare >
        void sort(RandomIt first, RandomIt last, Compare comp);

## Folders Structure

* Project of threaded_sort library: threaded_sort/threaded_sort.
* Project of library tests: threaded_sort/threaded_sort_test.
To execute unit tests put the folder 3rd_party_libs with headers and built googletest libraries to the folder of this repository. You can take 3rd_party_libs folder from here:
https://github.com/SStepanenko/my-cpp-test-tasks/tree/master/3rd_party_libs
Or you can download and build googletest library by yourself.
