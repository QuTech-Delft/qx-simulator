QX simulator can be installed as a Python package:

```shell
pip install qxelarator
```

And then imported from a Python program.

## How to run the simulator

### execute_string

The simplest way to simulate a cQASM program is using the `execute_string` method:

```
>>> import qxelarator

>>> result = qxelarator.execute_string("version 3.0; qubit[2] q; H q[0]")

>>> result
Shots requested: 1
Shots done: 1
Measurements: {'00': 1}
State: {'00': (0.7071067811865475+0j), '01': (0.7071067811865475+0j)}
```

### execute_file

The cQASM program can also be retrieved from a file:

```
>>> qxelarator.execute_file("bell_pair.cq")
Shots requested: 1
Shots done: 1
Measurements: {'11': 1}
State: {'11': (1+0j)}
```

## Simulation output

### SimulationResult

If the simulation succeeds, it will return a `SimulationResult` object, which offers access to:
- the final quantum state, and 
- the aggregated measurement register.

```
>>> isinstance(result, qxelarator.SimulationResult)
True

>>> result.state["00"]
(0.7071067811865475+0j)

>>> result.measurements
{'00': 1}
```

### SimulationError

In case the simulation fails, a `qxelarator.SimulationError` object is returned:

```
>>> r = qxelarator.execute_string("version 3.0; qubit[2] q; H q[0")
>>> r
Quantum simulation error: cQASM v3 analyzer returned errors:
Error at <unknown file name>:1:31: missing ']' at '<EOF>'

>>> isinstance(r, qxelarator.SimulationError)
True
```

## Simulation parameters

### Iterations

To simulate a quantum circuit multiple times, a number of `iterations` can be specified:

```
>>> qxelarator.execute_file("bell_pair.cq", iterations=10)
Shots requested: 10
Shots done: 10
Measurements: {'00': 3, '11': 7}
State: {'11': (1+0j)}
```

### Seed

By default, QX simulator will generate different random numbers for different executions of a given circuit.
This means that `measure`, `reset`, and error models will make simulation results non-deterministic.

In some cases this is not desired.
To make the output of the simulator deterministic over different runs, you can pass a constant `seed` parameter:

```python
qxelarator.execute_string("version 3.0; qubit[2] q; bit[2] b; H q[0]; b = measure q", iterations=1000, seed=123)
```
