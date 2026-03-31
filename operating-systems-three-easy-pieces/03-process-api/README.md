## Process API

### `fork()`
`fork()` is used to create a new process.

After calling `fork()`, there are two processes:
- **parent**
- **child**

The child is almost an exact copy of the parent, but it has its own:
- address space
- registers
- program counter
- stack


```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("hello world from process(%d)\n", getpid());

    // fork() creates a new process
    int rc = fork();

    if (rc < 0) {
        perror("creating new process failed");
        exit(1);
    } else if (rc == 0) {
        // rc == 0 means: this code is running in the child
        printf("hello from child process(%d)\n", getpid());
    } else {
        // rc > 0 means: this code is running in the parent
        // rc is the PID of the child
        printf("hello from parent process(%d). I'm parent of process(%d)\n", getpid(), rc);
    }

    return 0;
}
````

### Important

The output is **not deterministic**.
After `fork()`, parent and child run independently, and the scheduler decides which one prints first.

### What does `fork()` return?

* `< 0` → error
* `0` → child process
* `> 0` → parent process, and the value is the child's PID

### Why compare with `0` if PID is some positive number?

Because the return value of `fork()` is **different in parent and child**:

* in the **child**, `fork()` returns `0`
* in the **parent**, `fork()` returns the child's PID

So `0` does **not** mean the child's real PID.
It is just a special return value that tells the process: *you are the child*.

### `exit()` vs `_exit()`

Both terminate the process, but there is a difference:

* `exit()`

  * performs normal C library cleanup
  * flushes buffered output
  * runs `atexit()` handlers

* `_exit()`

  * terminates immediately
  * does not flush stdio buffers
  * does not run `atexit()` handlers

After `fork()`, `_exit()` is often safer in the child, especially if the child will not continue normal program flow.

---

### `wait()`

`wait()` is used by the parent to wait until one of its child processes finishes.

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    printf("hello world from process(%d)\n", getpid());

    int rc = fork();

    if (rc < 0) {
        perror("creating new process failed");
        _exit(1);
    } else if (rc == 0) {
        printf("hello from child process(%d)\n", getpid());
    } else {
        int wc = wait(NULL);
        printf("hello from parent process(%d). I'm parent of process(%d), wc:%d\n",
               getpid(), rc, wc);
    }

    return 0;
}
```

Now the output becomes more predictable:

* the parent waits
* the child finishes first
* only then the parent continues

### What parameter does `wait()` take?

```c
wait(int *status)
```

It takes a pointer to an integer where the child's exit status can be stored.

### Why `NULL`?

If we write:

```c
wait(NULL);
```

it means:

* wait for a child to finish
* but do **not care** about its exit status

If you want the status:

```c
int status;
wait(&status);
```

### What does `wait()` return?

It returns the PID of the finished child process.

---

### `waitpid()`

`waitpid()` is a more flexible version of `wait()`.

Prototype:

```c
pid_t waitpid(pid_t pid, int *status, int options);
```

### Parameters

* `pid`

  * `> 0` → wait for a specific child
  * `-1` → wait for any child (similar to `wait()`)
* `status`

  * pointer to exit status
  * `NULL` if not needed
* `options`

  * usually `0`
  * can be used for special behavior, e.g. non-blocking wait

Example:

```c
int status;
waitpid(rc, &status, 0);
```

This means:

* wait for child with PID `rc`
* save its exit status into `status`
* block until it finishes

---
Jasne — poniżej masz **oczyszczoną, spójną i poprawioną** wersję do README, w tym też sekcję o przekierowaniu `stdout`, krótką wzmiankę o `pipe()` i `kill()`.

---

### `exec()`

`exec()` is used to run a **different program inside the current process**.

Important:

* `exec()` does **not** create a new process
* it **replaces** the current process image with a new program

A common pattern is:

* `fork()` creates a child process
* `exec()` is called in the child
* the child stops running the old code and starts running a new program

---

#### Main idea

Before `exec()`:

* the current process is running program A

After `exec()`:

* the **same process** is now running program B

So `exec()` does not create an additional process.
It transforms the already existing one.

If `exec()` succeeds:

* the old code is gone
* execution does **not** continue after the `exec()` call

If `exec()` returns, it usually means an error occurred.

---

#### What changes after `exec()`

When `exec()` succeeds, the OS:

* loads code and static data from the new executable
* replaces the old code and old static data
* reinitializes the stack and heap
* starts execution of the new program from its entry point
* passes the provided arguments as `argv[]`

So from the program’s point of view, it is almost as if the previous process image never existed.

---

#### Why `fork()` + `exec()` are often used together

This is the standard UNIX pattern:

* `fork()` creates a child process
* the child calls `exec()` to run a different program
* the parent can continue its work or wait for the child

This pattern is especially useful in shells.

Example idea:

* parent process = shell
* child process = command to run
* child calls `exec()` and becomes e.g. `ls`, `wc`, `gcc`, etc.

---

### Variants of `exec()`

There are several versions:

* `execl()`
* `execv()`
* `execlp()`
* `execvp()`
* `execle()`
* `execve()`

The main differences are:

1. **how arguments are passed**
2. **whether the program is searched in `PATH`**
3. **whether a custom environment is provided**

---

#### `l` vs `v`

* `l` = **list**

  * arguments are passed one by one

* `v` = **vector**

  * arguments are passed as an array of pointers

Example:

