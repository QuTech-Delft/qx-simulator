# QX Simulator #

Description...

## Supported Patforms

* Linux
* Windows
* OSX

## Required Packages

* gcc and g++ capable of C++11 standard
* Python (> 3.0)
* cmake (> 3.0) <!-- I didn't confirm but it worked with 3.0 -->
* libqasm (As a git submodule), which requires
    * Flex (> 2.6)
    * Bison (> 3.0)
    * doctest (As a git submodule)
* ...


# How to set up
<!---
N.B. the instructions below will compile the C++ files the first time you try to install OpenQL package. If you are updating an existing installation you should first clean and recompile the C++ files using the following command. 
--->
## Cloning the submodules
This project contains git submodules. \
If you didn't clone this repo using the `--recursive` option, please run: 
```
git submodule update --init --recursive
```

## Building
Clean previous build files:
```
rm -rf build       # delete the old build folder
```

<!-- ## Linux, Windows & OSX -->
Build the files:
```
mkdir build && cd build && cmake ../ && make
```
This creates a bin folder containing subfolders:\
&nbsp;&nbsp;&nbsp; /bin/qx-server\
&nbsp;&nbsp;&nbsp; /bin/qx-simulator\
Each containing its respective executable.

## Installing

To install you can further run:
```
  make install 
```

which installs the two executables under the default bin directory, usually:\
&nbsp;&nbsp;&nbsp;/usr/local/bin \

To change this, cmake can be ran with the following prefix:
```
  cmake -DCMAKE_INSTALL_PREFIX=<desired_absolute_path>
```





<!-- 
# Changelog

Please read [CHANGELOG.md](CHANGELOG.md) for more details. -->


<!-- # Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for more details.
Typically you will be need to perform the following steps to contribute:

1. `Fork` this repository
1. Create a `branch`
1. `Commit` your changes
1. `Push` your `commits` to the `branch`
1. Submit a `pull request`

You can find more information about Pull Requests [here](https://help.github.com/categories/collaborating-on-projects-using-pull-requests/) -->

<!-- # Contributors
Please see [CONTRIBUTORS.md](CONTRIBUTORS.md). -->
