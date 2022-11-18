# QX Simulator

A universal quantum computer simulator.

## Dependencies

QX should work on the following platforms.

 - Linux
 - Windows
 - MacOS

For compilation, you need the following things.

 - A C++ compiler with C++14 and OpenMP support (GCC for Linux, LLVM/clang for
   MacOS, MSVC 2015 with update 3 or above for Windows)
 - cmake (>= 3.13)
 - flex (>= 2.6.1)
 - bison (>= 3.0)

For the QXelarator Python module, you also need the following.

 - swig (Linux/MacOS >= 3.0.12; Windows >= 4.0.0)
 - python (>= 3.7) [for python interface to QX]

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

    qx.set('basic.qasm')                    # set the required qasm to be executed on qx
    qx.set_json_output_path('output.json)   # set the path to the output JSON, containing
                                                either complex amplitude (single shot) or
                                                measurement register averaging
    qx.execute()                            # execute the circuit once, print the final quantum
                                                state
    qx.execute(100)                         # execute the circuit 100 times, print measurement
                                                register averaging
    qx.get_measurement_outcome(0)           # get measurement results from qubit 'n' as bool
                                                note: in the case of repeated execution, the very
                                                last value of the measurement register is returned
    qx.get_state()                          # get quantum register state as string
                                                note: in the case of repeated execution, the very
                                                last quantum state is returned


### Installation

The easiest way to get QXelarator running on your machine is to use a pre-built
wheel. These are available for 64-bit Windows, Linux and OSX, for all currently
active Python versions. You can install them as follows.

    python3 -m pip install qxelarator --user

Alternatively, you can build and install from source using

    python3 -m pip install -v -e .

The latter might run faster on your machine, as it allows the compiler to
optimize for your particular CPU. The PyPI binaries assume that only SSE3 is
available.

## Licensing

QX is licensed under the Apache License, Version 2.0. See
[LICENSE](https://github.com/QuTech-Delft/qx-simulator/blob/master/LICENSE) for the full
license text.
