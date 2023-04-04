#ifndef DACSOURCE_H
#define DACSOURCE_H

#include "register.h"

class PotdrDACSource : public LogicBlock
{
public:
    PotdrDACSource(LogicBlock *bdt, uint32_t offset, int idx, std::string name = "DAC Source");

public:
    IOMemory &r_data_points;
    Register &r_max_nb_points;
    Register &r_configuration;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrDACSource, PYBIND11_TOSTRING(x), r_##x)
public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<PotdrDACSource>(m, "DACSource")
                .def("__repr__", &PotdrDACSource::__repr__)
                .def("dump", &PotdrDACSource::dump, py::arg("level") = 0)
                .add_register(max_nb_points)
                .add_register(configuration)
                ;
    }
#undef add_register
#endif
};

#endif // DACSOURCE_H
