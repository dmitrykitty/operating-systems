# Fibonacci Numbers: Modulo, Pisano Periods, Fibonacci Numeral System, and Matrix Exponentiation

## 1. Basic Fibonacci sequence

The Fibonacci sequence is usually defined as:

```text
F_0 = 0
F_1 = 1
F_n = F_{n-1} + F_{n-2}
```

So the sequence starts as:

```text
0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...
```

A simple iterative implementation stores only two consecutive Fibonacci numbers:

```c
int fib_pow_x_mod_m(int x, int m){
    int first = 0, second = 1, i, new; 

    for(i = 0; i < x; i++){
        printf("%d: %d\n", i, first);
        new = (first + second) % m; 
        first = second; 
        second = new;
    }
    return first;
}
```

At every moment:

```text
first  = F_i
second = F_{i+1}
```

After one iteration:

```text
new = F_i + F_{i+1} = F_{i+2}
```

Then we shift:

```text
first  = F_{i+1}
second = F_{i+2}
```

So this is a linear `O(n)` algorithm.

---

# 2. Why compute Fibonacci modulo `m`?

Fibonacci numbers grow very fast. Approximately:

```text
F_n ≈ φ^n / sqrt(5)
```

where:

```text
φ = (1 + sqrt(5)) / 2 ≈ 1.618
```

So even relatively small indices produce huge numbers.

For example, `F_93` already does not fit into signed `long long`.

But if we only need:

```text
F_n mod m
```

we do not need to store the full Fibonacci number.

Instead of calculating:

```text
F_n
```

and then reducing it modulo `m`, we can reduce after every addition:

```c
new = (first + second) % m;
```

This works because:

```text
(a + b) mod m = ((a mod m) + (b mod m)) mod m
```

So the values always stay in the range:

```text
0, 1, ..., m - 1
```

This is a very important low-level and algorithmic idea: **when the final answer is modulo `m`, avoid building enormous intermediate values.**

---

# 3. Fibonacci sequence modulo `m`

For a fixed `m`, we can look at:

```text
F_0 mod m, F_1 mod m, F_2 mod m, ...
```

Example for `m = 2`:

```text
F_n:      0 1 1 2 3 5 8 13 21 ...
mod 2:    0 1 1 0 1 1 0 1 1 ...
```

The pattern repeats:

```text
0, 1, 1
```

So the period for modulo `2` is:

```text
3
```

Example for `m = 3`:

```text
F_n mod 3:
0, 1, 1, 2, 0, 2, 2, 1, 0, 1, 1, 2, ...
```

The period is:

```text
8
```

This repeating period is called the **Pisano period**.

---

# 4. Pisano period

The Pisano period of `m` is usually written as:

```text
π(m)
```

It is the smallest positive number such that:

```text
F_{n + π(m)} ≡ F_n mod m
```

for every `n`.

In simple words:

```text
The Fibonacci sequence modulo m eventually repeats, and π(m) is the length of this cycle.
```

---

# 5. Why does the Pisano period exist?

The Fibonacci sequence is determined by two previous values:

```text
F_{n+2} = F_{n+1} + F_n
```

So to continue the sequence, we only need the pair:

```text
(F_n mod m, F_{n+1} mod m)
```

Each value can be one of:

```text
0, 1, ..., m - 1
```

So there are at most:

```text
m * m = m^2
```

possible pairs.

For example, if `m = 10`, there are only `100` possible states:

```text
(0,0), (0,1), (0,2), ..., (9,9)
```

The transition is deterministic:

```text
(a, b) -> (b, (a + b) mod m)
```

That means if the same pair appears twice, everything after it will repeat in exactly the same way.

Since there are finitely many states and infinitely many Fibonacci terms, some state must eventually repeat.

That gives us periodicity.

---

# 6. Why does the period return to `(0, 1)`?

The starting Fibonacci state is:

```text
(F_0, F_1) = (0, 1)
```

Modulo `m`, the starting state is still:

```text
(0, 1)
```

The transition is:

```text
(a, b) -> (b, a + b mod m)
```

This transition is reversible.

If we know:

```text
current = (a, b)
```

then the previous state was:

```text
(b - a mod m, a)
```

because:

```text
F_{n-1} = F_{n+1} - F_n
```

This reversibility is important.

It means the sequence of states does not have a “tail” before entering a cycle. It is cyclic from the beginning.

