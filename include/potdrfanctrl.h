#ifndef POTDRFANCTRL_H
#define POTDRFANCTRL_H

#include "register.h"


class PotdrFanCtrl : public LogicBlock
{
public:
    PotdrFanCtrl(LogicBlock *potdr, uint32_t offset, std::string name = "Fan controller");

public:
        Register &r_pwm_cmd;
        Register &r_curr_temp1;
        Register &r_curr_temp2;
        Register &r_threshold_1_temp_1;
        Register &r_threshold_2_temp_1;
        Register &r_threshold_3_temp_1;
        Register &r_threshold_1_temp_2;
        Register &r_threshold_2_temp_2;
        Register &r_threshold_3_temp_2;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrFanCtrl, PYBIND11_TOSTRING(x), r_##x)

public:
    static void add_class(pybind11::module &m)
    {
        py::class_<PotdrFanCtrl>(m, "PotdrFanCtrl")
                .def("__repr__", &PotdrFanCtrl::__repr__)
                .def("dump", &PotdrFanCtrl::dump, py::arg("level") = 0)
                .add_register(pwm_cmd)
                .add_register(curr_temp1)
                .add_register(curr_temp2)
                .add_register(threshold_1_temp_1)
                .add_register(threshold_2_temp_1)
                .add_register(threshold_3_temp_1)
                .add_register(threshold_1_temp_2)
                .add_register(threshold_2_temp_2)
                .add_register(threshold_3_temp_2)
                ;
    }
#undef add_register

#endif
};

#endif // POTDRFANCTRL_H
