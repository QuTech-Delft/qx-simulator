%define DOCSTRING
"`qxelarator` Python interface to QX simulator as an accelerator."
%enddef

%module(docstring=DOCSTRING) qxelarator

%include "std_string.i"

%typemap(in) std::optional<std::uint_fast64_t> {
    if($input == Py_None) {
        $1 = std::nullopt;
    } else {
        $1 = PyLong_AsLong($input);
    }
}

// Map the output of execute_string/execute_file to a simple Python class for user-friendliness.
%typemap(out) std::variant<qx::SimulationResult, qx::SimulationError> {
    if (std::holds_alternative<qx::SimulationResult>($1)) {
        auto pmod = PyImport_ImportModule("qxelarator");
        auto pclass = PyObject_GetAttrString(pmod, "SimulationResult");
        Py_DECREF(pmod);

        auto simulationResult = PyObject_CallObject(pclass, NULL);
        Py_DECREF(pclass);

        auto const* cppSimulationResult = std::get_if<qx::SimulationResult>(&$1);

        PyObject_SetAttrString(simulationResult, "shots_done", PyLong_FromUnsignedLongLong(cppSimulationResult->shotsDone));
        PyObject_SetAttrString(simulationResult, "shots_requested", PyLong_FromUnsignedLongLong(cppSimulationResult->shotsRequested));

        auto results = PyDict_New();
        for(auto const& [state, count]: cppSimulationResult->results) {
            PyDict_SetItemString(results, state.c_str(), PyLong_FromUnsignedLongLong(count));
        }
        PyObject_SetAttrString(simulationResult, "results", results);

        auto state = PyDict_New();
        for(auto const& [value, amplitude]: cppSimulationResult->states) {
            PyDict_SetItemString(state, value.c_str(), PyComplex_FromCComplex({ .real = amplitude.real, .imag = amplitude.imag }));
        }
        PyObject_SetAttrString(simulationResult, "state", state);

        $result = simulationResult;
    } else {
        auto pmod = PyImport_ImportModule("qxelarator");
        auto pclass = PyObject_GetAttrString(pmod, "SimulationError");
        Py_DECREF(pmod);

        auto errorString = PyUnicode_FromString(std::get_if<qx::SimulationError>(&$1)->what());
        auto args = PyTuple_Pack(1, errorString);

        auto simulationError = PyObject_CallObject(pclass, args);
        Py_DECREF(args);
        Py_DECREF(pclass);
        Py_DECREF(errorString);

        $result = simulationError;
    }
}

%{
#include "qx/Qxelarator.hpp"
%}

// Include the header file with above prototypes
%include "qx/Qxelarator.hpp"