Therefore, once a state repeats, the first repeated important state is the initial one:

```text
(0, 1)
```

That is why your code checks:

```c
if(first == 0 && second == 1){
    break;
}
```

---

# 7. Your Pisano period code

```c
llu find_pisano_period(llu m){
    llu first = 0, second = 1, n = 0, new; 

    while(1){
        new = (first + second) % m; 
        first = second; 
        second = new; 
        n++; 

        if(first == 0 && second == 1){
            break;
        }
    }

    return n; 
}
```

This code stores the current Fibonacci state:

```text
first  = F_n mod m
second = F_{n+1} mod m
```

Each loop iteration moves one step forward:

```text
(F_n, F_{n+1}) -> (F_{n+1}, F_{n+2})
```

When the state becomes:

```text
(0, 1)
```

again, the full sequence starts repeating.

The number of steps taken is the Pisano period.

---

# 8. Why Pisano periods are useful

If we know `π(m)`, then:

```text
F_x mod m = F_{x mod π(m)} mod m
```

This is extremely useful for huge indices.

Example:

```text
π(10) = 60
```

Therefore:

```text
F_1,000,000 mod 10 = F_(1,000,000 mod 60) mod 10
```

Since:

```text
1,000,000 mod 60 = 40
```

we only need:

```text
F_40 mod 10
```

instead of computing one million Fibonacci steps.

---

# 9. Huge indices that do not fit in machine types

Sometimes `x` can be too large even for `unsigned long long`.

For example:

```text
x = 123456789123456789123456789123456789
```

This number cannot fit into normal integer types.

But if we need:

```text
F_x mod m
```

and know the Pisano period `π(m)`, then we only need:

```text
x mod π(m)
```

This can be calculated from a string:

```c
llu mod_string(const char* s, llu mod) {
    llu result = 0;

    for (int i = 0; s[i] != '\0'; ++i) {
        result = (result * 10 + (s[i] - '0')) % mod;
    }

    return result;
}
```

The idea is:

```text
We never construct the full huge number.
We process it digit by digit.
We keep only the remainder.
```

This is very common in competitive programming, cryptography, and number theory algorithms.

---

# 10. Your code using Pisano period

```c
llu pisano_period = find_pisano_period(m);
llu fib_num_x = find_x_fib_by_mod(x % pisano_period, m);
printf("%llu %llu", fib_num_x, pisano_period);
```

The logic is:

```text
1. Find π(m)
2. Reduce x to x mod π(m)
3. Compute F_(x mod π(m)) mod m
```

So instead of calculating:

```text
F_x mod m
```

directly, you calculate:

```text
F_(x mod π(m)) mod m
```

This is mathematically equivalent.

---

# 11. Important technical problems in this code

## Problem 1: `m = 1`

If `m = 1`, then every value modulo `1` is `0`.

So the state `(0, 1)` will never appear again, because `1 mod 1 = 0`.

This means your loop can become infinite.

You need a special case:

```c
if (m == 1) {
    return 1;
}
```

And for the result:

```text
F_x mod 1 = 0
```

## Problem 2: overflow in addition

This line can overflow:

```c
new = (first + second) % m;
```

Even though `first` and `second` are smaller than `m`, their sum can overflow `unsigned long long` if `m` is very large.

Safer version with GCC/Clang:

```c
new = ((unsigned __int128)first + second) % m;
```

Or implement safe modular addition manually.

This is a real low-level issue: mathematically the expression is fine, but machine arithmetic has fixed-width limits.

---

# 12. Fibonacci numeral system

The Fibonacci numeral system represents integers as sums of Fibonacci numbers.

Instead of binary weights:

```text
1, 2, 4, 8, 16, ...
```

we use Fibonacci weights:

```text
1, 2, 3, 5, 8, 13, 21, ...
```

Usually we start from:

```text
1, 2
```

not:

```text
1, 1, 2, 3, ...
```

because two `1`s would make representation non-unique.

Example:

```text
10 = 8 + 2
```

Using weights:

```text
8 5 3 2 1
```

we get:

```text
1 0 0 1 0
```

So:

```text
10 = 10010_F
```

---

# 13. Your Fibonacci numeral system code

```c
#define MAX_SIZE 64

int fill_fib_and_get_last(int* arr, int sz, int target){
    int idx = 2;
    arr[0] = 1; arr[1] = 2; 
    
    while(idx < MAX_SIZE){
        int next = arr[idx - 2] + arr[idx - 1];

        if(next > target){
            break;
        }
        arr[idx] = next;
        idx++;
    } 
    return idx - 1; 
}
```

