```c
x & (1u << k)       // test k-th bit
x | (1u << k)       // set
x & ~(1u << k)      // clear
x ^ (1u << k)       // toggle

x & (x - 1)         // remove lowest set bit
x & -x              // isolate lowest set bit
x && !(x & (x - 1)) // power of two
```