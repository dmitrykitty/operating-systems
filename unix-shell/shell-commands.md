# UNIX Shell Basics — Deep Notes

## 1. What is a shell?

A shell is a program that reads commands and starts other programs.

Examples:

```bash
bash
zsh
sh
fish
````

When you write:

```bash
ls -la
```

the shell:

```text
1. Parses the command line.
2. Expands variables, globs, quotes, etc.
3. Looks for the executable.
4. Creates a child process.
5. Runs the program.
6. Waits for it to finish.
7. Stores its exit status in `$?`.
```

So the shell is not only “a terminal”. The terminal is just UI. The shell is the command interpreter.

---

# 2. Package management: `apt`

On Ubuntu/Debian-based systems:

```bash
sudo apt update
sudo apt upgrade
sudo apt install package-name
```

## `apt update`

```bash
sudo apt update
```

This does not upgrade programs.

It downloads fresh package metadata:

```text
What packages exist?
What versions are available?
Where can they be downloaded from?
```

## `apt upgrade`

```bash
sudo apt upgrade
```

This upgrades already installed packages to newer available versions.

## `apt install`

```bash
sudo apt install gcc
sudo apt install gdb
sudo apt install valgrind
```

This installs packages.

## Searching packages

```bash
apt search --names-only 'regex'
```

Example:

```bash
apt search --names-only '^gcc'
```

This searches package names matching a regular expression.

---

# 3. `cd dev` vs `cd /dev`

These are very different:

```bash
cd dev
```

means:

```text
Go to directory named `dev` inside current directory.
```

If current directory is:

```bash
/home/dima
```

then:

```bash
cd dev
```

means:

```bash
cd /home/dima/dev
```

But:

```bash
cd /dev
```

starts with `/`, so it is an absolute path.

It means:

```text
Go to the `dev` directory directly under filesystem root.
```

So:

```bash
/dev
```

is a system directory containing device files:

```text
/dev/null
/dev/zero
/dev/sda
/dev/tty
```

Important rule:

```text
path starting with `/`     -> absolute path
path not starting with `/` -> relative path
```

---

# 4. Standard streams: stdin, stdout, stderr

Every Unix process usually starts with three standard file descriptors:

```text
0 -> stdin   -> standard input
1 -> stdout  -> standard output
2 -> stderr  -> standard error
```

Example:

```bash
./program
```

By default:

```text
stdin  comes from keyboard
stdout goes to terminal
stderr goes to terminal
```

---

# 5. Input/output redirection

Example:

```bash
./program < text.in 1> text.out 2> text.err
```

Meaning:

```text
< text.in     -> connect stdin  to text.in
1> text.out   -> connect stdout to text.out
2> text.err   -> connect stderr to text.err
```

The `1` can be omitted:

```bash
./program < text.in > text.out 2> text.err
```

This is the same.

---

# 6. Redirecting stdout and stderr to the same file

Correct syntax:

```bash
./program < text.in > text.out 2>&1
```

Meaning:

```text
> text.out  -> redirect stdout to text.out
2>&1        -> redirect stderr to wherever stdout currently goes
```

So both stdout and stderr go to `text.out`.

Order matters.

This:

```bash
./program 2>&1 > text.out
```

is different.

Step by step:

```text
2>&1        -> stderr goes to current stdout, usually terminal
> text.out  -> stdout goes to file
```

Result:

```text
stdout -> text.out
stderr -> terminal
```

So the usual correct version is:

```bash
./program > text.out 2>&1
```

Bash also supports shorter syntax:

```bash
./program &> text.out
```

or:

```bash
./program >& text.out
```

Both redirect stdout and stderr to the same file in Bash.

But the most portable/common explicit version is:

```bash
./program > text.out 2>&1
```

---

# 7. `echo`

`echo` prints text.

```bash
echo "Hello"
```

Print environment/shell variables:

```bash
echo "$HOME"
```

`$HOME` contains path to your home directory:

```text
/home/dima
```

Print exit status of last command:

```bash
echo "$?"
```

`$?` stores the exit code of the last foreground command.

Example:

```bash
true
echo "$?"
```

Output:

```text
0
```

Example:

```bash
false
echo "$?"
```

Output:

```text
1
```

In Unix:

```text
0     -> success
non-0 -> error/failure
```

This is opposite to C booleans, where `0` usually means false.

---

# 8. Exit codes and signals

Normal program exit:

```c
return 0;
```

means success.

```c
return 1;
```

means failure.

If a program is killed by a signal, shell usually reports status:

```text
128 + signal_number
```

For example, `SIGABRT` is usually signal number `6`.

So if a program aborts:

```text
exit status = 128 + 6 = 134
```

That is why after a crash you can often see:

```bash
echo "$?"
```

printing:

```text
134
```

---

# 9. Why do we sometimes still see `Aborted`?

Suppose your program crashes with `abort()`.

You run:

```bash
./program < text.in > text.out 2> text.err
```

You may still see something like:

```text
Aborted (core dumped)
```

Why?

Because this message is often printed by the **shell**, not by your program.

Your program terminates by signal. The shell notices this and prints a diagnostic message to its own stderr.

Redirection of the program's stderr does not always redirect diagnostics printed later by the parent shell.

That is why this topic is subtle.

---

# 10. Grouping commands with `( ... )`

This:

```bash
( ./program < text.in ) >& text.out
```

runs the command inside a **subshell**.

A subshell is another shell process.

The redirection:

```bash
>& text.out
```

applies to the whole subshell.

So stdout and stderr of commands inside the subshell are redirected.

However, if the subshell itself terminates because its child was killed by a signal, the parent shell may still print:

```text
Aborted
```

outside that redirection.

A common trick is to force the subshell to exit normally with a controlled status.

---

# 11. `&&` and `||`

```bash
cmd1 && cmd2
```

means:

```text
Run cmd2 only if cmd1 succeeded.
```

Success means exit code `0`.

Example:

```bash
echo "Hello" && echo "world"
```

Output:

```text
Hello
world
```

Because `echo "Hello"` returns `0`.

---

```bash
cmd1 || cmd2
```

means:

```text
Run cmd2 only if cmd1 failed.
```

Example:

```bash
echo "Hello" || echo "world"
```

Output:

```text
Hello
```

Because `echo "Hello"` succeeded, so `echo "world"` is not executed.

---

# 12. `true` and `false`

In Bash:

```bash
true
```

returns:

```text
0
```

and:

```bash
false
```

returns:

```text
1
```

So:

```bash
true && echo "yes"
```

prints:

```text
yes
```

But:

```bash
false && echo "yes"
```

prints nothing.

And:

```bash
false || echo "failed"
```

prints:

```text
failed
```

---

# 13. Why this normalizes crash status to 1

Example:

```bash
( ./program < text.in || false ) >& text.out
echo "$?"
```

If `./program` succeeds:

```text
./program returns 0
false is not executed
group returns 0
```

If `./program` fails or crashes:

```text
./program returns non-zero, for example 134
`false` executes
`false` returns 1
group returns 1
```

So after this:

```bash
echo "$?"
```

you get:

```text
1
```

The purpose is:

```text
Convert any failure/crash into a clean exit code 1.
```

Also, because the subshell continues after the crash and runs `false`, the subshell itself exits normally with status `1`, not by signal. That can prevent the outer shell from printing `Aborted`.

This is useful in testing scripts when you want clean machine-readable failure.

---

# 14. `bash -c`

Correct form:

```bash
bash -c "command to run"
```

Example:

```bash
bash -c "./program < text.in" >& text.out
```

This starts a new Bash process and asks it to execute the command string.

Meaning:

```text
1. Outer shell starts bash.
2. New bash parses and runs "./program < text.in".
3. Redirection >& text.out applies to the new bash process.
4. Diagnostics printed by that inner bash can be captured.
```

Your note had:

```bash
bin/bash -c -c "command"
```

That is probably a typo.

Usually it should be:

```bash
/bin/bash -c "command"
```

The path `/bin/bash` is the Bash executable.

---

# 15. Test automation with shell

Suppose we have:

```text
program
tests/test1.in
tests/test2.in
tests/test3.in
```

We can iterate over all input files:

```bash
for i in tests/*.in; do
    echo "$i"
done
```

Here:

```bash
tests/*.in
```

is expanded by the shell before the loop starts.

If files are:

```text
tests/a.in
tests/b.in
tests/c.in
```

then Bash turns this into:

```bash
for i in tests/a.in tests/b.in tests/c.in; do ...
```

Important:

```text
The shell expands `*`, not the program.
```

---

# 16. Variable usage in Bash

In Bash assignment:

```bash
i=5
```

No `$`.

To read value:

```bash
echo "$i"
```

So:

```text
i       -> variable name / lvalue
$i      -> variable value / rvalue
```

Example:

```bash
i=10
echo "$i"
```

Output:

```text
10
```

---

# 17. Running tests manually

Example:

```bash
for i in tests/*.in; do
    echo "$i"
    cat "$i"
    ./program < "$i"
done
```

Better version with output files:

```bash
mkdir -p out

for input in tests/*.in; do
    name=$(basename "$input" .in)

    echo "Running $input"

    ./program < "$input" > "out/$name.out" 2> "out/$name.err"

    status=$?

    echo "Exit status: $status"
done
```

Why quote variables?

```bash
"$input"
```

Because file names can contain spaces.

Bad:

```bash
cat $input
```

Good:

```bash
cat "$input"
```

---

# 18. `$PATH`

Your note:

> `$PATH` - path where files are saved.

More precise:

```text
$PATH is a list of directories where the shell searches for executable commands.
```

Example:

```bash
echo "$PATH"
```

Possible output:

```text
/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin
```

Directories are separated by `:`.

When you write:

```bash
ls
```

the shell searches:

```text
/usr/local/bin/ls
/usr/bin/ls
/bin/ls
...
```

When it finds executable `ls`, it runs it.

But when you write:

```bash
./program
```

there is a slash `/` in the command, so `$PATH` is not used.

The shell directly tries to execute:

```text
program in current directory
```

That is why we often run our own compiled program as:

```bash
./program
```

not just:

```bash
program
```

because current directory is usually not in `$PATH`.

---

# 19. Creating many test files

Your command:

```bash
mkdir -p "$folder" && cd "$folder" && for i in $(seq 1 1 10); do echo "$i"; echo "$i" > "file${i}.in"; done
```

Meaning:

```text
1. mkdir -p "$folder" creates folder if it does not exist.
2. && cd "$folder" runs only if mkdir succeeded.
3. Loop from 1 to 10.
4. Create files file1.in, file2.in, ...
```

`mkdir -p` means:

```text
Create parent directories if needed.
Do not fail if directory already exists.
```

Better multi-line version:

```bash
mkdir -p "$folder" && cd "$folder" || exit 1

for i in $(seq 1 1 10); do
    echo "$i"
    echo "$i" > "file${i}.in"
done
```

Even better for integer loops in Bash:

```bash
mkdir -p "$folder" && cd "$folder" || exit 1

for ((i = 1; i <= 10; ++i)); do
    echo "$i"
    echo "$i" > "file${i}.in"
done
```

---

# 20. What is an executable file in Unix/Linux?

In Unix/Linux, a file is executable if it has execute permission:

```bash
ls -l script.sh
```

Example:

```text
-rwxr-xr-x
```

The `x` bits mean executable.

You can add execute permission:

```bash
chmod +x script.sh
```

But execute permission alone is not enough. The kernel also needs to understand how to run the file.

Common executable types:

```text
1. ELF binary        -> compiled C/C++ program
2. Script with shebang -> Bash/Python/etc.
```

Example ELF:

```bash
gcc main.c -o program
./program
```

Example script:

```bash
#!/bin/bash
echo "Hello"
```

Run:

```bash
chmod +x script.sh
./script.sh
```

---

# 21. Shebang

First line:

```bash
#!/bin/bash
```

is called shebang.

It tells the kernel:

```text
Run this file using /bin/bash.
```

When you execute:

```bash
./script.sh
```

the kernel sees:

```bash
#!/bin/bash
```

and effectively runs:

```bash
/bin/bash ./script.sh
```

But technically this is done by the kernel during `execve`.

For Python, better:

```bash
#!/usr/bin/env python3
```

Why?

Because this searches `python3` in `$PATH`.

So it works even if Python is installed in different locations:

```text
/usr/bin/python3
/usr/local/bin/python3
custom virtual environment
```

---

# 22. `./script.sh` vs `bash script.sh`

These are similar but not identical.

## `./script.sh`

```bash
./script.sh
```

requires:

```text
1. execute permission
2. valid executable format, usually shebang
```

The kernel runs the interpreter from shebang.

## `bash script.sh`

```bash
bash script.sh
```

does not require execute permission on `script.sh`.

Why?

Because you are executing `bash`, and `bash` opens `script.sh` as an input file.

So for:

```bash
bash script.sh
```

the script mostly needs read permission, not execute permission.

---

# 23. Why variables disappear inside scripts

Suppose in your current shell:

```bash
folder=my_tests
border=10
./script.sh
```

Inside `script.sh`, variables may be empty.

Why?

Because shell variables are not automatically passed to child processes.

When you run:

```bash
./script.sh
```

the current shell creates a child process.

The child gets a copy of the **environment**, not all shell variables.

To pass variables, use `export`:

```bash
export folder=my_tests
export border=10
./script.sh
```

Now child process can see them.

---

# 24. Process model: fork/exec/environment

At OS level, running an external command usually means:

```text
1. shell calls fork()
2. child process is created
3. child process calls execve()
4. execve replaces child memory with new program
5. environment variables are passed to new program
6. parent shell waits
```

Important:

```text
Child process cannot modify parent shell variables.
```

The child receives a copy.

So if script does:

```bash
folder=abc
```

the parent shell does not see this change after script exits.

---

# 25. `source script.sh`

If you want to run a script in the current shell:

```bash
source script.sh
```

or:

```bash
. script.sh
```

Then no separate shell process is created for the script logic.

Variables set inside the script remain visible:

```bash
source script.sh
echo "$folder"
```

This is why activation scripts use `source`:

```bash
source .venv/bin/activate
```

They need to modify current shell environment.

---

# 26. Passing variables with `env`

You can pass variables only for one command:

```bash
env folder=myfolder border=10 ./script.sh
```

Inside `script.sh`:

```bash
echo "$folder"
echo "$border"
```

works.

But after the command finishes, your parent shell does not keep those variables.

---

# 27. Your file-generation script

Your script:

```bash
#!/bin/bash

mkdir -p $folder 

for i in $(seq 1 1 $boarder)
do
    echo "processing $i"
    echo $i > ${folder}/file${i}.in
done
```

Improved:

```bash
#!/bin/bash

mkdir -p "$folder"

for i in $(seq 1 1 "$border")
do
    echo "processing $i"
    echo "$i" > "${folder}/file${i}.in"
done
```

Important fixes:

```text
1. Quote variables: "$folder", "$i"
2. Use consistent spelling: border, not boarder
```

Better Bash loop:

```bash
#!/bin/bash

mkdir -p "$folder"

for ((i = 1; i <= border; ++i)); do
    echo "processing $i"
    echo "$i" > "${folder}/file${i}.in"
done
```

---

# 28. Positional arguments

Inside a script:

```bash
$0 -> script name
$1 -> first argument
$2 -> second argument
...
$# -> number of arguments
$@ -> all arguments as separate words
$* -> all arguments as one string-like expansion
```

Example:

```bash
#!/bin/bash

echo "param 0=$0"
echo "param 1=$1"
```

Run:

```bash
./scr2.sh param1
```

Output:

```text
param 0=./scr2.sh
param 1=param1
```

---

# 29. `getopts`

Example:

```bash
#!/bin/bash

while getopts "b:f:" flag
do
    case "${flag}" in
        b) border="${OPTARG}" ;;
        f) folder="${OPTARG}" ;;
        \?) echo "invalid argument" ;;
    esac
done

echo "folder is $folder"
echo "border is $border"
```

Run:

```bash
./script.sh -b 10 -f tests
```

Output:

```text
folder is tests
border is 10
```

## What does `"b:f:"` mean?

```text
b: -> option -b requires an argument
f: -> option -f requires an argument
```

So:

```bash
-b 10
```

sets:

```bash
flag=b
OPTARG=10
```

And:

```bash
-f tests
```

sets:

```bash
flag=f
OPTARG=tests
```

## Where does `OPTARG` come from?

`OPTARG` is a special variable set by `getopts`.

When `getopts` parses an option that requires an argument, it stores that argument in `OPTARG`.

There is also `OPTIND`, which stores index of the next argument to process.

---

# 30. `[` vs `[[` vs `(( ))`

## `[ ... ]`

This is the old POSIX test command.

Example:

```bash
[ "$i" -le "$j" ]
```

It means:

```text
test whether i <= j numerically
```

Numeric operators:

```text
-eq -> equal
-ne -> not equal
-lt -> less than
-le -> less than or equal
-gt -> greater than
-ge -> greater than or equal
```

Important: spaces are required.

Correct:

```bash
[ "$i" -le "$j" ]
```

Wrong:

```bash
["$i" -le "$j"]
```

Because `[` is actually a command name.

This:

```bash
[ "$i" -le "$j" ]
```

is similar to:

```bash
test "$i" -le "$j"
```

---

## `[[ ... ]]`

This is a Bash keyword, more powerful and safer than `[ ... ]`.

Example:

```bash
[[ "$name" == *.txt ]]
```

Inside `[[ ]]`, pattern matching works better, and quoting is less dangerous.

Example:

```bash
if [[ "$file" == *.in ]]; then
    echo "input file"
fi
```

Use `[[ ]]` for string tests in Bash.

---

## `(( ... ))`

This is arithmetic evaluation.

Example:

```bash
while (( i <= j ))
do
    echo "$i"
    ((i++))
done
```

Inside `(( ))`, you can write C-like arithmetic:

```bash
((i = i + 1))
((i++))
((i <= j))
((x = y * 2 + 1))
```

You do not need `$` inside:

```bash
(( i <= j ))
```

not:

```bash
(( $i <= $j ))
```

Although the second often works, the first is idiomatic.

Exit status:

```text
(( expression )) returns 0 if expression is non-zero.
(( expression )) returns 1 if expression is zero.
```

Example:

```bash
(( 5 > 3 ))
echo "$?"
```

prints:

```text
0
```

because expression is true.

Example:

```bash
(( 0 ))
echo "$?"
```

prints:

```text
1
```

because expression value is zero.

---

# 31. `$(( ... ))` vs `(( ... ))`

These are different.

## Arithmetic expansion

```bash
x=$((i + 1))
```

This calculates expression and substitutes the result as text.

Example:

```bash
i=5
echo $((i + 1))
```

Output:

```text
6
```

## Arithmetic command

```bash
((i = i + 1))
```

This executes arithmetic operation.

So these are similar:

```bash
i=$((i + 1))
```

and:

```bash
((i = i + 1))
```

or:

```bash
((i++))
```

---

# 32. Semicolon `;` vs colon `:`

## Semicolon `;`

Semicolon separates commands on the same line.

These are equivalent:

```bash
i=1
j=$#
```

and:

```bash
i=1; j=$#
```

In one-line loops, semicolon is needed:

```bash
while (( i <= j )); do echo "$i"; ((i++)); done
```

## Colon `:`

Colon is a command that does nothing and returns success.

```bash
:
```

It is called no-op.

Example infinite loop:

```bash
while true; do
    :
done
```

So:

```text
; -> command separator
: -> empty successful command
```

---

# 33. Script using positional arguments and `shift`

Your script:

```bash
#!/bin/bash

i=1
j=$#

while [ "$i" -le "$j" ]
do
    echo "Param $i: $1"
    i=$((i + 1))
    shift 1
done
```

`shift 1` removes the first positional argument.

Example:

```bash
./script.sh a b c
```

Initially:

```text
$1 = a
$2 = b
$3 = c
$# = 3
```

After:

```bash
shift 1
```

we get:

```text
$1 = b
$2 = c
$# = 2
```

Your loop stores original argument count in `j`, so it still runs correct number of times.

More idiomatic version:

```bash
#!/bin/bash

i=1

while (($# > 0)); do
    echo "Param $i: $1"
    shift
    ((i++))
done
```

This loops while arguments remain.

---

# 34. `grep`

`grep` searches text using patterns.

Basic:

```bash
grep "main" file.c
```

Prints lines containing `main`.

## Why `grep something *`?

```bash
grep "main" *
```

The shell expands `*`.

If current directory contains:

```text
a.c b.c notes.txt
```

then shell runs:

```bash
grep "main" a.c b.c notes.txt
```

So `grep` receives file names as arguments.

Important:

```text
`*` is expanded by shell, not grep.
```

## Useful flags

### `-i`

Ignore case:

```bash
grep -i "error" log.txt
```

Matches:

```text
error
ERROR
Error
```

### `-I`

Ignore binary files:

```bash
grep -I "main" *
```

Useful when `*` may include compiled binaries.

### `-l`

Print only file names with matches:

```bash
grep -l "TODO" *.cpp
```

Instead of printing matching lines, it prints names of files containing matches.

### `-r`

Recursive search:

```bash
grep -r "TODO" src/
```

Searches inside directory recursively.

Common combination:

```bash
grep -RIn "TODO" src/
```

Meaning:

```text
-R -> recursive
-I -> ignore binary files
-n -> show line numbers
```

---

# 35. How is `grep` implemented conceptually?

At high level:

```text
1. Open file.
2. Read it line by line or block by block.
3. Match each line against pattern.
4. Print matching lines.
```

For simple substring search, it can use efficient string-search algorithms.

For regular expressions, grep implementations often compile the regex into an automaton-like internal structure.

Conceptually:

```text
pattern -> compiled matcher -> scan input
```

GNU grep is heavily optimized. It does not naively restart matching from every character in the simplest possible way.

Engineering lesson:

```text
grep is fast because it streams input and avoids loading whole files into memory.
```

---

# 36. `sed`

`sed` means stream editor.

It reads input line by line, applies editing commands, and writes output.

Basic substitution:

```bash
sed 's/old/new/' file.txt
```

This replaces first `old` in each line with `new`.

Replace all occurrences in each line:

```bash
sed 's/old/new/g' file.txt
```

Important:

```text
By default sed does not modify the original file.
It writes changed text to stdout.
```

So you can redirect:

```bash
sed 's/old/new/g' file.txt > changed.txt
```

## In-place editing

```bash
sed -i 's/old/new/g' file.txt
```

This modifies file.

Safer with backup:

```bash
sed -i.bak 's/old/new/g' file.txt
```

This creates:

```text
file.txt.bak
```

before modification.

Conceptually, in-place editing is often implemented by writing to a temporary file and replacing the original.

## Print selected lines

```bash
sed -n '1,10p' file.txt
```

Meaning:

```text
-n      -> do not print automatically
1,10p   -> print lines 1 to 10
```

## Delete lines

```bash
sed '/DEBUG/d' file.txt
```

Prints file without lines containing `DEBUG`.

---

# 37. `awk`

`awk` is a small programming language for text processing.

It reads input line by line.

Each line is split into fields.

```text
$0 -> whole line
$1 -> first field
$2 -> second field
...
NF -> number of fields
NR -> current line number
```

Example file:

```text
Alice 10
Bob 20
Charlie 30
```

Command:

```bash
awk '{ print $1 }' file.txt
```

Output:

```text
Alice
Bob
Charlie
```

Print second column:

```bash
awk '{ print $2 }' file.txt
```

Output:

```text
10
20
30
```

## Sum numbers in second column

```bash
awk '{ sum += $2 } END { print sum }' file.txt
```

Meaning:

```text
For each line: add field 2 to sum.
At the end: print sum.
```

## Use custom separator

CSV-like file:

```text
Alice,10
Bob,20
```

Use:

```bash
awk -F',' '{ print $1, $2 }' file.csv
```

`-F','` means field separator is comma.

## Filter lines

```bash
awk '$2 > 15 { print $0 }' file.txt
```

Prints lines where second field is greater than 15.

---

# 38. `grep` vs `sed` vs `awk`

Simple rule:

```text
grep -> search/filter lines
sed  -> transform/edit lines
awk  -> parse columns and compute
```

Examples:

```bash
grep "ERROR" log.txt
```

Find lines.

```bash
sed 's/error/ERROR/g' log.txt
```

Replace text.

```bash
awk '{ sum += $2 } END { print sum }' data.txt
```

Process structured columns.

---

# 39. Small practical testing script

Example project layout:

```text
program
tests/
    test1.in
    test1.out
    test2.in
    test2.out
```

Script:

```bash
#!/bin/bash

mkdir -p actual

for input in tests/*.in; do
    name=$(basename "$input" .in)
    expected="tests/${name}.out"
    actual="actual/${name}.out"
    err="actual/${name}.err"

    echo "Running $name"

    if ./program < "$input" > "$actual" 2> "$err"; then
        if diff -u "$expected" "$actual"; then
            echo "OK"
        else
            echo "Wrong answer"
        fi
    else
        echo "Runtime error"
        cat "$err"
    fi
done
```

This is the beginning of real test automation.

---

# 40. Practical engineering summary

Important ideas from this chapter:

```text
1. Shell runs programs as child processes.
2. stdin/stdout/stderr are file descriptors 0/1/2.
3. Redirection changes where these descriptors point.
4. Exit status 0 means success, non-zero means failure.
5. `&&` and `||` use exit codes for control flow.
6. `$PATH` controls command lookup.
7. Scripts usually run in child shells, so variables do not flow back to parent.
8. `source` runs code in the current shell.
9. `export` passes variables into child process environment.
10. `grep`, `sed`, and `awk` are streaming text-processing tools.
```

This is not only “Linux commands”. It is a practical introduction to how Unix processes, file descriptors, environments, and text streams work.
