#include "py_sandbox.hpp"
#include "py_pybind_additions.hpp"

namespace py = pybind11;

class Animal {
public:
    virtual ~Animal() { }
    virtual std::string go(int n_times) = 0;
};

class Dog : public Animal {
public:
    std::string go(int n_times) override {
        std::string result;
        for (int i=0; i<n_times; ++i)
            result += "woof! ";
        return result;
    }
};

std::string call_go(Animal *animal);
std::string call_go(Animal *animal) {
    return animal->go(3);
}

class PyAnimal : public Animal {
public:
    /* Inherit the constructors */
    using Animal::Animal;

    /* Trampoline (need one for each virtual function) */
    std::string go(int n_times) override {
        PYBIND11_OVERRIDE_PURE(
            std::string, /* Return type */
            Animal,      /* Parent class */
            go,          /* Name of function in C++ (must match Python name) */
            n_times      /* Argument(s) */
        );
    }
};

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

    py::class_<Animal, PyAnimal>(m, "Animal")
        .def(py::init<>())
        .def("go", &Animal::go);

    py::class_<Dog, Animal>(m, "Dog")
        .def(py::init<>());

    m.def("call_go", &call_go);
}
