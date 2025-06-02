Here is a properly formatted and structured README in Markdown format for your **Multi-Level Feedback Queue (MLFQ) Scheduling Program** — ready to copy and paste:

---

# Simple MLFQ Scheduling Program in C

## Overview

This program implements a simplified **Multi-Level Feedback Queue (MLFQ)** scheduler in C. It was created as a learning exercise to understand foundational concepts in operating systems, specifically process scheduling and queue management.

The program simulates a scheduler that manages processes across multiple priority levels using a time-sliced Round Robin (RR) strategy. Each process is executed, demoted based on usage, and periodically promoted back to the highest priority level — mimicking real-world MLFQ behavior.

---

## Key Concepts Demonstrated

* **Process Control Structures** using custom structs
* **Queue Data Structures** for managing priority levels
* **Round Robin Scheduling** with individual time slices per level
* **Priority Aging** and dynamic priority adjustment
* **Memory Management** with `malloc` and `free`

---

## Features

* Defines `Process` and `Queue` structures to model a system of scheduled tasks.
* Implements **4-level MLFQ**, where each queue has:

  * Different time slice (`quantum`)
  * Lower queues represent lower priority
* Processes start in the **highest-priority queue** (`q[0]`).
* If a process does not finish within its time slice:

  * It is **demoted** to a lower-priority queue.
* Every 3 full MLFQ cycles:

  * All remaining processes are **promoted** back to the highest-priority queue (`q[0]`).
* Provides verbose console output showing process activity and scheduling decisions.

---

## Program Structure

### Data Structures

* `Process`:

  * Fields: `pid`, `size`, `name`, `priority`, and `next` pointer
* `Queue`:

  * Singly-linked queue with `first` and `last` pointers

### Functions

* `createQueue()`: Initializes an empty queue
* `createProcess()`: Allocates and initializes a new process
* `addToQueueLast()`: Adds a process to the end of a queue
* `getFirstFromQueue()`: Removes and returns the first process in a queue
* `rr()`: Performs Round Robin scheduling for a single queue
* `moveToHighPriority()`: Reprioritizes all remaining processes to the highest-priority queue
* `MLFQ()`: Manages all queues, invokes RR, tracks cycle counts, and handles periodic reprioritization

---

## How to Compile

Use a C compiler such as `gcc`:

```bash
gcc mlfq_scheduler.c -o mlfq_scheduler
```

> Replace `mlfq_scheduler.c` with your actual source file name.

---

## How to Run

After compiling, run the program:

```bash
./mlfq_scheduler
```

The program will output step-by-step details of process scheduling as it simulates execution.

---

## Sample Output Behavior

```
==========================================================

Round Robin for Queue 0 Is In Progress: (Time Slice: 4)

==========================================================

🔄Process A is running: size = 63, priority = 0
❗️Process A Has Been Moved to a Lower Priority.

...

✅Process B has completed.

...

⭐️All Processes Have Been Raised to the Highest Priority⭐️
```

---

## Customization

You can easily modify:

* **Number of Queues**: Change `numOfQueues` and adjust `timeSlice[]`
* **Time Slice Durations**: Modify `slice_q0`, `slice_q1`, etc.
* **Initial Processes**: Add or remove process creation lines in `main()`

---

## Notes

* No external libraries are required.
* This is a **simulation** — there is no real multitasking or threading involved.
* This is intended as a **learning tool**, not a production-grade scheduler.
* All memory allocations via `malloc` are freed upon process completion.

---

Let me know if you'd like a downloadable version of this README or help integrating this into your project structure (e.g., in a GitHub repository).
