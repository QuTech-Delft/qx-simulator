======
Output
======

Contents of the output
----------------------

As described in :ref:`usage`, the simulator can output to stdout (C++ executable) and as a ``SimulationResult`` object (Python API).
Those all contain the same data.

Let's simulate a bell pair 100 times:

::

    version 1.0
    qubits 2

    h q[0] 
    cnot q[0], q[1]


This will result in:

::

    Shots requested: 100
    Shots done: 100
    Results: {'00': 100}
    State: {'00': (0.7071067811865475+0j), '11': (0.7071067811865475+0j)}


- ``Shots requested`` and ``Shots done`` are always equal to the number of iterations.
- The ``Results`` section contains how many times a given measurement register is captured when running the iterations. In this case, the circuit doesn't contain any measurement, and therefore the measurement registers are always 00, and this occurred 100 times.
- The ``State`` section contains the full quantum state at the end of the very last iteration. It maps quantum kets to complex amplitudes. Here you can recognize
the usual bell pair state: ``1/sqrt(2) ( |00> + |11> )``.


Let's add measurements:

::

    h q[0] 
    cnot q[0], q[1]
    measure q[0:1]


The result is now:

::

    Shots requested: 100
    Shots done: 100
    Results: {'00': 49, '11': 51}
    State: {'00': (0.9999999999999998+0j)}


You can there notice that the final quantum state is collapsed to "00", because of the measurements. The ket ``|00>`` doesn't have amplitude exactly 1 because of the
approximation of real numbers done by floating point arithmetic inside the simulator, something to keep in mind when interpreting the results.
Again, the ``State`` section is the quantum state at the end of the 100th iteration. Some other iterations ended up in the state ``|11>``.

The ``Results`` section can here be interpreted as: in 49 iterations out of 100, the final measurement register was "00" and in the remaining 51 iterations
it was "11".


Iterating a simulation
----------------------

For those circuits that contain only final measurements, running multiple iterations and aggregating measuremeng registers in the ``Results`` section is not very useful, since you can directly obtain the
probability of each measurement by taking the squared modulus of the complex numbers in the ``State`` section.

However, the ``Results`` section takes all its meaning when using for instance conditional gates, since then the quantum state varies per iteration.
For example:

::
    
    version 1.0
    qubits 3

    h q[0]
    measure q[0]
    cond(b[0]) h q[1]
    measure q[1]
    cond(b[1]) x q[2]


When simulating this circuit, the final quantum state in the ``State`` section is non-deterministic. However, the aggregated measurement register is very useful and the ratios like
``results["001"] / shots_done`` converge as the number of iterations grow.
For 100 iterations:

::

    Shots requested: 100
    Shots done: 100
    Results: {'000': 48, '001': 27, '011': 25}
    State: {'000': (0.9999999999999998+0j)}

For 10000 iterations:

::

    Shots requested: 10000
    Shots done: 10000
    Results: {'000': 4995, '001': 2517, '011': 2488}
    State: {'001': (0.9999999999999998+0j)}