# Scheduling: Introduction

Scheduling is the part of the operating system responsible for deciding **which process should run next on the CPU**.

When there are many runnable processes but only a limited number of CPUs, the OS needs a policy for choosing the next process. This policy is called a **scheduling policy** or **scheduling algorithm**.

In old batch systems, many schedulers were **non-preemptive**. This means that once a job started running, it continued until it finished. Modern operating systems usually use **preemptive scheduling**, where the OS can stop a running process and switch to another one.

This is possible because of mechanisms such as:

- timer interrupts
- context switches
- saving and restoring process state

So scheduling is a **policy**, while context switching is a **mechanism**.

---

## Scheduling Metrics

A scheduling metric is a way to measure whether a scheduler is good or bad.

Different schedulers optimize different metrics. There is no single perfect scheduler for all workloads.

---

## Turnaround Time

For now, the first important metric is **turnaround time**.

Turnaround time measures how long it takes for a job to complete after it arrives.

```text
Tturnaround = Tcompletion - Tarrival
````

Where:

* `Tarrival` is the time when the job enters the system
* `Tcompletion` is the time when the job finishes

Example:

If a job arrives at time `0` and finishes at time `10`, then:

```text
Tturnaround = 10 - 0 = 10
```

A scheduler with low average turnaround time is good for batch workloads, where we mostly care about completing jobs quickly.

---

## Response Time

Another important metric is **response time**.

Response time measures how long it takes from the moment a job arrives until it is scheduled for the first time.

```text
Tresponse = Tfirstrun - Tarrival
```

Where:

* `Tarrival` is the time when the job enters the system
* `Tfirstrun` is the first time when the job actually gets CPU time

This metric is very important for interactive systems.

For example, if a user clicks a button or types something, the system should react quickly. Even if the total completion time is not perfect, the first response should happen fast.

---

## Fairness

Another possible scheduling goal is **fairness**.

A fair scheduler tries to divide CPU time between jobs in a reasonable way.

One formal way to measure fairness is **Jain's Fairness Index**.

For `n` jobs with resource shares `x1, x2, ..., xn`, the index is:

```text
J(x) = (x1 + x2 + ... + xn)^2 / (n * (x1^2 + x2^2 + ... + xn^2))
```

The result is between:

```text
1/n and 1
```

Where:

* `1` means perfectly fair allocation
* values closer to `1/n` mean unfair allocation

Example:

If three processes receive equal CPU shares:

```text
x = [10, 10, 10]
```

Then fairness is perfect:

```text
J(x) = 1
```

If one process gets almost everything and others get very little, the fairness index becomes much smaller.

However, fairness is often in tension with performance. A scheduler that is very fair may not minimize turnaround time.

---

## Initial Scheduling Assumptions

At the beginning, we make several simplifying assumptions:

1. Each job runs for the same amount of time.
2. All jobs arrive at the same time.
3. Once started, each job runs to completion.
4. All jobs only use the CPU and perform no I/O.
5. The run time of each job is known.

These assumptions are unrealistic, but they make it easier to understand the basic scheduling policies.

Later, we relax these assumptions one by one.

---

# Scheduling Algorithms

## First In, First Out FIFO

The simplest scheduling policy is **First In, First Out**, also called **FIFO**.

The idea is simple:

```text
Run jobs in the order in which they arrive.
```

FIFO is also called **First Come, First Served FCFS**.

---

## FIFO Example: Equal Job Lengths

Assume we have three jobs:

```text
Job A: 10 seconds
Job B: 10 seconds
Job C: 10 seconds
```

All jobs arrive at the same time.

FIFO schedule:

```text
A -> B -> C
```

Completion times:

```text
A finishes at 10
B finishes at 20
C finishes at 30
```

Average turnaround time:

```text
(10 + 20 + 30) / 3 = 20
```

So in this simple case, FIFO works fine.

---

## FIFO Problem: Convoy Effect

Now remove the assumption that every job takes the same amount of time.

Assume:

```text
Job A: 100 seconds
Job B: 10 seconds
Job C: 10 seconds
```

All jobs still arrive at the same time.

FIFO schedule:

```text
A -> B -> C
```

Completion times:

```text
A finishes at 100
B finishes at 110
C finishes at 120
```

Average turnaround time:

```text
(100 + 110 + 120) / 3 = 110
```

This is bad because short jobs B and C must wait for the long job A.

This problem is called the **convoy effect**.

A convoy effect happens when short jobs get stuck behind a long job, increasing average waiting or turnaround time.

---

# Shortest Job First SJF

**Shortest Job First** runs the shortest job first.

The idea:

```text
Always run the job with the shortest total run time.
```

For the previous example:

```text
Job A: 100 seconds
Job B: 10 seconds
Job C: 10 seconds
```

SJF schedule:

```text
B -> C -> A
```

Completion times:

```text
B finishes at 10
C finishes at 20
A finishes at 120
```

Average turnaround time:

```text
(10 + 20 + 120) / 3 = 50
```

This is much better than FIFO:

```text
FIFO: 110
SJF:  50
```

Under the assumption that all jobs arrive at the same time and job lengths are known, SJF is optimal for minimizing average turnaround time.

---

## Problem with SJF: Late Arrivals

Now relax the assumption that all jobs arrive at the same time.

Assume:

```text
Job A arrives at t = 0, needs 100 seconds
Job B arrives at t = 10, needs 10 seconds
Job C arrives at t = 10, needs 10 seconds
```

With non-preemptive SJF, job A starts first because it is the only available job at time `0`.

Schedule:

```text
A -> B -> C
```

Timeline:

```text
0          100  110  120
|---- A ----|--B--|--C--|
```

Turnaround times:

```text
A: 100 - 0  = 100
B: 110 - 10 = 100
C: 120 - 10 = 110
```

Average turnaround time:

```text
(100 + 100 + 110) / 3 = 103.33
```

Even though B and C are short, they still wait behind A.

So SJF is good only when all jobs arrive at the same time or when the scheduler can preempt running jobs.

---

# Shortest Time-to-Completion First STCF

**Shortest Time-to-Completion First** is the preemptive version of SJF.

It is also called:

```text
Preemptive Shortest Job First PSJF
```

The idea:

```text
Whenever a new job arrives, choose the job with the least remaining time.
```

This means the scheduler can stop a currently running job and switch to a shorter one.

---

## STCF Example

Use the same example:

```text
Job A arrives at t = 0, needs 100 seconds
Job B arrives at t = 10, needs 10 seconds
Job C arrives at t = 10, needs 10 seconds
```

At time `0`, only A exists, so A starts running.

At time `10`, B and C arrive.

At this moment:

```text
A has 90 seconds remaining
B has 10 seconds remaining
C has 10 seconds remaining
```

STCF chooses B or C because they have less remaining time than A.

Schedule:

```text
A runs from 0 to 10
B runs from 10 to 20
C runs from 20 to 30
A resumes from 30 to 120
```

Timeline:

```text
0    10   20   30                         120
| A  | B  | C  |------------ A ------------|
```

Turnaround times:

```text
A: 120 - 0  = 120
B: 20 - 10  = 10
C: 30 - 10  = 20
```

Average turnaround time:

```text
(120 + 10 + 20) / 3 = 50
```

This is much better than non-preemptive SJF in this case.

---

## Why STCF Is Good

STCF is excellent for minimizing average turnaround time.

It works well because short jobs do not wait behind long jobs. If a short job arrives while a long job is running, the long job can be paused.

However, STCF needs information that is usually unavailable:

```text
How much time does each job still need?
```

In real systems, the OS usually does not know the future.

---

# Response Time Problem

SJF and STCF are good for turnaround time, but they can be bad for response time.

Assume three jobs arrive at the same time:

```text
Job A: 5 seconds
Job B: 5 seconds
Job C: 5 seconds
```

SJF schedule:

```text
A -> B -> C
```

Response times:

```text
A: 0
B: 5
C: 10
```

Average response time:

```text
(0 + 5 + 10) / 3 = 5
```

The third job waits 10 seconds before running for the first time.

This is bad for interactive systems.

---

# Round Robin RR

**Round Robin** is designed to improve response time.

The idea:

```text
Run each job for a small time slice, then switch to the next job.
```

The time slice is also called a **scheduling quantum**.

Example:

```text
Time slice = 1 second
```

With jobs A, B, and C, the schedule may look like:

```text
A -> B -> C -> A -> B -> C -> A -> B -> C ...
```

---

## RR Example

Assume:

```text
Job A: 5 seconds
Job B: 5 seconds
Job C: 5 seconds
```

All arrive at the same time.

Round Robin with time slice `1`:

```text
0 1 2 3 4 5 ...
A B C A B C ...
```

Response times:

```text
A: 0
B: 1
C: 2
```

Average response time:

```text
(0 + 1 + 2) / 3 = 1
```

This is much better than SJF for response time.

---

## Time Slice Trade-off

The length of the time slice is very important.

If the time slice is short:

```text
Response time improves.
```

But if the time slice is too short:

```text
Context switching overhead becomes too large.
```

A context switch is not free. When the OS switches from one process to another, it may disturb CPU state such as:

* CPU registers
* CPU caches
* TLB entries
* branch predictor state
* other microarchitectural state

Programs build up useful state while running. When the scheduler switches to another process, some of this state becomes less useful or must be replaced.

Therefore, very small time slices can reduce performance.

---

## Turnaround vs Response Time Trade-off

There is an important trade-off:

```text
SJF/STCF optimize turnaround time.
RR optimizes response time.
```

But:

```text
SJF/STCF can be bad for interactivity.
RR can be worse for turnaround time.
```

This is a common pattern in operating systems.

A scheduler often cannot optimize every metric at the same time.

---

# Incorporating I/O

So far, we assumed that jobs only use the CPU.

Now we relax this assumption.

Real programs often alternate between:

```text
CPU bursts
I/O operations
```

For example, a process may:

1. compute for a while
2. read from disk
3. wait for disk I/O
4. compute again
5. write data to a file
6. wait again

When a process waits for I/O, it cannot use the CPU.

So the scheduler should run another process during that time.

---

## CPU and I/O Overlap

Assume process A does many short CPU bursts followed by I/O.

Assume process B is CPU-bound and just wants to compute.

Bad schedule:

```text
Run A and wait during its I/O
Then run B
```

This wastes CPU time while A is waiting for I/O.

Better schedule:

```text
Run A until it starts I/O
While A waits for I/O, run B
When A's I/O completes, run A again
```

This creates overlap:

```text
A uses the disk while B uses the CPU.
```

That improves overall resource utilization.

---

## Treating CPU Bursts as Jobs

One useful idea is to treat each CPU burst as a separate job.

For example, if process A performs:

```text
10 ms CPU
I/O
10 ms CPU
I/O
10 ms CPU
I/O
```

The scheduler can treat each `10 ms` CPU burst as a short job.

This helps interactive or I/O-heavy jobs run frequently, while CPU-bound jobs use the CPU when interactive jobs are waiting for I/O.

---

# The Biggest Problem: Unknown Job Length

The worst assumption we made is:

```text
The run time of each job is known.
```

In real general-purpose operating systems, this is usually false.

The OS does not know:

* how long a process will run
* when it will block for I/O
* how much CPU time it will need in the future
* whether it is interactive or CPU-bound

This makes SJF and STCF hard to implement directly.

So the real question becomes:

```text
How can the OS behave like SJF/STCF without knowing the future?
```

One common idea is:

```text
Use the recent past to predict the near future.
```

This idea leads to more advanced schedulers, especially the **Multi-Level Feedback Queue MLFQ**.

---

# Summary

Scheduling decides which process should run next on the CPU.

The main metrics are:

```text
Turnaround time = completion time - arrival time
Response time   = first run time - arrival time
Fairness        = how equally resources are shared
```

Basic scheduling policies:

```text
FIFO  - simple, but can suffer from convoy effect
SJF   - runs shortest jobs first, good for turnaround time
STCF  - preemptive SJF, even better for turnaround time
RR    - cycles through jobs, good for response time
```

Important trade-offs:

```text
Low turnaround time vs good response time
Fairness vs performance
Short time slices vs context-switch overhead
CPU-bound jobs vs I/O-bound jobs
```

The biggest challenge is that real operating systems usually do not know job lengths in advance.

This motivates more advanced schedulers that try to learn from past behavior.

