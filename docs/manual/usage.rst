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

It contains the following cQasm circuit:

::
    
    qubits 2
    

    .init
    prep_z q[0]
    prep_z q[1]

    .entangle
    h q[0] 
    cnot q[0],q[1]

    .measurement
    measure q[0]
    measure q[1]

Should the parsing or analysis of the file fail, the above call will return ``false`` and print an error.
If everything went well, you can execute the circuit (and obtain a json string with the output):

::

    >>> json_string = qx.execute()

This will print the resulting final quantum state as well as the resulting measurement register:

::

    >>> json_string = qx.execute()
    [QXELERATOR] Creating quantum register of 2 qubits...
    [QXELERATOR] Loaded 3 circuits.
    [+] executing circuit 'init' (1 iter) ...
    [+] circuit execution time: +0.215587 sec.
    [+] executing circuit 'entangle' (1 iter) ...
    [+] circuit execution time: +0.081271 sec.
    [+] executing circuit 'measurement' (1 iter) ...
    [+] circuit execution time: +0.202154 sec.
    -------------------------------------------
    Final quantum state
    11       1.0000 + 0.0000 * i (1.0000)

    Measurement register averaging
    11       1/1 (1.0000)

Since the circuit contains final measurements for all qubits, the quantum state is collapsed. If you execute the above command multiple times, you can notice
that the final quantum state changes between runs, non-deterministically.

When removing the measurement, you obtain a fixed quantum state:

::

    Final quantum state
    00       0.7071 + 0.0000 * i (0.5000)
    11       0.7071 + 0.0000 * i (0.5000)

    Measurement register averaging
    00       1/1 (1.0000)

The quantum state shows as expected superposition of the two qubits in the bell state.
Notice how the measurement register is then always 0, even if you repeat the execution.

Add back the measurement and you can leverage measurement register averaging, by passing an integer number of iterations to ``execute()``:

::

    >>> qx.execute(100)
    [QXELERATOR] Creating quantum register of 2 qubits...
    [QXELERATOR] Loaded 3 circuits.
    -------------------------------------------
    Final quantum state
    00       1.0000 + 0.0000 * i (1.0000)

    Measurement register averaging
    00       54/100 (0.4500)
    11       46/100 (0.5500)

The quantum state is less relevant here, since it displays the very final quantum state after the last run of the 100 executions.
The above "Measurement register averaging" section means that out of 100 executions of the bell pair circuit, the measurement register had a final state of "00" in 54 cases,
and "11" in 46 cases. While this can be deducted from the final quantum state (and probabilities) obtained earlier on an uncollapsed state, this way to simulate circuits becomes
way more interesting for non-deterministic circuits, that is, quantum program with mid-circuit measurements and conditional gates.

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
    Final quantum state
    001       1.0000 + 0.0000 * i (1.0000)

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
        },
        "state": {
            "001": {
                "real": 1.000000,
                "imag": 0.000000
            }
        }
    }

Note: ``shots_done`` will always equal to ``shots_requested``. These two keys are distinct so that hardware backends can output the same JSON keys and optionally
notify that they have failed to execute all requested runs.

Running the binary built from source
------------------------------------

The following will result in the same runs using the executable binary instead of the Python package:

::

    ./qx-simulator -c 100 -j simulation_result.json ../tests/circuits/bell_pair.qc