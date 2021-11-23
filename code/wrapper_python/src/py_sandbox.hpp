#ifndef PY_SANDBOX_HPP
#define PY_SANDBOX_HPP

#include <pybind11/pybind11.h>

namespace py = pybind11;

void py_add_module_sandbox(py::module& m);

#endif