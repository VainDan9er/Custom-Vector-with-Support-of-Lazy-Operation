# Custom Vector with Lazy Evaluation & Raw Memory Management

A from-scratch C++ `Vector` implementation focused on **low-level memory control** and **lazy arithmetic operations** (expression templates).

### Key Features

- **Raw memory management**
  - Manual allocation via custom allocator
  - Object construction with **placement new**
  - Explicit control over construction/destruction of elements
  - Separation of memory allocation and object lifetime

- **Lazy operations (`+`, `-`, `*`)**
  - Expression templates (`AddExp`, `MulExp`, `SubExp`...)
  - No temporary vectors are created for intermediate results
  - Entire expression is evaluated in a **single pass** only when assigned to a `Vector`
  - Correct operator precedence and associativity are preserved

- **Additional features**
  - Support for negative indices via `std::ptrdiff_t` (Python-style)
  - Strong exception safety considerations during construction
  - Educational focus on understanding how `std::vector` works under the hood

### Why Lazy Evaluation?

Classic implementation:
```cpp

Vector result = a + b * c - d;   // creates several temporary Vectors
