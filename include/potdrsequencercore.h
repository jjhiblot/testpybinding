#ifndef POTRSEQUENCERCORE_H
#define POTRSEQUENCERCORE_H

#include "drvwrapper.h"
#include "register.h"


#ifdef PYTHON_BINDINGS
#include <pybind11/pybind11.h>
namespace py = pybind11;
#endif

class PotdrSequencerCore: public LogicBlock
{
public:
    PotdrSequencerCore(LogicBlock *bdt, uint32_t offset, std::string name = "Sequencer Core");

public:
    Register &r_teeth;
    Register &r_laser_delay;
    Register &r_masq_duration;
    Register &r_wait_duration;
    Register &r_noise_duration;
    Register &r_average_number;
    Register &r_ramp_incr;
    Register &r_ramp_reset_val;
    Register &r_comb_number;
    Register &r_comb_incr;
    Register &r_misc;
    Register &r_hw_cfg;
    Register &r_total_teeth;
    Register &r_force_comb_id;
    Register &r_recure_prime_number;
    Register &r_state_debug;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrSequencerCore, PYBIND11_TOSTRING(x), r_##x)
public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<PotdrSequencerCore>(m, "PotdrSequencerCore")
                .def("__repr__", &PotdrSequencerCore::__repr__)
                .def("dump", &PotdrSequencerCore::dump, py::arg("level") = 0)
                .add_register(teeth)
                .add_register(laser_delay)
                .add_register(masq_duration)
                .add_register(wait_duration)
                .add_register(noise_duration)
                .add_register(average_number)
                .add_register(ramp_incr)
                .add_register(ramp_reset_val)
                .add_register(comb_number)
                .add_register(comb_incr)
                .add_register(misc)
                .add_register(hw_cfg)
                .add_register(total_teeth)
                .add_register(force_comb_id)
                .add_register(recure_prime_number)
                .add_register(state_debug)
                ;
    }
#undef add_register
#endif
};

#endif // POTRSEQUENCERCORE_H
