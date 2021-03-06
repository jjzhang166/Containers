# Containers
This library provides various containers. Each container has utility functions to manipulate the data it holds. This is an abstraction as to not have to manually manage and reallocate memory. 

Inspired by the C++ standard library. Implementation of the C++ standard library data structures using C; however, the data structures have different interfaces than those in the C++ standard library.

# Sequence containers
Data structures which can be accessed sequentially.
<p> array - static contiguous array
<p> vector - dynamic contiguous array
<p> deque - double-ended queue
<p> forward_list - singly-linked list
<p> list - doubly-linked list

# Associative containers
Data structures that can be quickly searched which use comparators.
<p> set - collection of unique keys, sorted by keys
<p> map - collection of key-value pairs, sorted by keys, keys are unique
<p> multiset - collection of keys, sorted by keys
<p> multimap - collection of key-value pairs, sorted by keys

# Unordered associative containers
Data structures that can be quickly searched which use hashing.
<p> unordered_set - collection of unique keys, hashed by keys
<p> unordered_map - collection of key-value pairs, hashed by keys, keys are unique
<p> unordered_multiset - collection of keys, hashed by keys
<p> unordered_multimap - collection of key-value pairs, hashed by keys

# Container adaptors
Data structures which adapt other containers to enhance functionality.
<p> stack - adapts a container to provide stack (last-in first-out)
<p> queue - adapts a container to provide queue (first-in first-out)
<p> priority_queue - adapts a container to provide priority queue
