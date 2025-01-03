`execute_string` and `execute_file` return either a `SimulationResult` or a `SimulationError`.
These are actually Python classes that work as bindings for accessing C++ code from Python.
They are defined in `python/qxelarator/__init__.py`.

::: SimulationResult
    handler: python

::: SimulationError
    handler: python
