#ifndef POTDRACCUMULATORCORE_H
#define POTDRACCUMULATORCORE_H

#include "register.h"

class PotdrAccumulatorCore : public LogicBlock
{
public:
    PotdrAccumulatorCore(LogicBlock *bdt, uint32_t offset, int idx, std::string name);
    uint32_t event_mask() { return BIT(m_idx);};

public:
    Register &r_axi_addr_offset;
    Register &r_crunch_number;
    Register &r_offset_continu;
    Register &r_state_machine_dbg;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(PotdrAccumulatorCore, PYBIND11_TOSTRING(x), r_##x)
public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<PotdrAccumulatorCore>(m, "PotdrAccumulatorCore")
                .def("__repr__", &PotdrAccumulatorCore::__repr__)
                .def("dump", &PotdrAccumulatorCore::dump, py::arg("level") = 0)
                .def("event_mask", &PotdrAccumulatorCore::event_mask)
                .add_register(axi_addr_offset)
                .add_register(crunch_number)
                .add_register(offset_continu)
                .add_register(state_machine_dbg)
                ;
    }
#undef add_register
#endif
};

#endif // POTDRACCUMULATORCORE_H
