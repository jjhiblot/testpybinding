#include "potdrtimebase.h"
#include "potdr.h"

PotdrTimeBase::PotdrTimeBase(LogicBlock *parent, uint32_t offset, int idx, std::string name):
    LogicBlock(name, offset, parent, idx),
    m_seq_core(this, offset + 0x0),
    m_accumulator_cores {
        PotdrAccumulatorCore(this, offset + 0x100, (idx * 4) + 0, "Accumulator Core 0"),
        PotdrAccumulatorCore(this, offset + 0x200, (idx * 4) + 1, "Accumulator Core 1"),
        PotdrAccumulatorCore(this, offset + 0x300, (idx * 4) + 2, "Accumulator Core 2"),
        PotdrAccumulatorCore(this, offset + 0x400, (idx * 4) + 3, "Accumulator Core 3")
        },
    m_laster_ctrl(this, offset + 0x500),
    m_adc_spi(this, offset  + 0x600, "ADC SPI"),
    m_dac_spi(this, offset  + 0x700, "DAC SPI"),
    m_powermeters {
        PotdrPowermeter(this, offset + 0x800, (idx * 4) + 0, "Powermeter 0"),
        PotdrPowermeter(this, offset + 0x900, (idx * 4) + 1, "Powermeter 1"),
        PotdrPowermeter(this, offset + 0xA00, (idx * 4) + 2, "Powermeter 2"),
        PotdrPowermeter(this, offset + 0xB00, (idx * 4) + 3, "Powermeter 3")
        },
    m_dac_sources {
        PotdrDACSource(this, offset + 0x1000, (idx * 4) + 0, "DAC source 0"),
        PotdrDACSource(this, offset + 0x2000, (idx * 4) + 1, "DAC source 1"),
        PotdrDACSource(this, offset + 0x3000, (idx * 4) + 2, "DAC source 2"),
        PotdrDACSource(this, offset + 0x4000, (idx * 4) + 3, "DAC source 3")
        }
{
}

void PotdrTimeBase::analog_power_on(bool up)
{
    Potdr *potdr = Potdr::potdr_module_of(this);
    potdr->analog_power_on(m_idx, up);
}


