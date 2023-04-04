#ifndef POTDRLASERCTRL_H
#define POTDRLASERCTRL_H

#include "register.h"

#ifdef PYTHON_BINDINGS
#include <pybind11/pybind11.h>
namespace py = pybind11;
#endif

class PotdrLaserCtrl: public LogicBlock
{
public:
    PotdrLaserCtrl(LogicBlock *bdt, uint32_t offset, std::string name = "Laster Ctrl");
public:
    Register &r_laser_width_ent;
    Register &r_laser_width_dec;
    Register &r_laser_enable;
    Register &r_laser_source_enable;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrLaserCtrl, PYBIND11_TOSTRING(x), r_##x)

public:
    static void add_class(pybind11::module &m)
    {
        py::class_<PotdrLaserCtrl>(m, "PotdrLaserCtrl")
                .def("__repr__", &PotdrLaserCtrl::__repr__)
                .def("dump", &PotdrLaserCtrl::dump, py::arg("level") = 0)
                .add_register(laser_width_ent)
                .add_register(laser_width_ent)
                .add_register(laser_width_dec)
                .add_register(laser_enable)
                .add_register(laser_source_enable)
                ;
    }
#undef add_register

#endif

};


#endif // POTDRLASERCTRL_H
