The goal of the extended Euclidean algorithm is not only to find:

```text
gcd(a, b)
```

but also to find numbers `u` and `v` such that:

```text
gcd(a, b) = u * a + v * b
```

This is called **Bézout identity**.

Example:

```text
gcd(17, 5) = 1

1 = (-2) * 17 + 7 * 5
```

So the algorithm returns:

```text
gcd = 1
u = -2
v = 7
```

---

# 1. Normal Euclidean algorithm

Assume:

```text
a >= b
```

We divide `a` by `b`:

```text
a = q[1] * b + r[1]
```

where:

```text
q1 = quotient
r1 = remainder
```

Then:

```text
b = q[2] * r[1] + r[2]
r[1] = q[3] * r[2] + r[3]
r[2] = q[4] * r[3] + r[4]
...
```

Eventually one remainder becomes zero.

Your notes use this notation:

```text
r[-1] = a
r[0]  = b

r[-1] = q[1] * r[0] + r[1]
r[0]  = q[2] * r[1] + r[2]
r[1]  = q[3] * r[2] + r[3]
...
r[n-2] = q[n] * r[n-1] + r[n]
r[n] = 0
```

The last non-zero remainder is the gcd:

```text
gcd(a, b) = r[n-1]
```

That is why in your notes the last non-zero remainder `r[n-1]` is circled as `d`, where:

```text
d = gcd(a, b)
```

---

# 2. Why do we rewrite it as `r[k] = r[k-2] - q[k] * r[k-1]`?

From division:

```text
r[k-2] = q[k] * r[k-1] + r[k]
```

we can move the term:

```text
r[k] = r[k-2] - q[k] * r[k-1]
```

Example:

```text
17 = 3 * 5 + 2
```

Then:

```text
2 = 17 - 3 * 5
```

So the remainder is expressed through the previous two numbers.

---

# 3. What extended Euclid adds

Normal Euclid only finds the gcd.

Extended Euclid wants every remainder to be represented as:

```text
r[k] = u[k] * a + v[k] * b
```

This means:

```text
Every remainder is some linear combination of the original a and b.
```

So the algorithm tracks not only:

```text
r[k]
```

but also:

```text
u[k] and v[k]
```

such that:

```text
r[k] = u[k] * a + v[k] * b
```

At the end, when:

```text
r[n-1] = gcd(a, b)
```

we also get:

```text
gcd(a, b) = u[n-1] * a + v[n-1] * b
```

That is the final goal.

---

# 4. Where do the `0` and `1` values come from?

This is the most important part.

At the beginning we have:

```text
r[-1] = a
r[0]  = b
```

Now we want to express both of them as:

```text
r[k] = u[k] * a + v[k] * b
```

For `r[-1] = a`:

```text
a = 1 * a + 0 * b
```

So:

```text
u[-1] = 1
v[-1] = 0
```

For `r[0] = b`:

```text
b = 0 * a + 1 * b
```

So:

```text
u[0] = 0
v[0] = 1
```

That is where the `0` and `1` values come from.

They are not magic. They are just the simplest way to write `a` and `b` using themselves.

```text
a = 1a + 0b
b = 0a + 1b
```

---

# 5. Deriving the formulas for `u[k]` and `v[k]`

We know:

```text
r[k] = r[k-2] - q[k] * r[k-1]
```

Assume we already know:

```text
r[k-2] = u[k-2] * a + v[k-2] * b
r[k-1] = u[k-1] * a + v[k-1] * b
```

Substitute them:

```text
r[k] = (u[k-2] * a + v[k-2] * b) - q[k] * (u[k-1] * a + v[k-1] * b)
```

Open brackets:

```text
r[k] = u[k-2] * a + v[k-2] * b - q[k] * u[k-1] * a - q[k] * v[k-1] * b
```

Group `a` and `b` terms:

```text
r[k] = (u[k-2] - q[k] * u[k-1]) * a + (v[k-2] - q[k] * v[k-1]) * b
```

Therefore:

```text
u[k] = u[k-2] - q[k] * u[k-1]
v[k] = v[k-2] - q[k] * v[k-1]
```

These are the red boxed formulas from your notes.

So we have three parallel recurrences:

```text
r[k] = r[k-2] - q[k] * r[k-1]

u[k] = u[k-2] - q[k] * u[k-1]

v[k] = v[k-2] - q[k] * v[k-1]
```

The remainder `r[k]` changes the same way as its coefficients `u[k]` and `v[k]`.

---

# 6. Concrete example: `a = 17`, `b = 5`

We want:

```text
gcd(17, 5)
```

and also:

```text
gcd(17, 5) = u * 17 + v * 5
```

Initial values:

```text
r[-1] = 17 = 1 * 17 + 0 * 5
u[-1] = 1
v[-1] = 0

r[0] = 5 = 0 * 17 + 1 * 5
u[0] = 0
v[0] = 1
```

---

## Step 1

Divide:

```text
17 = 3 * 5 + 2
```

So:

```text
q[1] = 3
r[1] = 2
```

Now calculate coefficients:

```text
u[1] = u[-1] - q[1] * u[0] = 1 - 3 * 0 = 1

v[1] = v[-1] - q[1] * v[0] = 0 - 3 * 1 = -3
```

So:

```text
r[1] = 1 * 17 + (-3) * 5
```

Check:

```text
17 - 15 = 2
```

Correct.

---

## Step 2

Now divide:

```text
5 = 2 * 2 + 1
```

