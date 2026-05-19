# Russian Peasant Multiplication and Fast Powering

These notes are based on Konstantin Vladimirov’s C programming/math lecture. The main idea is: instead of doing many repeated additions or multiplications, we use the **binary representation** of a number.

This is important for C/C++ because it connects math, bit operations, performance, modular arithmetic, and algorithms used in cryptography and competitive programming. 

---

# 1. Naive multiplication

Suppose we want to calculate:

```text id="0uyrq4"
67 * 13
```

The most primitive interpretation is:

```text id="8xj4s2"
67 * 13 = 67 + 67 + 67 + ... + 67
```

`67` added `13` times.

That works, but it is inefficient.

Complexity:

```text id="m96gdj"
O(b)
```

for `a * b`, because we add `a` exactly `b` times.

For large numbers, this is too slow.

---

# 2. Binary representation idea

Instead of thinking about `13` as thirteen ones, represent it in binary:

```text id="mcdom1"
67 = 1000011₂
13 = 1101₂
```

Now:

```text id="g6l240"
13 = 8 + 4 + 1
```

because:

```text id="13v965"
1101₂ = 1*8 + 1*4 + 0*2 + 1*1
```

So:

```text id="d6eh02"
67 * 13 = 67 * (8 + 4 + 1)
        = 67 * 8 + 67 * 4 + 67
```

This replaces 13 additions with only a few doublings and additions.

---

# 3. Binary multiplication visually

Decimal:

```text id="0sz8pd"
67 * 13
```

Binary:

```text id="dw3jk2"
1000011₂ * 1101₂
```

Long multiplication:

```text id="szwc9x"
      1000011
x        1101
-------------
      1000011      // bit 0 of 13 is 1 -> add 67
     0000000       // bit 1 of 13 is 0 -> skip
    1000011        // bit 2 of 13 is 1 -> add 67 << 2
   1000011         // bit 3 of 13 is 1 -> add 67 << 3
-------------
   1101100111
```

So:

```text id="l8ll7g"
1000011₂ * 1101₂ = 1101100111₂
```

Check in decimal:

```text id="xf1ztp"
67 * 13 = 871
1101100111₂ = 871
```

---

# 4. Russian peasant multiplication

Russian peasant multiplication uses this binary idea.

Instead of multiplying directly, we repeatedly:

```text id="3eyaj5"
double the first number
halve the second number
add the first number when the second number is odd
```

Example:

```text id="6appko"
67 * 13
```

Table:

```text id="xojc3i"
a      b      action
67     13     b is odd  -> add 67
134    6      b is even -> skip
268    3      b is odd  -> add 268
536    1      b is odd  -> add 536
```

Sum:

```text id="77rr30"
67 + 268 + 536 = 871
```

So:

```text id="39hbef"
67 * 13 = 871
```

Why does it work?

Because:

```text id="1vjny3"
13 = 1101₂ = 8 + 4 + 1
```

And the doubled values are:

```text id="6x8w0e"
67       = 67 * 1
134      = 67 * 2
268      = 67 * 4
536      = 67 * 8
```

We add only the rows where the binary digit of `13` is `1`.

---

# 5. Multiplication algorithm in C

```c id="djyvy1"
unsigned multiply(unsigned a, unsigned b) {
    unsigned result = 0;

    while (b > 0) {
        if (b % 2 == 1) {
            result += a;
        }

        a *= 2;
        b /= 2;
    }

    return result;
}
```

More bit-oriented version:

```c id="b0z0tc"
unsigned multiply(unsigned a, unsigned b) {
    unsigned result = 0;

    while (b > 0) {
        if (b & 1u) {
            result += a;
        }

        a <<= 1;
        b >>= 1;
    }

    return result;
}
```

Explanation:

```text id="r34xdh"
b & 1      checks whether b is odd
a <<= 1    multiplies a by 2
b >>= 1    divides b by 2
```

Complexity:

```text id="90zytp"
O(log b)
```

because each step halves `b`.

For example:

```text id="d4qzsy"
1 billion ≈ 2^30
```

So multiplying by a billion needs about `30` loop iterations, not one billion additions.

---

# 6. Fast exponentiation idea

Naive power:

```text id="1m78ba"
67^13 = 67 * 67 * 67 * ... * 67
```

This takes `13 - 1` multiplications.

But again:

```text id="4nh70u"
13 = 1101₂ = 8 + 4 + 1
```

So:

```text id="7s8r22"
67^13 = 67^(8 + 4 + 1)
      = 67^8 * 67^4 * 67^1
```

Now we can build powers by squaring:

```text id="4tp594"
67^1
67^2 = 67 * 67
67^4 = 67^2 * 67^2
67^8 = 67^4 * 67^4
```

Then:

