#ifndef BINARY_HEAP_ARRAY_HPP
#define BINARY_HEAP_ARRAY_HPP

#include <cstddef> // size_t
#include <utility> // std::move()
#include <cassert>
#include <functional>
#include <iostream>

/*
A Binary Heap is a Binary Tree with following properties.
1) It’s a complete tree (All levels are completely filled except possibly the last level and the last level has all keys as left as possible). This property of Binary Heap makes them suitable to be stored in an array.

2) A Binary Heap is either Min Heap or Max Heap. In a Min Binary Heap, the key at root must be minimum among all keys present in Binary Heap. The same property must be recursively true for all nodes in Binary Tree. Max Binary Heap is similar to MinHeap.

Here we implement the BinaryHeap using an array. It can also be implemented using a binary tree.


TIME COMPLEXITY:
- Get Minimum in Min Heap: O(1) [Or Get Max in Max Heap]
- Extract Minimum Min Heap: O(Log n) [Or Extract Max in Max Heap]
- Decrease Key in Min Heap: O(Log n)  [Or Extract Max in Max Heap]
- Insert: O(Log n)
- Delete: O(Log n)

SPACE COMPLEXITY:
- O(N) as we store n elements + pointers

ADVANTAGES:
- easy traversal to retrieve elements
- cache friendly

DISADVANTAGES:
- Heap is a special data structure and it cannot be used for searching of a particular element.

USAGES:
1. Dijkstra, Prim - Graph algorithms - Priority Queue
2. Used to efficiently find the k smallest (or largest) elements in an array, merging k sorted arrays, median of a stream, etc.
3. Scheduling processes in operating systems

Used to implement other data types as:
a) Priority Queue
b) 

MODEL:


*/



#endif /* BINARY_HEAP_ARRAY_HPP */