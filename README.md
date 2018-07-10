# Data structures and algorithms - C

**The cdsaext project provides a library of data structures and algorithms for software development in C.**  
*Similar libraries are also available for software development in C++ [(cppdsaext)](https://github.com/raltnoeder/cppdsaext), Java [(javadsaext)](https://github.com/raltnoeder/javadsaext) and Go [(godsaext)](https://github.com/raltnoeder/godsaext)*.

Check the **experimental** and various **future/...** branches for **recent development, fixes and upcoming new features.**

The primary project goals are:
- Correctness and robustness
- High quality implementation
- Clean and easy to use API
- Suitability for special applications (e.g., embedded systems)
- Lightweight and small enough to be verifiable/provable

## Available building blocks

**Algorithms & utility functions**  
bsearch - Binary search on a sorted array  

**Data structures**  
qtree - Sorted key/value map providing O(log(n)) lookup, insert, delete  
vmap - Double ended queue (deque) key/value map  
vlist - Double ended queue (deque) list  

**In development (experimental, future/...)**  
c\_integerparse - Safe and strictly typed string to number conversion  
