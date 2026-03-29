## 02. The Abstraction: The Process

### Process
A **process** is a **running program**.  
A program stored on disk is just passive data; it becomes a process when the OS loads it into memory and starts executing it.

The OS gives the illusion that many programs run at the same time, even if there is only one CPU. This is done through **virtualization**.
By running one process for a short time, then stopping it and running another, the OS creates the illusion of many virtual CPUs. This technique is called **time sharing**.

### Time sharing vs space sharing
- **Time sharing** — many entities use the same resource, but each gets it only for a short period of time  
  Example: CPU
- **Space sharing** — the resource is divided into parts, and each part is assigned to one user/entity  
  Example: disk blocks, memory regions

If many processes share one CPU, each process may run more slowly because the CPU has to be shared.

---

### What is part of a process?
A process consists of more than just the program code.

#### 1. Address space
The **address space** is the memory visible to the process.  
It includes:
- program code (instructions)
- static/global data
- heap
- stack

Yes — in practice this refers to the process view of memory, which is mapped by the OS to physical RAM (and sometimes other storage mechanisms, e.g. paging).

#### 2. Registers
**Registers** are small, very fast storage locations inside the CPU.  
They hold important temporary values while the program executes.

Examples:
- current arithmetic values
- memory addresses
- stack position
- current instruction location

When the OS switches from one process to another, it must save and restore the register state.

#### 3. Program Counter (PC)
The **program counter** is a special register that stores the address of the next instruction to execute. It tells the CPU where the program currently is.

#### 4. Stack Pointer / Frame Pointer
These registers help manage the **stack**.

The stack is used for:
- function parameters
- local variables
- return addresses

The **stack pointer** points to the current top of the stack.  
The **frame pointer** helps organize one function call frame.

---

### Process API
Typical operations the OS provides for processes:

- **create** — create a new process
- **destroy** — terminate a process
- **wait** — wait until a process finishes
- **miscellaneous control** — other actions such as pausing, resuming, sending signals, changing priority, etc.
- **status** — get information about a process, e.g. whether it is running, exited, blocked, and what exit code it returned

---

### How is a process created?
How does a program become a process?

#### 1. Load code and static data
The OS loads the program code and static data from disk into the process address space. In modern systems this is often done **lazily**, meaning some parts are loaded only when they are actually needed.

#### 2. Create the stack
The OS allocates a **runtime stack** for the process.

The stack is used for:
- local variables
- function arguments
- return addresses

#### 3. Create the heap
The OS sets up the **heap**, which is used for dynamic allocation such as:
- `malloc()`, `calloc()`, `free()`
- `new`, `delete`

#### 4. Perform initialization
The OS performs some initialization work, for example setting up standard file descriptors:
- `0` — standard input
- `1` — standard output
- `2` — standard error

#### 5. Start execution
The OS transfers control to the program’s entry point.

Strictly speaking, execution starts at a lower-level entry point set by the runtime system, and eventually reaches `main()`.

---

### Stack: process or thread?
Good question.

A process has an address space that contains memory regions such as code, heap, and stack-related areas.  
However, in a **multithreaded process**, **each thread has its own stack**.

Why?
Because each thread has its own:
- function calls
- local variables
- return addresses
- execution path

But threads in the same process usually **share**:
- code
- heap
- global variables
- open files

So:
- **address space** belongs to the process
- **individual stack** belongs to each thread

---

### Process states
Typical process states:

- **running** — currently executing on the CPU
- **ready** — ready to run, waiting for CPU
- **blocked** — waiting for some event, usually I/O

Additional states may also exist:

- **new / initial** — process is being created
- **terminated** — process has finished
- **zombie** — process has exited, but the parent has not yet collected its exit status

#### Zombie process
A **zombie** is a finished process that still has an entry in the process table because its parent has not yet called `wait()`.

This allows the parent to read:
- the child’s exit status
- whether it finished successfully

After the parent calls `wait()`, the OS can fully clean up the process.

---

### Scheduler
The **OS scheduler** is responsible for deciding which process runs next.

It manages transitions between states, for example:
- `ready -> running`
- `running -> blocked`
- `blocked -> ready`
- `running -> ready`

---

### Process data structures
The OS must keep data structures describing processes.

Usually it stores:
- a **process list / process table**
- the current state of each process
- register values
- process ID
- scheduling information
- memory information
- file descriptor information
- queues for ready and blocked processes

The OS also needs to know which process is currently running.
