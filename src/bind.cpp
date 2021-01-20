#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "VK.h"
#include "Hive.h"

namespace py = pybind11;

PYBIND11_MODULE(reg_parse_v2, m) {
    m.doc() = "A windows registry parsing module for python";

    py::class_<Hive>(m, "Hive")
        .def(py::init<const char*>())
        .def("GetVersion", &Hive::GetVersion)
        .def("GetSubkeys", &Hive::GetSubkeys)
        .def("GetValues", &Hive::GetValues)
        .def("GetValue", &Hive::GetValue)

        .def_property_readonly("major", &Hive::GetMajor)
        .def_property_readonly("minor", &Hive::GetMinor)
        ;

    py::class_<NK, std::shared_ptr<NK>>(m, "NK")
        .def_property_readonly("name", &NK::GetName)
        ;

    py::class_<VK, std::shared_ptr<VK>>(m, "VK")
        .def_property_readonly("name", &VK::GetName)
        .def_property_readonly("type", &VK::GetType)

        .def("__repr__",
            [](VK& a) {
                return L"<value '" + a.GetName() + L"'>";
            })
        ;

    py::enum_<RegType>(m, "RegType", py::arithmetic())
        .value("RegNone", RegType::RegNone)
        .value("RegSz", RegType::RegSz)
        .value("RegExpandSz", RegType::RegExpandSz)
        .value("RegBinary", RegType::RegBinary)
        .value("RegDword", RegType::RegDword)
        .value("RegDwordBigEndian", RegType::RegDwordBigEndian)
        .value("RegLink", RegType::RegLink)
        .value("RegMultiSz", RegType::RegMultiSz)
        .value("RegResourceList", RegType::RegResourceList)
        .value("RegFullResourceDescription", RegType::RegFullResourceDescription)
        .value("RegResourceRequirementsList", RegType::RegResourceRequirementsList)
        .value("RegQWord", RegType::RegQWord)
        .value("RegFileTime", RegType::RegFileTime)
        .export_values();
}