This builds Fibonacci weights:

```text
1, 2, 3, 5, 8, 13, ...
```

up to the largest Fibonacci number not greater than `target`.

Then:

```c
void print_fs(int* arr, int last, int target) {
    for(last; last >= 0; last--){
        if(target >= arr[last]){
            printf("1");
            target -= arr[last];
        } else {
            printf("0");
        }
    }
}
```

This is a greedy algorithm:

```text
1. Take the largest Fibonacci number <= target.
2. Print 1.
3. Subtract it.
4. Move to the next smaller Fibonacci number.
5. Print 0 if it cannot be used.
```

Example for `10`:

```text
Fibonacci weights: 8 5 3 2 1

10 >= 8 -> print 1, target = 2
2 < 5  -> print 0
2 < 3  -> print 0
2 >= 2 -> print 1, target = 0
0 < 1  -> print 0
```

Result:

```text
10010
```

---

# 14. Zeckendorf theorem

The theory behind this system is called **Zeckendorf’s theorem**.

It says:

```text
Every positive integer can be represented uniquely as a sum of non-consecutive Fibonacci numbers.
```

In other words, every number has a unique representation using Fibonacci numbers where no two selected Fibonacci numbers are adjacent.

Example:

```text
100 = 89 + 8 + 3
```

The selected Fibonacci numbers are not neighbors in the Fibonacci sequence.

---

# 15. Why there are no two adjacent `1`s

The key identity is:

```text
F_k + F_{k-1} = F_{k+1}
```

So if a representation used two neighboring Fibonacci numbers:

```text
F_k + F_{k-1}
```

we could replace them with:

```text
F_{k+1}
```

Example:

```text
5 + 3 = 8
```

So a representation with adjacent ones is not canonical.

For example:

```text
01100
```

can be rewritten as:

```text
10000
```

That is why the valid Fibonacci representation never contains:

```text
11
```

It usually looks like:

```text
101001010
```

not:

```text
110011
```

---

# 16. Why the greedy algorithm works

Suppose the algorithm chooses the largest Fibonacci number:

```text
F_k <= N
```

By definition of largest:

```text
N < F_{k+1}
```

But:

```text
F_{k+1} = F_k + F_{k-1}
```

After subtracting `F_k`, the remainder is:

```text
R = N - F_k
```

Since:

```text
N < F_k + F_{k-1}
```

we get:

```text
R < F_{k-1}
```

That means the next smaller Fibonacci number `F_{k-1}` cannot be used.

Therefore, after every `1`, the next digit must be `0`.

This explains why the greedy algorithm naturally creates a representation without adjacent ones.

---

# 17. Is this NAF?

You mentioned NAF.

The Fibonacci numeral system is **not exactly NAF**, but the idea is related.

## Fibonacci representation

Weights:

```text
1, 2, 3, 5, 8, 13, ...
```

Digits:

```text
0 or 1
```

Rule:

```text
No adjacent 1s
```

Example:

```text
100101
```

## NAF — Non-Adjacent Form

Weights:

```text
1, 2, 4, 8, 16, ...
```

Digits:

```text
-1, 0, 1
```

Rule:

```text
No adjacent non-zero digits
```

Example:

```text
100-1001
```

So the similarity is:

```text
Both representations avoid adjacent non-zero digits.
```

But the difference is:

```text
Fibonacci system uses Fibonacci weights.
NAF uses powers of 2 with signed digits.
```

---

# 18. Why such representations are useful

The main idea is:

```text
The representation of a number can reduce the number of expensive operations.
```

In binary, each `1` often means “perform an addition/multiplication step”.

In NAF, using digits `-1, 0, 1` reduces the density of non-zero digits.

That matters in cryptography, especially elliptic curve cryptography, where scalar multiplication is expensive.

Fibonacci coding is also used in data compression and universal integer codes.

A special property of Fibonacci representation is that valid representations do not contain adjacent `1`s. Because of that, the sequence:

```text
11
```

can be used as a terminator in Fibonacci coding.

So Fibonacci coding can encode integers in a prefix-free way.

---

# 19. Can we compute `F_n` faster than `O(n)`?

The simple iterative algorithm takes:

```text
O(n)
```

because it moves one Fibonacci step at a time:

