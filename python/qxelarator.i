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
%typemap(out) std::variant<std::monostate, qx::SimulationResult, qx::SimulationError> {
    if (std::holds_alternative<qx::SimulationResult>($1)) {
        auto pmod = PyImport_ImportModule("qxelarator");
        auto pclass = PyObject_GetAttrString(pmod, "SimulationResult");
        Py_DECREF(pmod);

        auto simulation_result = PyObject_CallObject(pclass, NULL);
        Py_DECREF(pclass);

        auto const* cpp_simulation_result = std::get_if<qx::SimulationResult>(&$1);

        PyObject_SetAttrString(simulation_result, "shots_done", PyLong_FromUnsignedLongLong(cpp_simulation_result->shots_done));
        PyObject_SetAttrString(simulation_result, "shots_requested", PyLong_FromUnsignedLongLong(cpp_simulation_result->shots_requested));

        auto results = PyDict_New();
        for(auto const& [state_string, count]: cpp_simulation_result->measurements) {
            PyDict_SetItemString(results, state_string.c_str(), PyLong_FromUnsignedLongLong(count));
        }
        PyObject_SetAttrString(simulation_result, "results", results);

        auto state = PyDict_New();
        for(auto const& [state_string, amplitude]: cpp_simulation_result->state) {
            PyDict_SetItemString(state, state_string.c_str(), PyComplex_FromCComplex({ .real = amplitude.real, .imag = amplitude.imag }));
        }
        PyObject_SetAttrString(simulation_result, "state", state);

        $result = simulation_result;
    } else {
        auto pmod = PyImport_ImportModule("qxelarator");
        auto pclass = PyObject_GetAttrString(pmod, "SimulationError");
        Py_DECREF(pmod);

        auto error_string = PyUnicode_FromString(std::get_if<qx::SimulationError>(&$1)->what());
        auto args = PyTuple_Pack(1, error_string);

        auto simulation_error = PyObject_CallObject(pclass, args);
        Py_DECREF(args);
        Py_DECREF(pclass);
        Py_DECREF(error_string);

        $result = simulation_error;
    }
}

%{
#include "qx/qxelarator.hpp"
%}

// Include the header file with above prototypes
%include "qx/qxelarator.hpp"