```text id="bjly33"
67^13 = 67^8 * 67^4 * 67
```

This is called:

```text id="naoeix"
binary exponentiation
```

or:

```text id="i3glw9"
exponentiation by squaring
```

---

# 7. Fast power algorithm

```c id="80kveb"
unsigned power(unsigned n, unsigned k) {
    unsigned result = 1;
    unsigned base = n;

    while (k > 0) {
        if (k & 1u) {
            result *= base;
        }

        base *= base;
        k >>= 1;
    }

    return result;
}
```

Example:

```text id="161hnm"
n = 67
k = 13 = 1101₂
```

Algorithm checks bits of `k` from right to left:

```text id="0a2n7p"
bit 0 = 1 -> multiply by 67^1
bit 1 = 0 -> skip 67^2
bit 2 = 1 -> multiply by 67^4
bit 3 = 1 -> multiply by 67^8
```

So:

```text id="bucq3t"
result = 67^1 * 67^4 * 67^8 = 67^13
```

Complexity:

```text id="f5k1hf"
O(log k)
```

instead of:

```text id="0i5shc"
O(k)
```

---

# 8. Why modular exponentiation is needed

For large powers, numbers become enormous.

Example:

```text id="pg0sed"
67^13
67^1000
67^1,000,000
```

These do not fit into normal integer types.

Often we only need the result modulo `m`:

```text id="zbk9am"
n^k mod m
```

This appears in:

```text id="ch67e2"
cryptography
hashing
number theory
competitive programming
randomized algorithms
```

The key rule:

```text id="k6dx15"
(a * b) mod m = ((a mod m) * (b mod m)) mod m
```

So we can reduce after every multiplication.

---

# 9. Code from the lecture: `pow_mod`

```c id="5xlo2z"
unsigned pow_mod(unsigned n, unsigned k, unsigned m) {
    unsigned mult = n % m;
    unsigned prod = 1;

    while (k > 0) {
        if ((k % 2) == 1) {
            prod = (prod * mult) % m;
            k--;
        }

        mult = (mult * mult) % m;
        k /= 2;
    }

    return prod;
}
```

This computes:

```text id="4h5zm6"
n^k mod m
```

using binary exponentiation.

---

# 10. Meaning of variables

```c id="8n9ac5"
unsigned mult = n % m;
```

`mult` stores the current power of `n`.

At different stages:

```text id="qfrva7"
mult = n^(1) mod m
mult = n^(2) mod m
mult = n^(4) mod m
mult = n^(8) mod m
...
```

because each loop does:

```c id="rjj4rf"
mult = (mult * mult) % m;
```

which squares the current power.

---

```c id="qkpsk4"
unsigned prod = 1;
```

`prod` stores the accumulated answer.

At the end:

```text id="6n2s6k"
prod = n^k mod m
```

---

```c id="f69rno"
while (k > 0)
```

We process the binary digits of `k` until there are no bits left.

---

```c id="ch42u0"
if ((k % 2) == 1)
```

This checks whether the current lowest bit of `k` is `1`.

Equivalent bit version:

```c id="dl2r9a"
if (k & 1u)
```

If this bit is `1`, we need to include the current `mult` in the answer:

```c id="afqbr1"
prod = (prod * mult) % m;
```

Then:

```c id="lrgdsl"
k--;
```

makes `k` even before division by 2.

---

```c id="n3062d"
mult = (mult * mult) % m;
```

Move from:

```text id="64xnj7"
n^1  to n^2
n^2  to n^4
n^4  to n^8
n^8  to n^16
```

---

```c id="a9h6ng"
k /= 2;
```

Shift exponent right by one bit.

Equivalent:

```c id="1c7q34"
k >>= 1;
```

---

# 11. Step-by-step example: `pow_mod(67, 13, m)`

Binary:

```text id="qf4ere"
13 = 1101₂
```

The algorithm will use:

```text id="un8nuu"
67^1
67^4
67^8
```

because bits `0`, `2`, and `3` are set.

Conceptually:

```text id="tpzfrd"
67^13 mod m
= 67^(8 + 4 + 1) mod m
= (67^8 * 67^4 * 67^1) mod m
```

Table without concrete modulo values:

```text id="osrufr"
k       bit     mult meaning       action
13      1       67^1               prod *= 67^1
6       0       67^2               skip
3       1       67^4               prod *= 67^4
1       1       67^8               prod *= 67^8
0       -       stop               answer ready
```

---

# 12. Cleaner common version

This version is usually easier to read:

```c id="adf5lt"
unsigned pow_mod(unsigned n, unsigned k, unsigned m) {
    unsigned mult = n % m;
    unsigned prod = 1 % m;

    while (k > 0) {
        if (k & 1u) {
            prod = (prod * mult) % m;
        }

        mult = (mult * mult) % m;
        k >>= 1;
    }

    return prod;
}
```

