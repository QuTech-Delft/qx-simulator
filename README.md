# QX Simulator

A universal quantum computer simulator.

## Dependencies

QX should work on the following platforms.

 - Linux
 - Windows
 - MacOS

For compilation, you need the following things.

 - A C++ compiler with C++11 and OpenMP support (GCC for Linux, LLVM/clang for
   MacOS, MSVC 2015 with update 3 or above for Windows)
 - cmake (>= 3.13)
 - flex (>= 2.6.1)
 - bison (>= 3.0)

For the QXelarator Python module, you also need the following.

 - swig (Linux/MacOS >= 3.0.12; Windows >= 4.0.0)
 - python (>= 3.5) [for python interface to QX]

## Installation

To build run the following in the current directory:

```sh
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=<desired_install_path>
cmake --build .
cmake --target install # Linux/MacOS
cmake --target INSTALL # Windows
```

`<desired_install_path>` must be an absolute path to where you want to install
QX.


## QXelarator: QX as a Quantum Accelerator

QXelarator (note the typo; sorry) is the python interface to QX simulator,
which is perhaps the easiest way to start simulating quantum circuits.

### API

`qxelarator` mainly provides the following API calls:

    qx.set('basic.qasm')            # set the required qasm to be executed on qx
    qx.execute()                    # execute
    qx.get_measurement_outcome(0)   # get measurement results from qubit 'n' as bool
    get_state()                     # get quantum register state as string


### Installation

The easiest way to get QXelarator running on your machine is to use a pre-built
wheel. These are available for 64-bit Windows, Linux and OSX, for all currently
active Python versions. You can install them as follows.

    python3 -m pip install qxelarator --user

Alternatively, you can build and install from source using

    python3 -m pip install -v -e .

## Licensing

QX is licensed under the Apache License, Version 2.0. See
[LICENSE](https://github.com/QE-Lab/qx-simulator/blob/master/LICENSE) for the full
license text.
