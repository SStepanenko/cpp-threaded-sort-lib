# cpp-threaded-sort-lib

My C++ implementation of threaded quicksort algorithm. Project is implemented in Visual Studio 2013: C++ static library with tests.

**Coding style:**  [JSF-AV rules](http://www.stroustrup.com/JSF-AV-rules.pdf)
  
## Algorithm description

Quicksort algorithm is recursive by it's nature and my implementation combines recursion and execution of algorithm in separate thread.
Algorithm accepts input parameter "maximum depth of recursion" and when this constraint is reached then sorting of next fragment of array is executed in new thread.

## Ideas of how algorithm can be enhanced

* In addition to "maximum recursion depth" constraint it is worth to add "maximum active threads count" constraint.
* May be for sorting small parts of array other sorting algorithms are more effective and should be used instead of quicksort algorithm.
* Think about other ways of optimization.

* Current version sorts only std::vector<T> in ascending order.
    Implementation should be modified to support this prototype:
        template < class RandomIt, class Compare >
        void sort(RandomIt first, RandomIt last, Compare comp);

## Folders Structure

* Project of threaded_sort library: threaded_sort/threaded_sort.
* Project of library tests: threaded_sort/threaded_sort_test.

To execute unit tests put the folder 3rd_party_libs with headers and built googletest libraries to the folder of this reposotory. You can take 3rd_party_libs folder from here:
https://github.com/SStepanenko/my-cpp-test-tasks/tree/master/3rd_party_libs
Or you can download and build googletest library by yourself.
