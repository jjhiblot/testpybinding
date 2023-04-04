#ifndef DRVWRAPPER_H
#define DRVWRAPPER_H

#include <stdint.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include "register.h"

#ifdef PYTHON_BINDINGS
#include <pybind11/pybind11.h>
namespace py = pybind11;
#endif

enum ModuleSide{
    PWS = 0,
    OPS = 1
};

struct ModuleLocation {
    uint16_t slice; // slice or hetnet position
    ModuleSide side; // side (or hetnet Slot (A/B)
};

struct ModulesIDs {
    uint16_t vendor;
    uint16_t device;
    uint16_t subsystem_vendor;
    uint16_t subsystem_device;
    auto operator!=(const ModulesIDs &a) const {
        return a.vendor != vendor || a.device != device || a.subsystem_device != subsystem_device || a.subsystem_vendor != subsystem_vendor;
    };
    auto operator==(const ModulesIDs &a) const {
        return !(a != *this);
    };
};

class RegisterFactory;

class DrvWrapper {

public:
    DrvWrapper(const std::string &devname);
    ~DrvWrapper();

    void open();
    void close();


    /* register access. memory-mapped */
    void write(unsigned long offset, uint32_t value);
    uint32_t read(unsigned long offset) const;

    /* Wait for one event among many*/
    uint32_t wait_event(uint32_t mask, bool reset_event, int timeout_ms);

    /* perform a DMA transfer (syncrhonous) */
    void sync_dma(u_int32_t start, void *dst, uint32_t sz, uint32_t timeout_ms);

    /* Allocation /deallocation of a chunk of the device's memory */
    uint32_t allocate_device_mem(uint32_t sz);
    void free_device_mem(uint32_t addr, uint32_t sz);

    ModulesIDs ids() const { return m_device_ids; }

    ModuleLocation location() const { return m_module_location; }

    std::stringstream &log() { return *m_log;}

private:
    void validate_file_descriptor() const;
    void validate_register_access(unsigned long offset) const;

private:
    std::string m_devname;
    int m_fd;
    unsigned int m_reg_area_size;
    uint32_t *m_reg_ptrs;
    ModulesIDs m_device_ids;
    ModuleLocation m_module_location;
    std::stringstream *m_log;

public:
    RegisterFactory m_regs;

#ifdef PYTHON_BINDINGS
public:
    static void add_class(pybind11::module &m)
    {
        py::enum_<ModuleSide>(m, "ModuleSide")
           .value("PWS", ModuleSide::PWS )
           .value("OPS",  ModuleSide::OPS  )
           .export_values();

        py::class_<ModulesIDs>(m, "DeviceIDs")
                .def_readwrite("vendor", &ModulesIDs::vendor)
                .def_readwrite("device", &ModulesIDs::device)
                .def_readwrite("subsystem_vendor", &ModulesIDs::subsystem_vendor)
                .def_readwrite("subsystem_device", &ModulesIDs::subsystem_device);

        py::class_<ModuleLocation>(m, "ModuleLocation")
                .def_readwrite("slice", &ModuleLocation::slice)
                .def_readwrite("side", &ModuleLocation::side);

        py::class_<DrvWrapper>(m, "DrvWrapper")
            .def(py::init<const std::string &>())
            .def(py::init<const DrvWrapper &>())

            .def("open", &DrvWrapper::open,R"pbdoc(
                Open the device file. this is required before any other operation
            )pbdoc")

            .def("close", &DrvWrapper::close, R"pbdoc(
                Close the device file.
            )pbdoc")

            .def("read", &DrvWrapper::read,R"pbdoc(
                Reads a register at the given offset (in bytes) from the ctrl space
            )pbdoc")

            .def("write", &DrvWrapper::write, R"pbdoc(
                Write the given value into a register at the given offset (in bytes)
            )pbdoc")

            .def("WaitEvent", &DrvWrapper::wait_event, R"pbdoc(
                Wait for an event in a given set of events
            )pbdoc")

            .def("sync_dma", [](DrvWrapper &m, uint32_t src, uint32_t sz, uint32_t timeout_ms) {
                        char result[sz];
                        m.sync_dma(src, result, sz, timeout_ms);
                        return py::bytearray(result, sz);
                    }, R"pbdoc(
                Start a DMA transfer
            )pbdoc", py::arg("src"), py::arg("sz"), py::arg("timeout_ms")=100)

            .def("allocate_device_mem", &DrvWrapper::allocate_device_mem, R"pbdoc(
                Allocate a chunk of the device's memory (for DMA usage)
            )pbdoc")

            .def("free_device_mem", &DrvWrapper::free_device_mem, R"pbdoc(
                Free a previously allocated chunk of the device's memory
            )pbdoc")

            .def_property_readonly("ids", &DrvWrapper::ids, R"pbdoc(
                The identification of the device (PCI IDs)
            )pbdoc", py::return_value_policy::reference)

            .def_property_readonly("location", &DrvWrapper::location, R"pbdoc(
                The location of the device (slice/side)
            )pbdoc", py::return_value_policy::reference)

            .def_property_readonly("regs", [](DrvWrapper& m) {return m.m_regs;}, R"pbdoc(
                The registers of the device
            )pbdoc", py::return_value_policy::reference)

            .def("log", [](DrvWrapper &m, bool clear) {std::string s = m.log().str(); if (clear) m.log().str(std::string()); return s;},
            py::arg("clear")=false,
            R"pbdoc(
                Get the log (and optinnaly clear it)
            )pbdoc");

    }
#endif

};
#endif
