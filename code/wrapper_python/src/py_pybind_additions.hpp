#ifndef PY_PYBIND_ADDITIONS_HPP
#define PY_PYBIND_ADDITIONS_HPP

#include <pybind11/pybind11.h>
#include <pybind11/cast.h>
#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include <boost/optional.hpp>

// Code to handle boost::optional usage in force_models
// When C++ 17 standard will be used, we will be able to replace this std::optional
namespace pybind11 { namespace detail {
    template <typename T>
    struct type_caster<boost::optional<T>> : optional_caster<boost::optional<T>> {};
}}

#include <Eigen/Dense>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#endif