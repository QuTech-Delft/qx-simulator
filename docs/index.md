QX simulator is a simulator of cQASM programs, developed by QuTech.
At the moment, QX simulator only supports cQASM v3.0 programs
(see [cQASM-spec](https://qutech-delft.github.io/cQASM-spec/latest/) for the language specification).

It performs simulation of an input program received via a file or a string.
It produces one of the following results:

- A simulation result: a quantum state and, optionally, a list of measurements.
- An error. In case the input program was malformed or the simulation failed.

It can be used:

- As a standalone executable (built from C++), from the command line.
- As a module (called `QXelarator`), from Python projects (as a [Python package](https://pypi.org/project/libqasm/)).
