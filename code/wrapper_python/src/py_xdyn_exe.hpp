#ifndef PY_XDYN_EXE_HPP
#define PY_XDYN_EXE_HPP

#include <pybind11/pybind11.h>

namespace py = pybind11;

void py_add_module_xdyn_exe(py::module& m);

#endif