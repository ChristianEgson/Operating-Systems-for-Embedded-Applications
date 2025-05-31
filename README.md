# Operating-Systems-for-Embedded-Applications Projects

This repository contains a collection of hands-on projects completed during the *Operating Systems for Embedded Applications* course. The course focused on key OS concepts including memory management, device and file systems, multi-threading, scheduling algorithms, and kernel-level programming in a Linux environment—culminating in the design and implementation of components commonly found in real-time embedded systems.

## 📚 Course Description

> **Operating System Concepts:**  
> Topics include memory, device and file management techniques, multi-threaded programming, synchronization mechanisms, CPU scheduling, and the design of real-time operating systems for embedded controllers. Development is Linux-based.

---

## 🧠 Projects Overview

### 1. 🐚 Simple Shell with Custom Commands

A lightweight shell application in C that:
- Runs basic commands from `/usr/bin`
- Includes two custom commands:
  - `help` – displays information about the shell
  - `close` – cleanly terminates the shell application

Useful for understanding process creation (`fork`, `exec`), command parsing, and system call handling.

---

### 2. 🧮 Multi-Level Feedback Queue (MLFQ) Scheduler

A simulation of a **Multi-Level Feedback Queue (MLFQ)** CPU scheduler using dummy processes:
- Implements multiple priority queues
- Demonstrates aging and demotion/promotion of processes
- Reinforces CPU scheduling algorithms and process management concepts

---

### 3. 🔄 Producer-Consumer with Condition Variables

Multi-threaded producer-consumer system using `pthreads` and condition variables:
- Shared stock limit of 100 units
- Randomized producers (1–20 units) and consumers (1–5 units)
- Two condition variables:
  - `stock_empty` – for consumers to wait when stock is low
  - `stock_full` – for producers to wait when stock is full
- Threads dynamically spawned at time intervals
- Real-time stock updates and wait status are printed to the console

A practical application of thread synchronization and inter-thread communication using mutexes and condition variables.

---

### 4. ⚙️ Custom Kernel Module with `ioctl`

A Linux kernel module with custom file operations:
- Adds `unlocked_ioctl` support alongside `read` and `write`
- Includes a user-space program that interacts with the kernel module
- Implements a **4-digit numeric code guesser** using:
  - User space: sends guesses via `ioctl`
  - Kernel space: processes and responds to the guesses
- Built with a Makefile for both user and kernel components

An introduction to character device drivers, kernel/user space communication, and system-level programming.

---

## 🛠️ Technologies Used

- **C / C++**
- **POSIX Threads (pthreads)**
- **Linux Kernel Development**
- **Makefile / GCC**
- **Bash / Shell scripting**
- **GDB (for debugging)**

---

## 📁 Repository Structure

