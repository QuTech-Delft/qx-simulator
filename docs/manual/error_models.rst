============
Error models
============

Error models allow the introduction of probabilistic errors during the execution of the quantum circuit. They are useful for simulating more
realistically a real quantum computer.

The error model is specified by adding an ``error_model`` statement in the cQasm input, followed by the model name and its parameters.

The only supported error model is currently the depolarizing channel.


Depolarizing channel
--------------------

This model is implemented as described in *Quantum Computation and Quantum Information* by Nielsen & Chuang.
Note that the book uses the density matrix/quantum computation formalism, while currently QX-simulator only uses
state vector simulation.

This model is parametrized by a probability of error **p**.
Between each gate of the circuit, an error on a uniformly randomly chosen qubit is applied with probability **p**.
The error is uniformly a **X** (bit-flip), **Y** or **Z** (phase-flip) gate.

For instance, take the following circuit:

::

    version 1.0
    qubits 2

    error_model depolarizing_channel, 0.0001

    h q[0]

    measure_all


When simulated 100000 times, it can yield:

::

    Measurement register averaging
    00       50000/100000 (0.50000000)
    01       49994/100000 (0.49994000)
    10       4/100000 (0.00004000)
    11       2/100000 (0.00002000)
