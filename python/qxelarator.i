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
    $result = qxelarator::python::to_python($1);
    if (!$result) {
        SWIG_fail;
    }
}

%{
#include "qx/qxelarator.hpp"

namespace qxelarator::python {

PyObject* make_instance(const char* class_name, PyObject* args = nullptr) {
    PyObject* module = PyImport_ImportModule("qxelarator");
    if (!module) {
        return nullptr;
    }

    PyObject* class_object = PyObject_GetAttrString(module, class_name);
    Py_DECREF(module);
    if (!class_object) {
        return nullptr;
    }

    PyObject* instance = PyObject_CallObject(class_object, args);
    Py_DECREF(class_object);
    return instance;
}

bool set_attribute(PyObject* object, const char* name, PyObject* value) {
    if (!value) {
        return false;
    }

    int result = PyObject_SetAttrString(object, name, value);
    Py_DECREF(value);
    return result == 0;
}

PyObject* measurements_to_python(const qx::SimulationResult::Measurements& measurements) {
    PyObject* dictionary = PyDict_New();
    if (!dictionary) {
        return nullptr;
    }

    for (const auto& [state, count] : measurements) {
        PyObject* value = PyLong_FromUnsignedLongLong(count);
        if (!value || PyDict_SetItemString(dictionary, state.c_str(), value) < 0) {
            Py_XDECREF(value);
            Py_DECREF(dictionary);
            return nullptr;
        }
        Py_DECREF(value);
    }

    return dictionary;
}

PyObject* state_to_python(const qx::SimulationResult::State& state) {
    PyObject* dictionary = PyDict_New();
    if (!dictionary) {
        return nullptr;
    }

    for (const auto& [value, amplitude] : state) {
        PyObject* complex_value = PyComplex_FromCComplex({ .real = amplitude.real, .imag = amplitude.imag });
        if (!complex_value || PyDict_SetItemString(dictionary, value.c_str(), complex_value) < 0) {
            Py_XDECREF(complex_value);
            Py_DECREF(dictionary);
            return nullptr;
        }
        Py_DECREF(complex_value);
    }

    return dictionary;
}

PyObject* to_python(const std::variant<std::monostate, qx::SimulationResult, qx::SimulationError>& result) {
    if (const auto* simulation_result = std::get_if<qx::SimulationResult>(&result)) {
        PyObject* instance = make_instance("SimulationResult");
        if (!instance) {
            return nullptr;
        }

        if (!set_attribute(instance, "shots_done", PyLong_FromUnsignedLongLong(simulation_result->shots_done)) ||
            !set_attribute(instance, "shots_requested",
                PyLong_FromUnsignedLongLong(simulation_result->shots_requested)) ||
            !set_attribute(instance, "results", measurements_to_python(simulation_result->measurements)) ||
            !set_attribute(instance, "state", state_to_python(simulation_result->state))) {
            Py_DECREF(instance);
            return nullptr;
        }

        return instance;
    }

    if (const auto* simulation_error = std::get_if<qx::SimulationError>(&result)) {
        PyObject* message = PyUnicode_FromString(simulation_error->what());
        if (!message) {
            return nullptr;
        }

        PyObject* args = PyTuple_Pack(1, message);
        Py_DECREF(message);
        if (!args) {
            return nullptr;
        }

        PyObject* instance = make_instance("SimulationError", args);
        Py_DECREF(args);
        return instance;
    }

    PyErr_SetString(PyExc_RuntimeError, "simulation returned no result");
    return nullptr;
}

}  // namespace qxelarator::python
%}

// Include the header file with above prototypes
%include "qx/qxelarator.hpp"
