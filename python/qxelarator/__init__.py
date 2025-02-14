# Author Imran Ashraf


# The import syntax changes slightly between python 2 and 3, so we
# need to detect which version is being used:
from sys import version_info
if version_info[0] < 3:
    raise EnvironmentError("sys.version_info refers to a version of "
        "Python is not 3. This is not permitted. "
        "sys.version_info = {}".format(version_info))

class SimulationResult:
    """! A simulation result object."""

    def __init__(self):
        """! Default constructs a SimulationResult."""

        ## @var int shots_requested
        # Number of iterations the simulation was requested to be performed.
        self.shots_requested = 0
        ## @var int shots_done
        # Number of iterations the simulation was actually performed.
        self.shots_done = 0
        ## @var dict results
        # Contains the number of times a given measurement value is captured when running the iterations.
        self.results = {}
        ## @var dict state
        # Contains the full quantum state at the end of the very last iteration.
        self.state = {}

    def __repr__(self):
        """! Returns a string representation of a SimulationResult."""

        return f"""Shots requested: {self.shots_requested}
Shots done: {self.shots_done}
Measurements: {self.results}
State: {self.state}"""

class SimulationError:
    """! A simulation error object, containing just an error message."""

    def __init__(self, message):
        """! Constructs a SimulationError."""

        ## @var str message
        # Error message.
        self.message = message

    def __repr__(self):
        """! Returns a string representation of an error message."""

        return f"Quantum simulation error: {self.message}"

from .qxelarator import *