So:

```text
q[2] = 2
r[2] = 1
```

Calculate coefficients:

```text
u[2] = u[0] - q[2] * u[1] = 0 - 2 * 1 = -2

v[2] = v[0] - q[2] * v[1] = 1 - 2 * (-3) = 1 + 6 = 7
```

So:

```text
r2 = -2 * 17 + 7 * 5
```

Check:

```text
-34 + 35 = 1
```

Correct.

And:

```text
gcd(17, 5) = 1
```

Therefore:

```text
1 = -2 * 17 + 7 * 5
```

So:

```text
u = -2
v = 7
```

---

# 7. Table version

For `a = 17`, `b = 5`:

```text
k      r[k]    q[k]    u[k]   v[k]
-----------------------------------
-1     17      -       1       0
 0      5      -       0       1
 1      2      3       1      -3
 2      1      2      -2       7
 3      0      2       -       -
```

Last non-zero remainder:

```text
r2 = 1
```

So:

```text
gcd(17, 5) = 1
```

And coefficients from the same row:

```text
u2 = -2
v2 = 7
```

Therefore:

```text
1 = -2 * 17 + 7 * 5
```

---

# 8. How the code corresponds to the math

A typical iterative implementation:

```c
int extended_gcd(int a, int b, int* u_result, int* v_result) {
    int u_old = 1, u = 0;
    int v_old = 0, v = 1;

    while (b != 0) {
        int q = a / b;
        int r = a % b;

        a = b;
        b = r;

        int u_tmp = u;
        u = u_old - q * u;
        u_old = u_tmp;

        int v_tmp = v;
        v = v_old - q * v;
        v_old = v_tmp;
    }

    *u_result = u_old;
    *v_result = v_old;

    return a;
}
```

At any moment:

```text
a is the older remainder
b is the current remainder
```

And:

```text
u_old, v_old are coefficients for a
u,     v     are coefficients for b
```

So initially:

```text
a = original a = 1 * a + 0 * b
b = original b = 0 * a + 1 * b
```

That is why:

```c
int u_old = 1, u = 0;
int v_old = 0, v = 1;
```

Again, these `0` and `1` come from:

```text
a = 1a + 0b
b = 0a + 1b
```

---

# 9. Why we update like this

Inside the loop:

```c
int q = a / b;
int r = a % b;
```

This means:

```text
a = q * b + r
```

So:

```text
r = a - q * b
```

If:

```text
a = u_old * A + v_old * B
b = u     * A + v     * B
```

where `A` and `B` are original input numbers, then:

```text
r = a - q * b
```

becomes:

```text
r =
(u_old * A + v_old * B)
-
q * (u * A + v * B)
```

So:

```text
r =
(u_old - q * u) * A
+
(v_old - q * v) * B
```

Therefore new coefficients are:

```c
new_u = u_old - q * u;
new_v = v_old - q * v;
```

Then we shift:

```text
old b becomes new a
r becomes new b
```

In code:

```c
a = b;
b = r;
```

And the same shift happens for coefficients:

```c
u_old = u;
u = new_u;

v_old = v;
v = new_v;
```

---

# 10. What happens at the end?

Loop stops when:

```c
b == 0
```

At this moment:

```text
a = gcd(original_a, original_b)
```

And the coefficients for this current `a` are:

```text
u_old, v_old
```

So:

```text
gcd(original_a, original_b) = u_old * original_a + v_old * original_b
```

That is why we return:

```c
return a;
```

and store:

```c
*u_result = u_old;
*v_result = v_old;
```

---

# 11. Short intuitive explanation

Normal Euclid says:

```text
gcd(a, b) = gcd(b, a % b)
```

Extended Euclid says:

```text
While finding the gcd, also remember how every remainder was built from the original a and b.
```

Every new remainder is:

```text
new_remainder = old_remainder_1 - q * old_remainder_2
```

So its coefficients are updated in the same way:

```text
new_u = old_u_1 - q * old_u_2
new_v = old_v_1 - q * old_v_2
```

At the end, the last non-zero remainder is the gcd, and we already know how to express it as:

```text
gcd = u * a + v * b
```

---

# 12. Why this is useful

Extended Euclid is used for:

```text
modular inverse
linear Diophantine equations
Chinese Remainder Theorem
cryptography
number theory
competitive programming
```

Example: modular inverse.

If:

```text
gcd(a, m) = 1
```

then extended Euclid gives:

```text
1 = u * a + v * m
```

Modulo `m`:

```text
1 ≡ u * a   (mod m)
```

So:

```text
u
```

is the modular inverse of `a` modulo `m`.

Example from above:

```text
1 = -2 * 17 + 7 * 5
```

Modulo `5`:

```text
-2 * 17 ≡ 1 mod 5
```

Since:

```text
-2 ≡ 3 mod 5
```

the inverse of `17 mod 5` is `3`.

Check:

```text
17 mod 5 = 2
2 * 3 = 6 ≡ 1 mod 5
```

Correct.

---

# 13. The whole algorithm in one sentence

> The extended Euclidean algorithm performs the same divisions as the normal Euclidean algorithm, but for every remainder it also tracks coefficients `u` and `v` such that `r = u*a + v*b`. The initial coefficients are `1,0` for `a` and `0,1` for `b`, because `a = 1*a + 0*b` and `b = 0*a + 1*b`. When the last non-zero remainder is reached, it is the gcd, and its coefficients give Bézout’s identity.

This is the key idea.
