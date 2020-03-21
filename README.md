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

## Authors

* Márcio Duarte | [GitHub](https://github.com/ctrlMarcio) [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201909936)
* Luís Tavares | [GitHub](https://github.com/luist18) [FEUP](https://sigarra.up.pt/feup/pt/fest_geral.cursos_list?pv_num_unico=201809679)

## License

[MIT](https://opensource.org/licenses/MIT)

## Project Status

Currently work in progress, first release at 10/04/2020