```text
F_0 -> F_1 -> F_2 -> ... -> F_n
```

But we can do better using matrices.

The key matrix is:

```text
Q = [1 1]
    [1 0]
```

This matrix transforms:

```text
[F_n    ]
[F_{n-1}]
```

into:

```text
[F_{n+1}]
[F_n    ]
```

because:

```text
[1 1] [F_n    ] = F_n + F_{n-1} = F_{n+1}
[1 0] [F_{n-1}] = F_n
```

So one multiplication by `Q` means “move one Fibonacci step forward”.

---

# 20. Matrix identity from the lecture

The important identity is:

```text
Q^n = [F_{n+1}  F_n    ]
      [F_n      F_{n-1}]
```

where:

```text
Q = [1 1]
    [1 0]
```

This means that if we can compute `Q^n`, then we get Fibonacci numbers directly from the matrix.

For example, the top-right element is:

```text
F_n
```

So:

```text
F_n = (Q^n)[0][1]
```

This is the mathematical reason why matrix exponentiation can compute Fibonacci numbers.

---

# 21. Why matrix exponentiation is faster

Computing `Q^n` naively would still take `O(n)` multiplications:

```text
Q * Q * Q * ... * Q
```

But we can use binary exponentiation.

The idea:

```text
Q^8 = (((Q^2)^2)^2)
```

Instead of multiplying 8 times, we square 3 times.

For a general `n`, we use its binary representation.

Example:

```text
n = 13 = 1101₂
```

So:

```text
Q^13 = Q^8 * Q^4 * Q^1
```

We compute powers:

```text
Q^1
Q^2
Q^4
Q^8
```

by repeated squaring.

That takes:

```text
O(log n)
```

matrix multiplications.

Since each matrix is only `2 x 2`, each multiplication is constant time.

Therefore Fibonacci can be computed in:

```text
O(log n)
```

instead of:

```text
O(n)
```

---

# 22. Why this is mathematically connected to arrays

Konstantin’s slide says that a `2 x 2` matrix is just four numbers.

So instead of storing:

```text
A[2][2]
```

we can store:

```text
A[4]
```

using the formula:

```text
A_ij = A[i * 2 + j]
```

For a matrix:

```text
[a b]
[c d]
```

we store:

```text
A[0] = a
A[1] = b
A[2] = c
A[3] = d
```

So:

```text
A[i][j] = A[i * 2 + j]
```

Example:

```text
A[0][0] = A[0 * 2 + 0] = A[0]
A[0][1] = A[0 * 2 + 1] = A[1]
A[1][0] = A[1 * 2 + 0] = A[2]
A[1][1] = A[1 * 2 + 1] = A[3]
```

This is exactly how row-major storage works.

---

# 23. Matrix multiplication for `2 x 2` matrices

If:

```text
A = [a b]
    [c d]

B = [e f]
    [g h]
```

then:

```text
A * B = [a*e + b*g    a*f + b*h]
        [c*e + d*g    c*f + d*h]
```

For Fibonacci modulo `m`, every operation should be done modulo `m`:

```text
(A * B)[i][j] mod m
```

This keeps numbers small.

---

# 24. C-style matrix representation

A compact representation:

```c
typedef unsigned long long llu;

typedef struct {
    llu v[4];
} Matrix2;
```

Indexing:

```c
#define AT(M, i, j) ((M).v[(i) * 2 + (j)])
```

Matrix multiplication modulo `m`:

```c
Matrix2 mul_mod(Matrix2 a, Matrix2 b, llu m) {
    Matrix2 r;

    r.v[0] = (a.v[0] * b.v[0] + a.v[1] * b.v[2]) % m;
    r.v[1] = (a.v[0] * b.v[1] + a.v[1] * b.v[3]) % m;
    r.v[2] = (a.v[2] * b.v[0] + a.v[3] * b.v[2]) % m;
    r.v[3] = (a.v[2] * b.v[1] + a.v[3] * b.v[3]) % m;

    return r;
}
```

For large `m`, multiplication may overflow `llu`, so a more robust implementation would use `unsigned __int128`.

---

# 25. Fast power for Fibonacci

Binary exponentiation:

```c
Matrix2 pow_mod(Matrix2 base, llu n, llu m) {
    Matrix2 result = {{1, 0, 0, 1}}; // identity matrix

    while (n > 0) {
        if (n & 1) {
            result = mul_mod(result, base, m);
        }

        base = mul_mod(base, base, m);
        n >>= 1;
    }

    return result;
}
```

