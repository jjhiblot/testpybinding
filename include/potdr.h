#ifndef POTDR_H
#define POTDR_H

#include "drvwrapper.h"
#include "bdt.h"
#include "register.h"
#include "potdrtimebase.h"
#include "potdrfanctrl.h"

class Potdr: public LogicBlock
{
public:
    Potdr(DrvWrapper &drv, std::string name = "Parrallel OTDR", uint32_t offset = 0);
    ~Potdr();

    void init();
    void analog_power_on(int power_line, bool on);

    static Potdr &from_devname(const std::string devname);

    static Potdr* potdr_module_of(LogicBlock* lb) {
        return dynamic_cast<Potdr*>(lb->root_block());
    }
private:
    DrvWrapper *m_owned_drv;
public:
    PotdrTimeBase m_time_bases[3];
    Register &r_general;
    Register &r_power_supply_enable;
    Register &r_dma_trigger;
    Register &r_mezzanine_presence;
    Register &r_adc_status;
    Register &r_gain_mode_3_0;
    Register &r_gain_mode_7_4;
    Register &r_gain_mode_11_8;
    Register &r_version;
    Register &r_xadc;
    Register &r_pattern_lsb;
    Register &r_pattern_msb;
    Register &r_enable_3_0;
    Register &r_enable_11_8;
    Register &r_readwrite;
    Register &r_readonly;
    PotdrFanCtrl m_fan_ctrl;

#ifdef PYTHON_BINDINGS
#undef add_register
#define add_register(x) def_register(Potdr, PYBIND11_TOSTRING(x), r_##x)

public:
    static void add_class(pybind11::module &m)
    {
        auto c = py::class_<Potdr>(m, "Potdr")
                .def(py::init([](DrvWrapper& d) {
                         py::object o = py::cast(d);
                         py::handle h = o;
                         Py_INCREF(h.ptr());
                        return std::unique_ptr<Potdr>(new Potdr(d));
                    }))
                .def("__del__",[](Potdr& p) {
                         py::object o = py::cast(p.m_drv);
                         py::handle h = o;
                         Py_DECREF(h.ptr());})
                .def(py::init<const Potdr &>())
                .def_property_readonly("drv", [](Potdr &m) { return &m.m_drv;}, py::return_value_policy::reference)
                .def_property_readonly("regs", [](Potdr &m) { return &m.m_regs;}, py::return_value_policy::reference)
                .def("__repr__", &Potdr::__repr__)
                .def("dump", &Potdr::dump, py::arg("level") = 0)
                .def_property_readonly("time_bases", [](Potdr &m) {std::vector<PotdrTimeBase*> v({&m.m_time_bases[0],&m.m_time_bases[1],&m.m_time_bases[2]}); return v;})
                .add_register(general)
                .add_register(power_supply_enable)
                .add_register(dma_trigger)
                .add_register(mezzanine_presence)
                .add_register(adc_status)
                .add_register(gain_mode_3_0)
                .add_register(gain_mode_7_4)
                .add_register(gain_mode_11_8)
                .add_register(version)
                .add_register(xadc)
                .add_register(pattern_lsb)
                .add_register(pattern_msb)
                .add_register(enable_3_0)
                .add_register(enable_11_8)
                .add_register(readwrite)
                .add_register(readonly)
                .def_property_readonly("fanctrl", [](Potdr &m) {return &m.m_fan_ctrl;},  py::return_value_policy::reference)

                .def("init", &Potdr::init, R"pbdoc(
                    Initialize the Parralel Otdr module
                )pbdoc")
                .def("analog_power_on", &Potdr::analog_power_on, py::arg("power_line"), py::arg("up"),  R"pbdoc(
                    Turn On or Off the power supply indicated by power_line
                )pbdoc")
                ;
    }
#undef add_register
#endif


};

#endif // POTDR_H
