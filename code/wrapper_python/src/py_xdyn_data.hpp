#ifndef PY_XDYN_DATA_HPP
#define PY_XDYN_DATA_HPP

#include <pybind11/pybind11.h>

namespace py = pybind11;

void py_add_module_xdyn_data(py::module& m);

#endif