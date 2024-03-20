.. _usage:

=====
Usage
=====


Running the simulator from Python
---------------------------------

Python API
~~~~~~~~~~


The most straightforward way to execute a cQasm file is using the ``qxelarator.execute_string`` method:

.. code-block:: pycon

    >>> import qxelarator
    >>> r = qxelarator.execute_string("version 1.0;qubits 2;h q[0]")
    >>> r
    Shots requested: 1
    Shots done: 1
    Results: {'00': 1}
    State: {'00': (0.7071067811865475+0j), '01': (0.7071067811865475+0j)}


The return value is a ``qxelarator.SimulationResult`` object and offers access to aggregated measurement register results and final quantum state:

.. code-block:: pycon

    >>> isinstance(r, qxelarator.SimulationResult)
    True
    >>> r.results
    {'00': 1}
    >>> r.state["00"]
    (0.7071067811865475+0j)


You can also execute a cQasm file:

.. code-block:: pycon

    >>> qxelarator.execute_file("bell_pair.qc")
    Shots requested: 1
    Shots done: 1
    Results: {'11': 1}
    State: {'11': (1+0j)}


In case the parsing/analysis of the cqasm string or file fails, you end up with a ``qxelarator.SimulationError`` object:

.. code-block:: pycon

    >>> r = qxelarator.execute_string("version 1.0;qubits 2;h q[0")
    >>> r
    Quantum simulation error: Cannot parse and analyze cQASM: 
    <unknown>:1:27: syntax error, unexpected end of file, expecting ',' or ']'
    Failed to parse <unknown>
    
    >>> isinstance(r, qxelarator.SimulationError)
    True


To simulate a quantum circuit multiple times, pass an integer number of iterations to e.g. ``execute_file``:

.. code-block:: pycon

    >>> qxelarator.execute_file("bell_pair.qc", iterations = 10)
    Shots requested: 10
    Shots done: 10
    Results: {'00': 3, '11': 7}
    State: {'11': (1+0j)}


To make sense of the output of QX-simulator, please visit :ref:`Output`

Using a constant seed for random number generation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

By default QX-simulator will generate different random numbers for different executions of a given circuit.
This means that ``measure``, ``prep_z`` and error models will make simulation results non-deterministic.

In some cases this is not desired. To make the output of the simulator deterministic over different runs, you can pass a constant ``seed`` parameter:

.. code-block:: python

    qxelarator.execute_string("version 1.0;qubits 2;h q[0];measure_all", iterations=1000, seed=123)


Running the binary built from source
------------------------------------

The following will result in the same runs using the executable binary instead of the Python package:

.. code-block:: bash

    ./qx-simulator -c 1000 ../tests/circuits/bell_pair.qc