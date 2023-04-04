#include "potdr.h"
#include <exception>
#include <iostream>
#include <potdrtimebase.h>

ModulesIDs expected_ids = {
    .vendor = 0x1d7d,
    .device = 0x9021,
    .subsystem_vendor = 0x1d7d,
    .subsystem_device = 0x0001,
};

Potdr::Potdr(DrvWrapper &drv, std::string name, uint32_t offset):
    LogicBlock(name, 0x00000, drv),
    m_owned_drv(NULL),
    m_time_bases{
        PotdrTimeBase(this,   m_offset + 0x00000, 0, "Time Base 0"),
        PotdrTimeBase(this,   m_offset + 0x10000, 1, "Time Base 1"),
        PotdrTimeBase(this,   m_offset + 0x20000, 2, "Time Base 2")
        },
    r_general               (REGDEF(0x90000,    "General register", "Misc enables")),
    r_power_supply_enable   (REGDEF(0x90004,	"Power supply Enable",	"ON_ALIM and ON_ALIM_APD")),
    r_dma_trigger           (REGDEF(0x90008,	"DMA IRQ TRIGGER",	"Bit 8 = IRQ")),
    r_mezzanine_presence    (REGDEF(0x9000C,	"Mezzanine Boards presence",	"Mezzanine presence")),
    r_adc_status            (REGDEF(0x90010,	"ADC Status",		"ADC ready status")),
    r_gain_mode_3_0         (REGDEF(0x90014,	"Gain mode 3 - 0",	"Gain mode for channels 0 to 3")),
    r_gain_mode_7_4         (REGDEF(0x90018,	"Gain mode 7 - 4",	"Gain mode for channels 4 to 7")),
    r_gain_mode_11_8        (REGDEF(0x9001C,	"Gain mode 11 - 8",	"Gain mode for channels 8 to 11")),
    r_version               (REGDEF(0x90020,	"Version",		"Version register")),
    r_xadc                  (REGDEF(0x90024,	"XADC register",	"XADC direct channel ID & Data")),
    r_pattern_lsb           (REGDEF(0x90028,	"Reg_Pattern_LSB",	"Pattern register (31 downto 0)")),
    r_pattern_msb           (REGDEF(0x9002C,	"Reg_Pattern_MSB",	"Pattern register (63 downto 32)")),
    r_enable_3_0            (REGDEF(0x90030,	"Reg_Enable 3 - 0",	"Enable debug Tx for channels 0 to 3")),
    r_enable_11_8           (REGDEF(0x90034,	"Reg_Enable 11 - 8",	"Enable debug Tx for channels 8 to 11")),
    r_readwrite             (REGDEF(0x90038,	"REG_ReadWrite",	"Debug Register")),
    r_readonly              (REGDEF(0x9003C,	"REG_ReadOnly",		"Ascii = \"otdr\" (0x6F746472)")),
    m_fan_ctrl(this, m_offset + 0xA0000)
{
    // check if the IDs are correct
    if (m_drv.ids() != expected_ids)
        throw std::logic_error("IDs don't match!");
}

Potdr::~Potdr()
{
    // turn off all analog power supplies
    r_power_supply_enable = 0x0;
}

/*
 * Perform the basic initialization of the whole Potdr logic block
 */
void Potdr::init()
{
    // all debug modes, FPGA on-chip temperature and Noise align enabled
    r_general = 0x0;
    // start with all analog power supplies turned off
    r_power_supply_enable = 0;

    // Initialize all the children
    LogicBlock::init();
}

void Potdr::analog_power_on(int power_line, bool on)
{
    uint32_t mask = BIT(power_line);

    if (on)
        r_power_supply_enable |= mask;
    else
        r_power_supply_enable &= ~mask;
}

Potdr &Potdr::from_devname(const std::string devname)
{
    Potdr *n;
    DrvWrapper *d = new DrvWrapper(devname);
    n = new Potdr(*d);
    n->m_owned_drv = d;
    return *n;
}

