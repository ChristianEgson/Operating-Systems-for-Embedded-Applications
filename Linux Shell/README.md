Here's a clean and properly formatted **README** in Markdown for your **Custom Linux Shell Program**, ready for copy-paste:

---

# Simple Custom Linux Shell in C

## Overview

This is a simple, educational **custom shell program written in C**, designed to explore the basics of Unix/Linux system calls, process creation, and user interaction. The shell reads commands from the user and responds by executing built-in or system-level functions, some for utility and others just for fun.

It was developed as part of learning the fundamentals of **Operating Systems**, focusing on process management and system-level I/O.

---

## Features

* Command-line interface that mimics a minimal terminal experience
* Forks a child process for each command entered
* Built-in commands:

  * `done` – Exit the shell
  * `help` – Display available commands
  * `cwd` – Show current working directory
  * `ls` – List contents of the current directory
  * `clr` – Clear the terminal screen
  * `d/t` – Display current date and time
  * `createf` – Prompt to create a file in the current directory
  * `uname` – Print the system name (like `uname -s`)
  * `oops` – Initiates a humorous "self-destruct" sequence
  * `quote` – Displays a randomly selected tech-related pickup line

---

## How It Works

### User Input

* Reads input using `fgets()` and trims the newline character.
* Commands are processed using `strcmp()` to determine the appropriate action.

### Child Process Execution

* Each valid command (except `help` and `done`) is executed in a **child process** using `fork()`.
* Built-in commands are handled with direct system calls or custom logic.
* For unsupported commands, a message is printed to inform the user.

---

## Code Highlights

### Functions

* `createChild()`: Forks and returns process ID; waits for child to complete
* `helpScreenPrint()`: Prints the list of valid commands
* `quotes()`: Prints a randomly selected tech-themed pickup line
* `timeBomb()`: Executes a countdown followed by a fake "explosion" message

---

## How to Compile

Use a C compiler (like `gcc`):

```bash
gcc simple_shell.c -o simple_shell
```

> Replace `simple_shell.c` with the actual filename.

---

## How to Run

After compiling, run the shell from the terminal:

```bash
./simple_shell
```

You'll be greeted with a custom prompt:

```bash
christian@coolestTerminal:
```

Start typing commands (e.g., `cwd`, `ls`, `createf`, etc.)

---

## Example Usage

```
christian@coolestTerminal: help

*******Available Commands*******

done: to close application
cwd: to check current Working Directory
ls: to check files within current directory
clr: to clear terminal
d/t: to produce current local date & time
createf: create a file within current directory
oops: call this command only under dire circumstances
rizz: use this command if you're single and ready to mingle
```

---

## Notes

* This shell does **not** support piping, argument parsing, or background execution.
* Fun and creative commands like `oops` and `quote` add personality to the shell.

---

Let me know if you'd like a version of this README exported as a `.md` file, or assistance adding new commands or features to the shell!
