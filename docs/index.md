QX simulator is a simulator of cQASM programs, developed by QuTech.
At the moment, QX simulator only supports cQASM v3.0 programs
(see [cQASM-spec](https://qutech-delft.github.io/cQASM-spec/latest/) for the language specification).

It performs simulations of input programs received via file or string.
It produces one of the following results:

- A simulation result: a quantum state and, optionally, a list of measurements.
- An error. In case of an invalid input program or failed simulation.

It can be used as:

- A standalone executable (built from C++), from the command line.
- A module (called `qxelarator`), from Python projects (as a [Python package](https://pypi.org/project/libqasm/)).

## cQASM v3.0 specification coverage

QX simulator has the following shortcomings regarding to the current cQASM v3.0 specification:

- Power gate modifier can only be used with integer exponents, not with fractional (rational or irrational) ones.
