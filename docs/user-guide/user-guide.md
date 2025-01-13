QX simulator can be used:

- As a standalone executable (built from C++), from the command line.
- As a module (called `qxelarator`), from Python projects (as a [Python package](https://pypi.org/project/libqasm/)).

## Output

The results of a simulation will be dumped to standard output, when running the standalone executable,
or to a `SimulationResult` object, when using the Python API.

Let's simulate a Bell pair 100 times:

```
version 3.0
qubit[2] q

H q[0] 
CNOT q[0], q[1]
```

The output of the simulation should be:

```
Shots requested: 100
Shots done: 100
Measurements: {'00': 100}
State: {'00': (0.7071067811865475+0j), '11': (0.7071067811865475+0j)}
```

- `Shots requested` and `Shots done` are always equal to the number of iterations.
- `Measurements` contains the number of times a given measurement value is captured when running the iterations.
  In this case, the circuit doesn't contain any measurement, so the measurement register is always `00` (100 times).
- `State` contains the full quantum state at the end of the very last iteration.
  It maps quantum _kets_ to complex amplitudes.
  Here you can recognize the usual Bell pair state: `1/sqrt(2) ( |00> + |11> )`.

!!! note

    The number of iterations does not affect the quantum state.
    For  every iteration, the quantum state is reset.
    Thus, the state returned as part of the simulation result is that of the last simulation iteration.

If we now add measurements:

```
H q[0] 
CNOT q[0], q[1]
b = measure q
```

The result will change to:

```
Shots requested: 100
Shots done: 100
Measurements: {'00': 49, '11': 51}
State: {'00': (0.9999999999999998+0j)}
```

Due to the measurements, the state now collapses at the end of each iteration to either $|00\rangle$ or $|11\rangle$.

`Measurements` show that the state collapsed to $|00\rangle$ in 49:100 cases and $|11\rangle$ in 51:100,
so roughly a 50% chance for either outcome.

`State` shows the quantum state at the end of the 100th iteration, in this case to $|00\rangle$.

Note that its amplitude is not exactly 1 because of the approximation of real numbers
done by floating point arithmetic inside the simulator,
something to keep in mind when interpreting the results.

`Measurements` can here be interpreted as: in 49 iterations out of 100,
the final measurement register was `00` and in the remaining 51 iterations it was `11`.
