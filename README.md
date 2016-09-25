# osux
This is an experimental osu developement kit in C.

## Compilation
The project can be compiled using Autotools

### (re-)generate the build system: (you must probably do this if you pulled the project from a git repo)
```
./autogen.sh
```

### build:
```
./configure
make
make install
```
If you want to install the project in another directory add  `--prefix=/absolute/path/to/install/dir`.

## Components

#### Developement kit
The project implements the following functionalities:
* osu beatmap parser
* osu replay parser
* database for finding a beatmap path with its hash

## (Bunch of) Dependencies
* libyaml
* libsqlite3
* glib2.0 (The Gnome Portable Library) :3
* libcrypto (OpenSSL)
* liblzma (aka xz, xz utils)