The identity matrix is:

```text
I = [1 0]
    [0 1]
```

It is neutral for multiplication:

```text
I * A = A
A * I = A
```

Then Fibonacci:

```c
llu fib_mod_matrix(llu n, llu m) {
    if (m == 1) {
        return 0;
    }

    Matrix2 q = {{1, 1, 1, 0}};
    Matrix2 qn = pow_mod(q, n, m);

    return qn.v[1]; // top-right element = F_n
}
```

Because:

```text
Q^n = [F_{n+1}  F_n    ]
      [F_n      F_{n-1}]
```

So:

```text
Q^n[0][1] = F_n
```

---

# 26. Complexity comparison

## Simple iteration

```c
for (i = 0; i < n; ++i)
```

Complexity:

```text
Time:  O(n)
Space: O(1)
```

Good for small `n`.

## Pisano period method

If `m` is small and `n` is huge:

```text
F_n mod m = F_(n mod π(m)) mod m
```

Complexity depends on finding the period.

Useful when:

```text
m is small/moderate
n is enormous
```

especially when `n` is provided as a string.

## Matrix exponentiation

Complexity:

```text
Time:  O(log n)
Space: O(1)
```

Useful when:

```text
n is large
m can be large
we need direct fast computation
```

The matrix method is often more generally reliable than finding the Pisano period first.

---

# 27. Connection between all parts of the lecture

The lecture connects several deep ideas:

```text
1. Fibonacci recurrence
2. Modulo arithmetic
3. Finite state machines
4. Periodicity
5. Number representations
6. Greedy algorithms
7. Matrix algebra
8. Fast exponentiation
9. Array memory layout
```

The same Fibonacci sequence appears in different forms:

## As a recurrence

```text
F_n = F_{n-1} + F_{n-2}
```

## As a finite state transition modulo `m`

```text
(a, b) -> (b, a + b mod m)
```

## As a numeral system

```text
N = sum of non-consecutive Fibonacci numbers
```

## As a matrix power

```text
Q^n = [F_{n+1} F_n]
      [F_n F_{n-1}]
```

This is why the lecture is mathematically dense: it shows that the same object can be understood from different angles.

---

# 28. Interview-style summary

## What is a Pisano period?

A Pisano period `π(m)` is the period of the Fibonacci sequence modulo `m`.

```text
F_{n + π(m)} ≡ F_n mod m
```

It exists because the state `(F_n mod m, F_{n+1} mod m)` has only finitely many possible values.

## Why do we check for `(0, 1)`?

Because `(0, 1)` is the initial Fibonacci state. The transition is reversible, so the cycle starts from the initial state.

## Why can Pisano periods help with huge `n`?

Because:

```text
F_n mod m = F_(n mod π(m)) mod m
```

So even if `n` has hundreds of digits, we only need `n mod π(m)`.

## What is the Fibonacci numeral system?

It is a way to represent integers as sums of Fibonacci numbers:

```text
N = F_a + F_b + F_c + ...
```

with no two selected Fibonacci numbers adjacent.

## Why are there no adjacent `1`s?

Because:

```text
F_k + F_{k-1} = F_{k+1}
```

So two adjacent selected Fibonacci numbers can be replaced by one larger Fibonacci number.

## Is Fibonacci representation the same as NAF?

No. It is similar in spirit.

Fibonacci representation uses Fibonacci weights and digits `0/1`.

NAF uses powers of two and digits `-1/0/1`.

Both avoid adjacent non-zero digits.

## How do matrices speed up Fibonacci computation?

The matrix:

```text
Q = [1 1]
    [1 0]
```

moves the Fibonacci state forward by one step.

Therefore:

```text
Q^n
```

contains `F_n`.

Using binary exponentiation, we compute `Q^n` in:

```text
O(log n)
```

instead of `O(n)`.

---

# 29. Practical engineering lesson

This topic is not only math. It teaches several important systems and algorithmic ideas:

```text
Modulo arithmetic prevents integer explosion.
Finite state reasoning explains periodic behavior.
Greedy algorithms can be correct because of structural identities.
Matrix exponentiation turns linear recurrence into logarithmic computation.
Flat arrays can represent matrices efficiently in memory.
```

This is exactly the kind of topic that connects low-level C programming, algorithms, discrete math, and performance thinking.
