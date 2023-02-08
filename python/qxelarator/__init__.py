# Author Imran Ashraf


# The import syntax changes slightly between python 2 and 3, so we
# need to detect which version is being used:
from sys import version_info
if version_info[0] < 3:
    raise EnvironmentError("sys.version_info refers to a version of "
        "Python is not 3. This is not permitted. "
        "sys.version_info = {}".format(version_info))

class SimulationResult:
    def __init__(self):
        self.shots_requested = 0
        self.shots_done = 0
        self.results = {}
        self.state = {}

    def __repr__(self):
        return f"""Shots requested: {self.shots_requested}
Shots done: {self.shots_done}
Results: {self.results}
State: {self.state}"""

class SimulationError:
    def __init__(self, message):
        self.message = message

    def __repr__(self):
        return f"Quantum simulation error: {self.message}"

from .qxelarator import *