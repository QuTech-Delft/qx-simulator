# QX simulator

[![CI](https://github.com/QuTech-Delft/qx-simulator/workflows/Test/badge.svg)](https://github.com/qutech-delft/qx-simulator/actions)
[![cpp-linter](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml)
[![PyPI](https://badgen.net/pypi/v/qxelarator)](https://pypi.org/project/qxelarator/)
![OS](https://img.shields.io/badge/os-linux%20%7C%20macos%20%7C%20windows-blue?style=flat-square)
[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)

QX simulator is a simulator of cQASM programs, developed by QuTech.
At the moment, QX simulator only supports cQASM v3.0 programs
(see [cQASM-spec](https://qutech-delft.github.io/cQASM-spec/latest/) for the language specification).

It performs simulation of an input program received via a file or a string.
It produces one of the following results:

- A simulation result: a quantum state and, optionally, a list of measurements.
- An error. In case the input program was malformed or the simulation failed.

It can be used:

- As a standalone executable (built from C++), from the command line.
- As a module (called `qxelarator`), from Python projects (as a [Python package](https://pypi.org/project/libqasm/)).

## Getting started

Given a cQASM program `example.cq`.

```cQASM
version 3.0

qubit[2] q
bit[2] b

H q[0]
CNOT q[0], q[1]
b = measure q
```

We can simulate this circuit using QX simulator:

### Command line

```shell
./qx-simulator -c 1000 -j simulation_result.json ../tests/circuits/bell_pair.cq
```

### Python

```python
import qxelarator

if __name__ == "__main__":
    qxelarator.execute_file('example.cq', iterations=1000)
```

## Documentation

The [QX simulator documentation](https://QuTech-Delft.github.io/qx-simulator/) is hosted through GitHub Pages.

## License

QX simulator is licensed under the Apache License, Version 2.0.
See [LICENSE](https://github.com/QuTech-Delft/libqasm/blob/master/LICENSE.md) for the full license text.

## Authors

Quantum Inspire: [support@quantum-inspire.com](mailto:"support@quantum-inspire.com")
