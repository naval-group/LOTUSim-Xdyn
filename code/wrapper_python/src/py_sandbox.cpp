#include "py_sandbox.hpp"
#include "py_pybind_additions.hpp"

namespace py = pybind11;

void py_add_module_sandbox(py::module& m0)
{
    py::module m = m0.def_submodule("sandbox");
    m.def("add",
        [](int a, boost::optional<int> b) {
            //boost::optional -> if (!b.is_initialized()) {
            //C++17 std::optional -> if (!b.has_value()) {
            if (!b.is_initialized()) {
                return a;
            } else {
                return a + b.get();
            }
        },
        py::arg("a"),
        py::arg("b") = py::none()
    );
}
