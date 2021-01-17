#include <pybind11/pybind11.h>

#include "Hive.h"

namespace py = pybind11;

PYBIND11_MODULE(reg_parse_v2, m) {
    m.doc() = "A windows registry parsing module";

    py::class_<Hive>(m, "Hive")
        .def(py::init<const char*>())
        .def("GetVersion", &Hive::GetVersion)
        .def("GetValues", &Hive::GetValues)
        .def("GetValue", &Hive::GetValue)
        ;
}