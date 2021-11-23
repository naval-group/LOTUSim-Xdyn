#include "py_sandbox.hpp"
#include "py_ssc.hpp"
#include "py_xdyn_core.hpp"
#include "py_xdyn_data.hpp"
#include "py_xdyn_env.hpp"
#include "py_xdyn_hdb.hpp"
#include "py_xdyn_force.hpp"
#include "py_pybind_additions.hpp"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(xdyn, m) {
    m.doc() = R"pbdoc(
        xdyn
        ----

        .. currentmodule:: xdyn

        .. autosummary::
           :toctree: _generate
    )pbdoc";

    py::add_ostream_redirect(m, "ostream_redirect");

    py_add_module_sandbox(m);
    py_add_module_ssc(m);
    py_add_module_xdyn_core(m);
    py_add_module_xdyn_data(m);
    py_add_module_xdyn_env(m);
    py_add_module_xdyn_hdb(m);
    py_add_module_xdyn_force(m);

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
