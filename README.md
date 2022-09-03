# CAN Gauge

A home-made solution for displaying OBDII data on a gauge.

## Setup

### Cloning

After cloning the repo, you'll also need to clone the submodules.

```shell
# Clone the repo.
$ git clone git@github.com:thomas-crane/cangauge.git
$ cd cangauge

# Get the submodules.
$ git submodule update --init

# Get the submodules for pico-sdk.
$ pushd libs/pico-sdk
$ git submodule update --init
$ popd
```

You can use `git submodule update --init --recursive` instead of individually getting the submodules for cangauge and then pico-sdk, but this will result in a much larger file size for the repo. pico-sdk uses tinyusb as a submodule, and tinyusb has tens of submodules that will also be cloned if `--recursive` is used.

### Building

cangauge uses cmake as the build system.

```shell
# Configure cmake.
$ cmake -B build

# Build the project.
$ cmake --build build/
```

It can be a little bit quicker to use `make` directly with the `-j` flag to build the project using multiple cores.

```shell
$ cmake -B build

$ cd build/
# Build using 4 cores. Running `lscpu` will give you info on how many cores you have.
$ make -j4
```
