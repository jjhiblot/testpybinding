#ifndef POTDRADCSPICONF_H
#define POTDRADCSPICONF_H

#include "register.h"

class PotdrSpi : public LogicBlock
{
public:
    PotdrSpi(LogicBlock *bdt, uint32_t offset, std::string name);
public:
    Register &r_configuration;
    Register &r_status;
    Register &r_direct_request;
    Register &r_last_read;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrSpi, PYBIND11_TOSTRING(x), r_##x)
public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<PotdrSpi>(m, "PotdrSpi")
                .def("__repr__", &PotdrSpi::__repr__)
                .def("dump", &PotdrSpi::dump, py::arg("level") = 0)
                .add_register(configuration)
                .add_register(status)
                .add_register(direct_request)
                .add_register(last_read)
                ;
    }
#undef add_register
#endif

};

#endif // POTDRADCSPICONF_H
