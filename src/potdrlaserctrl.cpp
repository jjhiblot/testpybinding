#include "potdrlaserctrl.h"

PotdrLaserCtrl::PotdrLaserCtrl(LogicBlock *bdt, uint32_t offset, std::string name):
    LogicBlock(name, offset, bdt),
    r_laser_width_ent(REGDEF( 0x00, "LASER_WIDTH_ENT" ,   "Laser width (unit clock timebase = ADC clk period)")),
    r_laser_width_dec(REGDEF( 0x04, "LASER_WIDTH_DEC",    "Pattern for sub clock pulse")),
    r_laser_enable(REGDEF( 0x08, "LASER_ENABLE",       "Laser used for OTDR shot")),
    r_laser_source_enable(REGDEF( 0x0C, "LASER_SOURCE_ENABLE", "Laser used for Source"))
{

}