```c
execl("/bin/ls", "ls", "-l", NULL);
```

vs

```c
char *args[] = {"ls", "-l", NULL};
execv("/bin/ls", args);
```

---

#### `p`

* `p` = search the program in `PATH`

Example:

```c
execlp("ls", "ls", "-l", NULL);
```

You do not need to provide the full path like `/bin/ls`.

---

#### `e`

* `e` = provide a custom environment

This is useful when you want the new program to run with your own environment variables instead of inheriting the current ones.

---

### Example: `fork()` + `execvp()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("hello from process (%d)\n", getpid());

    int rc = fork();

    if (rc < 0) {
        perror("fork");
        exit(1);
    } else if (rc == 0) {
        printf("hello, I'm child process (%d)\n", getpid());

        char *myargs[] = {"wc", "code3-3.c", NULL};
        execvp(myargs[0], myargs);

        // execvp() returns only if an error occurred
        perror("execvp");
        exit(1);
    } else {
        printf("it's process (%d): parent of process (%d)\n", getpid(), rc);
        wait(NULL);
    }

    return 0;
}
```

---

#### How this example works

1. The program starts as one process.
2. `fork()` creates a child process.
3. Both parent and child continue from the line after `fork()`.
4. In the child:

   * `execvp("wc", myargs)` is called
   * the child process is replaced by the `wc` program
   * `wc` runs with argument `"code3-3.c"`
5. The parent waits for the child to finish using `wait(NULL)`.

So effectively, the child becomes equivalent to running:

```bash
wc code3-3.c
```

---

#### Important note about arguments

In:

```c
char *myargs[] = {"wc", "code3-3.c", NULL};
```

* `myargs[0]` is the program name
* `myargs[1]` is the first argument
* `myargs[2] = NULL` marks the end of the argument array

That final `NULL` is required.

---

### Memory note

For:

```c
char *myargs[] = {"wc", "code3-3.c", NULL};
```

* the array `myargs` itself is a local variable, so it is typically stored on the **stack**
* the string literals `"wc"` and `"code3-3.c"` are stored in static read-only memory
* the **heap is not used** here

---

### Example: redirecting `stdout` to a file

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    printf("parent process(%d) started\n", getpid());

    int rc = fork();

    if (rc < 0) {
        perror("fork");
        exit(1);
    } else if (rc > 0) {
        printf("this process(%d) is parent of process(%d)\n", getpid(), rc);
        wait(NULL);
    } else {
        printf("it's child process(%d)\n", getpid());

        // close standard output (fd = 1)
        close(STDOUT_FILENO);

        // open file; because fd 1 is free now, this file will usually become stdout
        int out_file = open("out.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (out_file < 0) {
            perror("opening/creating file");
            _exit(1);
        }

        char *myargs[] = {"wc", "code3-4.c", NULL};

        // output of wc will be written to out.txt instead of the terminal
        execvp(myargs[0], myargs);

        perror("execvp");
        _exit(1);
    }

    return 0;
}
```

---

#### How redirection works here

This part is the key:

```c
close(STDOUT_FILENO);
open("out.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
```

Explanation:

* `STDOUT_FILENO` is file descriptor `1`
* `close(STDOUT_FILENO)` closes standard output
* `open()` returns the **lowest available file descriptor**
* because descriptor `1` is now free, the new file usually gets descriptor `1`

So after that, `stdout` points to `out.txt` instead of the terminal.

Then:

```c
execvp(myargs[0], myargs);
```

runs `wc`, and `wc` writes its normal output to `stdout`.

But now `stdout` is the file `out.txt`, so the result is saved there.

This is basically the low-level version of:

```bash
wc code3-4.c > out.txt
```

---

#### Note about file permissions

In:

```c
open("out.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
```

`0644` is written in **octal notation**, which is the standard way to specify UNIX file permissions.

Common permissions:

* `0644` → owner can read/write, group and others can read
* `0744` → owner can read/write/execute, group and others can only read

Important:

* if the file already exists, `open(..., O_TRUNC, ...)` truncates it
* but it does **not** change existing permissions
* permissions passed in `open()` matter only when a **new file is created**

---

### `pipe()`

UNIX pipes are implemented with the `pipe()` system call.

The idea:

* output of one process becomes input of another process
* the kernel provides a pipe buffer between them
* this allows commands to be chained together

Example:

```bash
grep -o foo file.txt | wc -l
```

What happens here:

* `grep -o foo file.txt` prints every match
* `wc -l` counts how many lines it receives
* the output of `grep` is connected to the input of `wc` through a pipe

So pipes are another example of how `fork()`, `exec()`, and file descriptors are used together.

---

## Other parts of Process API

### `kill()`

`kill()` is used to send signals to a process.

Despite the name, it is not only for killing processes.
It can be used to send many kinds of signals, for example:

* `SIGTERM` → ask a process to terminate
* `SIGKILL` → force termination
* `SIGSTOP` → stop a process
* `SIGCONT` → continue a stopped process

Example:

```c
kill(pid, SIGTERM);
```

This sends `SIGTERM` to the process with id `pid`.

---

### Signals

Signals are a lightweight mechanism for **inter-process communication and process control**.

They are used to notify a process that:

* something happened
* it should terminate
* it should stop
* it should continue
* a child process changed state

Signals do **not** carry normal data like pipes or sockets.
They are mostly used for **notifications and control**.
