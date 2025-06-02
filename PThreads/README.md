
# Simple Producer-Consumer C Program with Timed Thread Creation

## Overview

This C program demonstrates a classic **producer-consumer problem** using **POSIX threads (pthreads)**. It simulates a scenario where multiple "producer" threads generate units and add them to a shared buffer, while multiple "consumer" threads retrieve units from this buffer.

The program illustrates key concepts of concurrent programming:

* **Thread Creation and Management** using `pthread_create` and `pthread_detach`.
* **Synchronization Primitives**:

  * **Mutexes (`pthread_mutex_t`)**: Protect shared resources (buffer and unit count) from race conditions.
  * **Condition Variables (`pthread_cond_t`)**: Allow threads to wait efficiently (e.g., when buffer is empty or full).
* **Timed Thread Spawning**: Producer and consumer threads are created at configurable intervals.

---

## Purpose

This program is designed as an **educational tool** to help understand:

* Multi-threading in C
* Synchronization techniques in concurrent environments
* Solutions to classic concurrency problems like producer-consumer
* Basic OS-level threading concepts

---

## Features

* Shared buffer with defined maximum capacity
* Producers generate a **random number of units**
* Consumers consume a **random number of units**
* Producers **wait** if buffer is full
* Consumers **wait** if buffer is empty
* Configurable number of producer and consumer threads
* Configurable time intervals for spawning producer and consumer threads
* Clear console output logs actions and buffer state

---

## Prerequisites

* A **C compiler** (e.g., GCC or Clang)
* A **POSIX-compliant OS** that supports pthreads (e.g., Linux, macOS)

---

## How to Compile

Use a C compiler with the `-pthread` flag to link the pthreads library:

```bash
gcc your_program_file.c -o producer_consumer -pthread
```

> Replace `your_program_file.c` with your actual source file name (e.g., `main.c` or `producer_consumer.c`).

---

## How to Run

Once compiled, you can run the program:

```bash
./producer_consumer [number_of_threads]
```

* `./producer_consumer`: Runs the program
* `[number_of_threads]` *(Optional)*: Number of producer and consumer threads to create. Defaults to a preset value if omitted.

### Example

To run with 7 producers and 7 consumers:

```bash
./producer_consumer 7
```

---

## Code Structure (Brief)

* `thread_produce()`: Function executed by producer threads
* `thread_consume()`: Function executed by consumer threads
* `main()`:

  * Initializes mutexes and condition variables
  * Seeds the random number generator
  * Parses command-line arguments
  * Spawns consumer threads at timed intervals
  * Spawns producer threads at timed intervals
  * Sleeps to allow detached threads to run

---

## Notes

* Maximum buffer size and thread creation intervals can be configured using `#define` constants at the top of the source code.
* The program uses `usleep()` for timed thread creation and `sleep()` in `main()` to let detached threads run briefly.
* For production use, consider implementing more robust shutdown and synchronization mechanisms.

