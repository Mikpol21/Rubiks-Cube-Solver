#include "RubiksCube.h"
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(rubiksCubePy, n)
{
    py::class_<RubiksCube>(n, "RubiksCube", py::buffer_protocol())
        .def(py::init<>())
        .def("rotate", &RubiksCube::rotate, py::arg("face"), py::arg("twice") = false)
        .def("toMatrix", &RubiksCube::toMatrix)
        .def_static("scrambleCube", &RubiksCube::scrambleCube)
        .def_static("scrambleCubeWithTrace", &RubiksCube::scrambleCubeWithTrace)
        .def("scramble", &RubiksCube::scramble)
        .def("scrambleWithTrace", &RubiksCube::scrambleWithTrace)
        .def(py::self == py::self)
        .def("__repr__", &RubiksCube::toString)
        .def_buffer([](RubiksCube &cube) -> py::buffer_info
                    { return py::buffer_info(
                          cube.toMatrix().data(),
                          sizeof(bool),
                          py::format_descriptor<bool>::format(),
                          2,
                          {20, 24},
                          {24 * sizeof(bool), sizeof(bool)}); });
    n.def("printCube", &printCube);
}