[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/u16ttUuk)

# CSEShell

CSEShell is a simple, custom shell for Unix-based systems, designed to provide an interface for executing system programs. This project includes a basic shell implementation, a set of system programs (`backup`, `dcheck`, `dspawn`, `find`, `ld`, `ldr`, `sys`), and some test files.

This shell was designed and implmented as part of SUTD course 50.005 Computer System Engineering programming assignment. Currently, the shell is only tested to work on WSL on Windows.

## Directory Structure

The project is organized as follows:

- `bin/` - Contains compiled executables for system programs.
  - `find` - Program to find files.
  - `ld` - Program for listing the contents of the current directory.
  - `ldr` - Program for listing the contents of the current directory recursively.
  - `backup` - Program to create a zip backup of a directory or file.
  - `dcheck` - Program to check how many daemons spawned by dspawn are running.
  - `dspawn` - Program to spawn a daemon process.
  - `sys` - Program to display system information.
- `cseshell` - The main executable for the CSEShell.
- `files/` - Contains various test files used with the shell and system programs.
  - `combined.txt`, `file1.txt`, `file2.txt`, ... - Test text files.
  - `notes.pdf` - A PDF file for testing.
  - `ss.png` - An image file.
- `makefile` - Makefile for building the CSEShell and system programs.
- `source/` - Source code for the shell and system programs.
  - `shell.c` and `shell.h` - Source and header files for the shell.
  - `system_programs/` - Source code and header for the system programs.

## Building the Project

To build the CSEShell and system programs, run the following command in the root directory:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

## Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

From there, you can execute built-in commands and any of the included system programs (e.g., `find`, `ld`, `ldr`, `backup`, `dcheck`, `dspawn`, `sys`).

## System Programs

- `find.c` - Searches for files in a directory.
- `ld.c` - List the contents of the curent directory.
- `ldr.c` - List the contents of the current directory recursively.
- `backup.c` - Creates a zip backup of a directory or file and moves it to the archive directory.
- `dcheck.c` - Checks how many daemon processes spawned by dspawn are running.
- `dspawn.c` - Spawns a daemon process that runs in the background.
- `sys.c` - Displays system information, including OS, kernel, total memory size, logged-in user, and CPU details.

Each program can be executed from the CSEShell once it is running. Shell can execute command many times and invalid/empty commands will result in no command executing and await new command.

## Files Directory

The `files/` directory contains various text, PDF, and image files for testing the functionality of the CSEShell and its system programs.

## Makefile

The Makefile contains rules for compiling the shell and system programs. You can clean the build by running:

```bash
make clean
```

## Source Directory

Contains all the necessary source code for the shell and system programs. It is divided into the shell implementation (`shell.c`, `shell.h`) and system programs (`system_programs/`).

## Additional Feature

We decorated the prompt by including the current working directory which is very useful in traversing file paths using the additional builtin commands like `cd`. This makes the shell more user-friendly and provides context to their location within the file system.

## Sustainability and Inclusivity

Sustainability:
We implemented a resource usage feedback which highlights CPU time and memory usage after each command execution to promote awawreness of resource consumption.

Inclusivity:
Creating a backup requires the folder `./archives` to be present which might not be there. We implement a additional inclusivity feature by creating the folder for them if it is not present.

The code also provides inclusive error messaging by implementing error messages where they are needed, and making the messages clear, informative, and user-friendly. The use of `perror` adds more detailed system error messaging to output, giving users more context. For example, it highlights missing environment varialbes, failure to create archives directory, too long path/command, and failure to create/move zip file.

Our additional feature also enhances inclusivity by highlighting the current working directory, making it more user friendly.
