# feup-sope-proj

![C/C++ CI Build](https://github.com/ctrlMarcio/feup-sope-proj/workflows/C/C++%20CI%20Build/badge.svg) [![C/C++ CI Documentation](https://github.com/ctrlMarcio/feup-sope-proj/workflows/C/C++%20CI%20Documentation/badge.svg)](https://ctrlmarcio.github.io/feup-sope-proj/) ![GCC version](https://img.shields.io/badge/gcc-7.4.0-green) ![bash version](https://img.shields.io/badge/bash-4.4.19-lightgrey)

Resolution proposal of the project from the course unit Operating Systems.

The goal is to make a **C** program based to do the exactly same thing as the `du` command of the Unix Shell, using the Unix API for C.

## Files

### Source files

The source files are located in the src/ folder. The main function is inside the `main.c` file and the most important functions are inside the `simpledu.c` file. Apart from those files, there are some additional files that contain some functions that are useful for the main algorithm but not directly related to the problem.

### Compiling

There is a make file in order to compile the program properly. Run `make` in the root directory of the repository and a binary ouput file is placed inside a bin/ folder in the repository root.

## Usage and running

The usage of the program is similar to the `du` usage, with some tweaks.
```bash
simpledu [OPTIONS]
```

### Options

* **-l, --count-links** – allows the contabilization of the same file multiple times;
* **-a, --all** - shows also files information;
* **-b, --bytes** – shows the real number of bytes, instead of blocks;
* **-B, --block-size=SIZE** – defines the size (in bytes) of a block;
* **-L, --dereference** – follows symbolic links;
* **-S, --separate-dirs** – doesn't count the size of subdirectories;
* **--max-depth=N** – limits the depth of subdirectories (default: infinite).

>**Note:** the use of **-l** is mandatory

### Exit codes

* **0** - regular execution.
* **1** - _args error_, error in the arguments:
  * not including the -l flag;
  * invalid block size;
  * invalid depth.
* **1** - _directory error_, error in the directories:
  * permission denied;
  * directory not existent;
  * [etc](http://man7.org/linux/man-pages/man3/opendir.3.html).
* **2** - _pipe error_, error creating pipes:
  * limit of file descriptors reached;
  * max memory allocated for pipes reached;
  * [etc](http://man7.org/linux/man-pages/man2/pipe.2.html).
* **3** - _fork error_, error forking processes:
  * max allowed amount of threads reached;
  * fork is not supported on this platform;
  * [etc](http://man7.org/linux/man-pages/man2/fork.2.html).

### Log file

The operations done while the program is running are reported in a log file. The log file is created with the name of the environment variable named `LOG_FILENAME`, or the default name '../logfile.log'.

For each execution, a header is written with the following structure:
> Program executed at HH:mm, DD/MM/YYYY

Each line of the log follows the following structure:
> instant – pid – action – info

with:

* **instant**: time elapsed after the beginning of the execution;
* **pid**    : the ID of the process that logged;
* **action** : description of the type of log (CREATE, EXIT, etc);
* **info**   : additional information.

| **Action**  | Description                      | **Info**                                                     |
| ----------- | -------------------------------- | ------------------------------------------------------------ |
| CREATE      | creation of a process            | the command line args                                        |
| EXIT        | termination of a process         | the exit status                                              |
| RECV_SIGNAL | reception of a signal            | the received signal (e.g. SIGINT)                            |
| SEND_SIGNAL | sending of a signal              | the sent signal, followed by the pid of the process that will receive it |
| RECV_PIPE   | reading from a pipe              | the received message                                         |
| SEND_PIPE   | writing on a pipe                | the written message                                          |
| ENTRY       | analysing of a file or directory | the number of bytes, followed by its whole path              |

## Features

| **Feature** | Status | Additional info |
| -------------- | :--------: |------------|
| Argument reading and processing | ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen)| N/a |
| Different exit code for each error | ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) | N/a |
| Dereference links | ![https://img.shields.io/badge/good-%20-yellowgreen](https://img.shields.io/badge/good-%20-yellowgreen) | Not working if a loop occurs|
| Specify the max depth of the search| ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) | N/a|
| Directory and file size communication with pipes| ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) | N/a|
| Directory handling with different a processes | ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) | N/a|
| Show all files and directories (option -a) | ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) | N/a |
| Signal handling | ![https://img.shields.io/badge/good-%20-yellowgreen](https://img.shields.io/badge/good-%20-yellowgreen) |N/a|
| Log information | ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) |N/a|
| Different size according to block size or bytes | ![https://img.shields.io/badge/great-%20-brightgreen](https://img.shields.io/badge/great-%20-brightgreen) | N/a |

## Authors

* Márcio Duarte | [GitHub](https://github.com/ctrlMarcio) | [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201909936)
* Luís Tavares | [GitHub](https://github.com/luist18)  | [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201809679)

## License

[MIT](https://opensource.org/licenses/MIT)

## Disclaimer

This repository contains the solution proposed by the authors and might contain errors as well as lack of efficiency.

The first release was on 10/04/2020 and a second one is expected whenever the assignment comes.
