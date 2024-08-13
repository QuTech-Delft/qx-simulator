# QX Simulator

[![CI](https://github.com/QuTech-Delft/qx-simulator/workflows/Test/badge.svg)](https://github.com/qutech-delft/qx-simulator/actions)
[![PyPI](https://badgen.net/pypi/v/qxelarator)](https://pypi.org/project/qxelarator/)

A universal quantum computer simulator. Allows simulation of a quantum circuit written in [cQASM](https://qutech-delft.github.io/cQASM-spec/) v3.0.

Can be used as a [Python module](https://pypi.org/project/qxelarator/) or as an executable.

## User manual

[Coming soon] QX Simulator documentation is hosted through GitHub Pages [here](https://QuTech-Delft.github.io/qx-simulator/).

## File organization

For development, see:

- `include`: public headers.
- `src`: source files.
- `test`: test files.
- `python`: SWIG interface.

For build process, and continuous integration:

- `.github`: GitHub Actions configuration files.
- `conan`: Conan profiles.

Build outputs may go into:

- `build/<build type>`: the C++ library output files generated by Conan.
- `pybuild`: the Python library output files generated by `setup.py`.

## Dependencies

* C++ compiler with C++23 support (gcc 12, clang 15, msvc 19)
* `CMake` >= 3.12
* `git`
* `Python` 3.x plus `pip`, with the following package:
  * `conan` >= 2.0.6
* `SWIG`

## Build

This version of QX Simulator can only be compiled via the Conan package manager.
You'll need to create a default profile before using it for the first time.

The installation and compilation of dependencies can be done in one go.

```shell
git clone https://github.com/QuTech-Delft/qx-simulator.git
cd qx-simulator
conan profile detect
conan build . -pr:a=conan/profiles/tests-debug -b missing
```

Notice:
- the `conan profile` command only has to be run only once, and not before every build.
- the `conan build` command is building `qx-simulator` in Debug mode with tests using the `tests-debug` profile.
- the `-b missing` parameter asks `conan` to build packages from sources
in case it cannot find the binary packages for the current configuration (platform, OS, compiler, build type...). 

### Build profiles

A group of predefined profiles is provided under the `conan/profiles` folder.<br/>
They follow the `[tests-](build_type)(-compiler)(-os)(-arch)` naming convention:
  - `tests`: if tests are being built.
  - `build_type`: can be `debug` or `release`.
  - `compiler`: `apple-clang`, `clang`, `gcc`, `msvc`.
  - `os`: `linux`, `macos`, `windows`.
  - `arch`: `arm64`, `x64`.

All the profiles set the C++ standard to 23.<br/>
All the `tests`, except for `linux-x64` profiles, enable Address Sanitizer.

### Build options

Profiles are a shorthand for command line options. The command above could be written, similarly,  as: 

```shell
conan build . -s:a compiler.cppstd=23-s:a qx/*:build_type=Debug -c tools.build:skip_test=False -b missing
```

This is the list of options that could be specified either in a profile or in the command line:

- `qx/*:asan_enabled={True,False}`: enables Address Sanitizer.
- `qx/*:build_python={True,False}`: builds the Python SWIG interface.
- `qx/*:build_type={Debug,Release}`: builds in debug or release mode.
- `qx/*:cpu_compatibility_mode={True,False}`: produce binaries that would work on any CPU,
instead of just the one of the machine on which the code is built.

Tests are disabled by default. To enable them, use `-c tools.build:skip_test=False`.

## Install

### From Python

Install from the project root directory as follows:

```shell
python3 -m pip install --verbose .
```

Notice that you will need to have `SWIG` installed for the above to work.

You can test if it works by running:

```
python3 -m pytest
```

### From C++

The `CMakeLists.txt` file in the root directory includes install targets:

```shell
conan create --version 0.7.1 . -pr:a=tests-debug -b missing
```

You can test if it works by doing:

```shell
cd test/Debug
ctest -C Debug --output-on-failure
```

## Use from another project

### From Python

The `qxelarator` module can be imported from another Python file with:

```python
import qxelarator
```

It should thenprovide access to a single API:
- `execute_string`.

## Licensing

QX-simulator is licensed under the Apache License, Version 2.0. See
[LICENSE](https://github.com/QuTech-Delft/qx-simulator/blob/master/LICENSE.md) for the full
license text.

## Authors

Quantum Inspire: [support@quantum-inspire.com](mailto:"support@quantum-inspire.com")
