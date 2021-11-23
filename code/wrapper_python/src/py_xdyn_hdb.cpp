#include "py_xdyn_hdb.hpp"
#include "py_pybind_additions.hpp"
#include "hdb_interpolators/inc/History.hpp"
#include "hdb_interpolators/inc/PrecalParser.hpp"
#include "hdb_interpolators/inc/TimestampedMatrix.hpp"

namespace py = pybind11;

void py_add_module_xdyn_hdb(py::module& m)
{
    py::module m_hdb_interpolators = m.def_submodule("hdbinterpolators");

    py::class_<RAOData>(m_hdb_interpolators, "RAOData")
        .def(py::init<>())
        .def_readwrite("periods", &RAOData::periods)
        .def_readwrite("psi", &RAOData::psi)
        .def_readwrite("values", &RAOData::values);
}