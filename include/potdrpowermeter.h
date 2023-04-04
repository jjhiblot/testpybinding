#ifndef POTDRPOWERMETER_H
#define POTDRPOWERMETER_H

#include "register.h"

class PotdrPowermeter : public LogicBlock
{
public:
    PotdrPowermeter(LogicBlock *bdt, uint32_t offset, int idx = 0, std::string name = "Power Meter");
public:
    Register &r_cmd_dark_g1;
    Register &r_cmd_dark_g2;
    Register &r_cmd_dark_g3;
    Register &r_cmd_dark_g4;
    Register &r_value_min_g1;
    Register &r_value_min_g2;
    Register &r_value_max_g2;
    Register &r_value_min_g3;
    Register &r_value_max_g3;
    Register &r_value_max_g4;
    Register &r_it_rate;
    Register &r_laser_freq;
    Register &r_nb_avg_pwm;
    Register &r_nb_avg_zero_auto;
    Register &r_data_zero_auto_high;
    Register &r_data_zero_auto_low;
    Register &r_pmw_tr_avg_high;
    Register &r_pmw_tr_avg_low;
    Register &r_cmd_freq;
    Register &r_cmd_pwm;
    Register &r_info_gain;
    Register &r_half_period;
    Register &r_period;
    Register &r_half_period_mem;
    Register &r_period_mem;
    Register &r_avg_level;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrPowermeter, PYBIND11_TOSTRING(x), r_##x)
public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<PotdrPowermeter>(m, "PotdrPowermeter")
                .def("__repr__", &PotdrPowermeter::__repr__)
                .def("dump", &PotdrPowermeter::dump, py::arg("level") = 0)
                .add_register(cmd_dark_g1)
                .add_register(cmd_dark_g2)
                .add_register(cmd_dark_g3)
                .add_register(cmd_dark_g4)
                .add_register(value_min_g1)
                .add_register(value_min_g2)
                .add_register(value_max_g2)
                .add_register(value_min_g3)
                .add_register(value_max_g3)
                .add_register(value_max_g4)
                .add_register(it_rate)
                .add_register(laser_freq)
                .add_register(nb_avg_pwm)
                .add_register(nb_avg_zero_auto)
                .add_register(data_zero_auto_high)
                .add_register(data_zero_auto_low)
                .add_register(pmw_tr_avg_high)
                .add_register(pmw_tr_avg_low)
                .add_register(cmd_freq)
                .add_register(cmd_pwm)
                .add_register(info_gain)
                .add_register(half_period)
                .add_register(period)
                .add_register(half_period_mem)
                .add_register(period_mem)
                .add_register(avg_level)
                ;
    }
#undef add_register
#endif

};

#endif // POTDRPOWERMETER_H
