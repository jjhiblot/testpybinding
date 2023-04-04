#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "drvwrapper.h"
#include "potdr.h"

namespace py = pybind11;


PYBIND11_MODULE(pypotdr, m) {
    m.doc() =  R"pbdoc(
        Python bindings for the devpotdr library
    )pbdoc";

    PotdrLaserCtrl::add_class(m);
    PotdrSequencerCore::add_class(m);
    PotdrAccumulatorCore::add_class(m);
    PotdrPowermeter::add_class(m);
    PotdrSpi::add_class(m);
    PotdrDACSource::add_class(m);
    PotdrTimeBase::add_class(m);
    PotdrFanCtrl::add_class(m);
    Potdr::add_class(m);
    DrvWrapper::add_class(m);

    py::class_<Register>(m, "Register")
            .def("read", [](Register& m) {return m.read();})
            .def("write", [](Register& m, uint32_t val) {return m.write(val);})
            .def("__repr__", [](Register& m) {return m.__repr__();})
            .def("__int__", [](Register& m) {return m.read();})
            .def("__str__", [](Register& m) {return m.__str__();});

    py::class_<RegisterSet>(m, "RegisterSet")
            .def("define", &RegisterSet::define)
            .def("__getitem__", [](RegisterSet& m, uint32_t offset) {return &m.get_by_offset(offset);} , py::return_value_policy::reference)
            .def("__getitem__", [](RegisterSet& m, std::string name) {return &m.get_by_name(name);} , py::return_value_policy::reference)
            .def("dump", &RegisterSet::dump, py::arg("level") = 0);

    py::class_<RegisterFactory, RegisterSet>(m, "RegisterFactory");

    py::class_<LogicBlock>(m, "RegisterBlock")
            .def("dump", &LogicBlock::dump, py::arg("level") = 0);

}
