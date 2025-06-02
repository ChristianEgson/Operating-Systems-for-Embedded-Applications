Here's a properly structured and formatted **README** for your **CodeCracker Linux Kernel Module and User-Space Game**, written in Markdown and ready for copy-paste:

---

# 🔐 CodeCracker — Linux Kernel Module Game

## Overview

**CodeCracker** is a simple number-guessing game implemented as a **Linux kernel module**, complete with a **user-space interface** via device file and IOCTL calls. It challenges the user to guess a secret 4-digit code, providing feedback on digits that are correct and/or in the correct position.

This project demonstrates:

* Character device drivers in the Linux kernel
* IOCTL-based communication between kernel and user-space
* Random number generation in the kernel
* Secure data copying between user and kernel space

---

## Features

### Kernel-Level Module

* Character device named `/dev/codecracker`
* IOCTL interface for:

  * Resetting the secret code
  * Submitting and validating guesses
* Dynamically generated 4-digit secret using `get_random_long()`
* Feedback includes:

  * Count of digits in the **correct position**
  * Count of correct digits in the **wrong position**
* Internal read/write buffer for reporting guess outcomes
* Clear, structured logging using `printk()`

### User-Space Program

* Console-based interactive interface
* Uses `ioctl()` to:

  * Reset the game
  * Submit guesses
* Displays game feedback and history
* Detects game completion (4 correct digits in correct position)

---

## Kernel Module Structure

### Key Components

* `codecracker_init()`: Registers the character device
* `codecracker_end()`: Cleans up and unregisters device
* `generate_code()`: Generates random 4-digit secret
* `codecracker_ioctl()`: Handles IOCTL commands:

  * `RESET`: Regenerate secret code
  * `GUESS`: Process user's guess and send result back
* `codecracker_read()` and `codecracker_write()`: Handle internal game message buffering

### IOCTL Definitions

```c
#define CODECRACKER_IOCTL_BASE 0xF0
#define CODECRACKER_IOCTL_RESET _IO(CODECRACKER_IOCTL_BASE, 0)
#define CODECRACKER_IOCTL_GUESS _IOWR(CODECRACKER_IOCTL_BASE, 1, struct codecracker_guess)
```

---

## User-Space Interface

### Key Structure

```c
struct codecracker_guess {
    char user_guess[5];
    int correct_position;
    int correct_digit_wrong_position;
};
```

### Program Flow

1. Open `/dev/codecracker`
2. Call `ioctl(..., CODECRACKER_IOCTL_RESET)` to initialize
3. Repeatedly:

   * Prompt user for 4-digit guess
   * Submit guess via `ioctl(..., CODECRACKER_IOCTL_GUESS)`
   * Print game feedback and read latest game message
4. Exit when guess is fully correct

---

## How to Build and Run

### 1. Build Kernel Module

Ensure you have kernel headers installed.

```bash
make
```

### 2. Load the Module

```bash
sudo insmod codecracker.ko
```

### 3. Create Device File (if needed)

```bash
sudo mknod /dev/codecracker c <major_number> 0
```

> Replace `<major_number>` with the value printed in `dmesg` after module insertion.

Or use `udev` if supported.

### 4. Compile and Run User Program

```bash
gcc user_codecracker.c -o user_codecracker
./user_codecracker
```

### 5. Unload Module After Use

```bash
sudo rmmod codecracker
```

---

## Makefile

```make
obj-m += codecracker.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

---

## Sample Output

```
Game Resetting...

Enter a 4-digit guess: 1234
-> Correct digits in correct position: 1
-> Correct digits but in wrong position: 2
-> Guess 1: 1234 | Correct Position: 1, Wrong Position: 2

...

You have cracked the code!
```

---

## Notes

* Module licensed under **GPL** to comply with kernel requirements.
* Uses `copy_from_user()` and `copy_to_user()` for safe memory access.
* Game logic is similar to "Mastermind" or "Bulls and Cows".
