# feup-sope-proj

![Ubuntu version](https://img.shields.io/badge/Ubuntu-18.04.02%20LTS-red)
![GCC version](https://img.shields.io/badge/gcc-7.4.0-green)
![bash version](https://img.shields.io/badge/bash-4.4.19-lightgrey)

Resolution proposal of the project from the course unit Operating Systems.

Development of _simpledu_, a tool to summarize the use of disk space in a file or directory. In the case of a directory, the information must include files and subdirectories that are contained therein.

## Instalation

Run _make_ inside the root of the repo (where makefile lies).
The binary file will be placed inside /bin

## Usage

```bash
simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]
```

where:

* **-l, --count-links** – allows the contabilization of the same file multiple times;
* **-a, --all** - shows also files information;
* **-b, --bytes** – shows the real number of bytes, instead of blocks;
* **-B, --block-size=SIZE** – defines the size (in bytes) of a block;
* **-L, --dereference** – follows symbolic links;
* **-S, --separate-dirs** – doesn't count the size of subdirectories;
* **--max-depth=N** – limits the depth of subdirectories (default: infinite).

### Exit codes

* **0** - regular execution.
* **1** - _args error_, error in the arguments:
  * not including the -l flag;
  * invalid block size;
  * invalid depth.
* **2** - _directory error_, error in the directories:
  * permission denied;
  * directory not existent;
  * [etc](http://man7.org/linux/man-pages/man3/opendir.3.html).
* **3** - _pipe error_, error creating pipes:
  * limit of file descriptors reached;
  * max memory allocated for pipes reached;
  * [etc](http://man7.org/linux/man-pages/man2/pipe.2.html).
* **4** - _fork error_, error forking processes:
  * max allowed amount of threads reached;
  * fork is not supported on this platform;
  * [etc](http://man7.org/linux/man-pages/man2/fork.2.html).

### Log file

A log file is always created wih the information described below. For that, the program searches for a environment variable named *LOG_FILENAME* with the wanted location and respective name. The default log, when there's no such variable, is *./logfile.log*

For each execution, a header is written with the following structure:
> Program executed at HH:mm, DD/MM/YYYY

Each line of the log follows the following structure:
> instant – pid – action – info

with:

* **instant**: time elapsed after the beginning of the execution;
* **pid**    : the ID of the process that logged;
* **action** : description of the type of log (CREATE, EXIT, etc);
* **info**   : additional information.

| **action**  | description                      | **info**                                                     |
| ----------- | -------------------------------- | ------------------------------------------------------------ |
| CREATE      | creation of a process            | the command line args                                        |
| EXIT        | termination of a process         | the exit status                                              |
| RECV_SIGNAL | reception of a signal            | the received signal (e.g. SIGINT)                            |
| SEND_SIGNAL | sending of a signal              | the sent signal, followed by the pid of the process that will receive it |
| RECV_PIPE   | reading from a pipe              | the received message                                         |
| SEND_PIPE   | writing on a pipe                | the written message                                          |
| ENTRY       | analysing of a file or directory | the number of bytes, followed by its whole path              |

## Authors

* Márcio Duarte | [GitHub](https://github.com/ctrlMarcio) | [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201909936)
* Luís Tavares | [GitHub](https://github.com/luist18)  | [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201809679)

## License

[MIT](https://opensource.org/licenses/MIT)

## Project Status

Currently work in progress, first release at 10/04/2020
