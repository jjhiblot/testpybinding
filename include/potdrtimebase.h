#ifndef POTDRTIMEBASE_H
#define POTDRTIMEBASE_H

#include <stdint.h>
#include "register.h"
#include "potdrsequencercore.h"
#include "potdrlaserctrl.h"
#include "potdrspi.h"
#include "potdrpowermeter.h"
#include "potdraccumulatorcore.h"
#include "dacsource.h"

class PotdrTimeBase: public LogicBlock
{
public:
        PotdrTimeBase(LogicBlock *parent, uint32_t offset, int idx, std::string name = "Time Base");
        void analog_power_on(bool up);

public:
        PotdrSequencerCore m_seq_core;
        PotdrAccumulatorCore m_accumulator_cores[4];
        PotdrLaserCtrl m_laster_ctrl;
        PotdrSpi m_adc_spi;
        PotdrSpi m_dac_spi;
        PotdrPowermeter m_powermeters[4];
        PotdrDACSource m_dac_sources[4];

#ifdef PYTHON_BINDINGS
public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<PotdrTimeBase>(m, "PotdrTimeBase")
                .def("dump", &PotdrTimeBase::dump, py::arg("level") = 0)
                .def("__repr__", &PotdrTimeBase::__repr__)
                .def_readonly("seq_core", &PotdrTimeBase::m_seq_core, py::return_value_policy::reference)
                .def_property_readonly("accumulators", [](PotdrTimeBase &m) {std::vector<PotdrAccumulatorCore*> v({&m.m_accumulator_cores[0], &m.m_accumulator_cores[1], &m.m_accumulator_cores[2], &m.m_accumulator_cores[3]}); return v;})
                .def_readonly("laster_ctrl", &PotdrTimeBase::m_laster_ctrl, py::return_value_policy::reference)
                .def_readonly("adc_spi", &PotdrTimeBase::m_adc_spi, py::return_value_policy::reference)
                .def_readonly("dac_spi", &PotdrTimeBase::m_dac_spi, py::return_value_policy::reference)
                .def_property_readonly("powermeters", [](PotdrTimeBase &m) {std::vector<PotdrPowermeter*> v({&m.m_powermeters[0], &m.m_powermeters[1], &m.m_powermeters[2], &m.m_powermeters[3]}); return v;})
                .def_property_readonly("dac_sources", [](PotdrTimeBase &m) {std::vector<PotdrDACSource*> v({&m.m_dac_sources[0], &m.m_dac_sources[1], &m.m_dac_sources[2], &m.m_dac_sources[3]}); return v;})

                .def("analog_power_on", &PotdrTimeBase::analog_power_on, py::arg("up"),  R"pbdoc(
                    Turn On or Off the power supply of the time base
                )pbdoc")

        ;
    }
#undef add_register
#endif


};

#endif // POTDRTIMEBASE_H
