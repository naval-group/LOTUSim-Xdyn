#include "py_ssc.hpp"
#include "py_xdyn_core.hpp"
#include "py_xdyn_exe.hpp"
#include "py_xdyn_data.hpp"
#include "py_xdyn_env.hpp"
#include "py_xdyn_hdb.hpp"
#include "py_xdyn_force.hpp"
#include "py_pybind_additions.hpp"

namespace py = pybind11;

PYBIND11_MODULE(xdyn, m) {
    m.doc() = R"pbdoc(
        xdyn
        ----

        .. currentmodule:: xdyn

        .. autosummary::
           :toctree: _generate
    )pbdoc";
    py_add_module_xdyn_exe(m);
    py_add_module_ssc(m);
    py_add_module_xdyn_core(m);
    py_add_module_xdyn_data(m);
    py_add_module_xdyn_env(m);
    py_add_module_xdyn_hdb(m);
    py_add_module_xdyn_force(m);

#ifdef GIT_VERSION
    m.attr("__version__") = GIT_VERSION;
#else
    m.attr("__version__") = "dev";
#endif
}