Difference from the lecture version:

```text id="fp1dr3"
It does not manually do k-- when k is odd.
It simply processes the lowest bit and then shifts k right.
```

This is the standard binary exponentiation style.

---

# 13. Important problem: overflow before modulo

This line looks safe:

```c id="wjkzub"
prod = (prod * mult) % m;
```

But there is a subtle issue.

The multiplication happens first:

```text id="ywrt07"
prod * mult
```

Then modulo is applied.

If `prod * mult` overflows `unsigned`, the result is computed modulo `2^32` first on a 32-bit `unsigned`.

In C, unsigned overflow is defined, but mathematically it may produce the wrong result for modulo `m`.

Example problem:

```text id="lzq886"
We want: (a * b) mod m

But machine computes:
    (a * b mod 2^32) mod m
```

These are not always equal.

Safer version for 32-bit inputs:

```c id="t1i374"
#include <stdint.h>

unsigned pow_mod(unsigned n, unsigned k, unsigned m) {
    unsigned mult = n % m;
    unsigned prod = 1 % m;

    while (k > 0) {
        if (k & 1u) {
            prod = (uint64_t)prod * mult % m;
        }

        mult = (uint64_t)mult * mult % m;
        k >>= 1;
    }

    return prod;
}
```

Here:

```c id="jxjufm"
(uint64_t)prod * mult
```

performs multiplication in 64 bits, reducing risk of overflow.

---

# 14. Important edge cases

## `m == 0`

This is invalid:

```c id="dte97v"
n % m
```

if `m == 0`.

Modulo by zero is undefined behavior.

So a robust version should handle it:

```c id="f1shnq"
if (m == 0) {
    // error case
}
```

---

## `k == 0`

Mathematically:

```text id="rkuzti"
n^0 = 1
```

So:

```text id="6dnlzj"
pow_mod(n, 0, m) = 1 mod m
```

That is why cleaner code uses:

```c id="3ek83k"
unsigned prod = 1 % m;
```

Because if `m == 1`, then:

```text id="f0id9r"
1 mod 1 = 0
```

The original code returns `1` for `m == 1, k == 0`, which is not mathematically correct modulo `1`.

---

# 15. Final robust version

```c id="4vlzbl"
#include <stdint.h>

unsigned pow_mod(unsigned n, unsigned k, unsigned m) {
    if (m == 0) {
        return 0; // or handle as an error
    }

    unsigned mult = n % m;
    unsigned prod = 1 % m;

    while (k > 0) {
        if (k & 1u) {
            prod = (uint64_t)prod * mult % m;
        }

        mult = (uint64_t)mult * mult % m;
        k >>= 1;
    }

    return prod;
}
```

This is better because:

```text id="n3qfzt"
handles m == 1 correctly
avoids 32-bit overflow for unsigned inputs
uses bit operations for exponent bits
is standard binary exponentiation
```

---

# 16. Complexity

Naive exponentiation:

```text id="p69c7q"
n^k = n * n * n * ... * n
```

Complexity:

```text id="045qnb"
O(k)
```

Fast exponentiation:

```text id="zsevbw"
use binary representation of k
square base each step
multiply result only when bit is 1
```

Complexity:

```text id="f6z7y9"
O(log k)
```

Memory complexity:

```text id="nhwmlg"
O(1)
```

---

# 17. Big picture connection

The same binary idea appears in many algorithms:

```text id="go9iwy"
Russian peasant multiplication
binary exponentiation
modular exponentiation
fast matrix exponentiation
binary lifting
cryptography algorithms
fast polynomial/hash computations
```

Core trick:

```text id="nznmbz"
Represent number in binary.
Process one bit at a time.
Use doubling/squaring to jump through powers of two.
```

---

# 18. Interview-style summary

Russian peasant multiplication uses the binary representation of the multiplier. Instead of adding `a` exactly `b` times, we repeatedly double `a`, halve `b`, and add the current `a` only when the current bit of `b` is `1`. This reduces complexity from `O(b)` to `O(log b)`.

Binary exponentiation applies the same idea to powers. Since an exponent can be decomposed into powers of two, for example `13 = 8 + 4 + 1`, we can compute `n^13` as `n^8 * n^4 * n`. Powers like `n^2`, `n^4`, `n^8` are produced by repeated squaring. This reduces exponentiation from `O(k)` multiplications to `O(log k)`.

Modular exponentiation adds modulo reduction after each multiplication, using the identity `(a * b) mod m = ((a mod m) * (b mod m)) mod m`. This keeps numbers small and is essential in cryptography and number theory. In C, we must still be careful about overflow before applying `%`, so for 32-bit `unsigned` values it is safer to cast to `uint64_t` before multiplication.
