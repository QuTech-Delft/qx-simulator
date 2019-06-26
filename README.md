# QX Simulator

A universal quantum computer simulator.


# Supported Patforms

* Linux
* Windows

# Required Packages

The following packges are required if you are not using pre-built conda packages:

* g++ compiler with C++11 support (Linux)
* MSVC 2015 with update 3 or above (Windows)
* cmake (>= 3.0)
* swig
* python (>= 3.5) [for python interface to QX]


# Installation

To build run the following in the current directory:
```sh
mkdir build && cd build && cmake ../ && make
```

This creates a bin folder containing subfolders:
-  /bin/qx-server
-  /bin/qx-simulator

Each containing its respective executable

To install you can further run:
```sh
make install
```

which installs the two executables under the default bin directory, usually:
  /usr/local/bin

To change this, cmake can be ran with the following prefix:

```sh
cmake -DCMAKE_INSTALL_PREFIX=<desired_absolute_path>
```

# QXelerator Installation

Follow the instructions in [README.md](qxelarator/README.md) in the qxelerator directory.


# Licensing

QX is licensed under the Apache License, Version 2.0. See
[LICENSE](https://github.com/QE-Lab/qx-simulator/blob/master/LICENSE) for the full
license text.
