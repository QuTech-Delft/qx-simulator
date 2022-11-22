=====
Usage
=====


Running the simulator from Python
---------------------------------

Python API
~~~~~~~~~~

Create a simulator instance:

::

    import qxelarator

    qx = qxelarator.QX()


Then, load the cQasm file (e.g. `bell_pair.qc`):

::

    qx.set('bell_pair.qc')

Should the parsing or analysis fail, the above call will return ``false`` and print an error.

If everything went well, you can execute the circuit:

::

    qx.execute()

This will print the resulting final quantum state:

::

    >>> qx.execute()
    [QXELERATOR] Creating quantum register of 2 qubits...
    [QXELERATOR] Loaded 3 circuits.
    [+] executing circuit 'init' (1 iter) ...
    [+] circuit execution time: +0.215587 sec.
    [+] executing circuit 'entangle' (1 iter) ...
    [+] circuit execution time: +0.081271 sec.
    [+] executing circuit 'measurement' (1 iter) ...
    [+] circuit execution time: +0.202154 sec.
    -------------------------------------------
    Complex amplitudes with probabilities
    11       1 + 0 * i (1.000000)

Since the circuit contains final measurement for all qubits, the quantum state is collapsed. When removing the measurement, you obtain as expected:

::

    Complex amplitudes with probabilities
    00       0.707107 + 0 * i (0.500000)
    11       0.707107 + 0 * i (0.500000)

Add back the measurement and you can do measurement register averaging, by passing an integer number of iterations to ``execute()``:

::

    >>> qx.execute(100)
    [QXELERATOR] Creating quantum register of 2 qubits...
    [QXELERATOR] Loaded 3 circuits.
    -------------------------------------------
    Measurement register averaging
    00       54/100 (0.540000)
    11       46/100 (0.460000)

The above output means that out of 100 executions of the bell pair circuit, the measurement register had a final state of "00" in 54 cases,
and "11" in 46 cases. While this can be deducted from the final quantum state (and probabilities) obtained earlier, this way to simulate circuits becomes
way more interesting for non-deterministic circuits, that is, quantum program with mid-circuit measurement and conditional gates.

For example, simulate the following circuit:

::

    qubits 3

    .main
        h q[0]
        measure q[0]
        cond(b[0]) h q[1]
        measure q[1]
        cond(b[1]) x q[2]

and get:

::

    >>> qx.execute(1000)
    [QXELERATOR] Creating quantum register of 3 qubits...
    [QXELERATOR] Loaded 1 circuits.
    -------------------------------------------
    Measurement register averaging
    000       504/1000 (0.504000)
    001       257/1000 (0.257000)
    011       239/1000 (0.239000)


Note: when the circuit does not contain any measure operation, measurement register averaging will as expected return that the measurement register is always completely 0.
Likewise, when some qubits are never measured, their corresponding bit will always be 0 in the measurement register averaging.


JSON output
~~~~~~~~~~~

The API provides a function to set a file to output JSON:

::

    >>> qx.set_json_output_path("simulation_result.json")

After another ``execute(1000)`` call, that JSON output will look like this:

::

    > cat simulation_result.json 
    {
    "info": {
        "shots_requested": 1000,
        "shots_done": 1000
    },
    "results": {
        "000": 0.516000,
        "001": 0.241000,
        "011": 0.243000
    }
    }

Note: ``shots_done`` will always equal to ``shots_requested``. These two keys are distinct so that hardware backends can output the same JSON keys and optionally
notify that they have failed to execute all requested runs.

Running the binary built from source
------------------------------------

The following will result in the same runs using the executable binary instead of the Python package:

::

    ./qx-simulator -c 100 -j simulation_result.json ../tests/circuits/bell_pair.qc