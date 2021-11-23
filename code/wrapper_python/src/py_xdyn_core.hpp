#ifndef PY_XDYN_CORE_HPP
#define PY_XDYN_CORE_HPP

#include <pybind11/pybind11.h>

namespace py = pybind11;

void py_add_module_xdyn_core(py::module& m);

#endif