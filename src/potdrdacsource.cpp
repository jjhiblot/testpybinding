#include "dacsource.h"

PotdrDACSource::PotdrDACSource(LogicBlock *bdt, uint32_t offset, int idx, std::string name):
    LogicBlock(name, offset, bdt, idx),
    r_data_points   (MEMDEF(0x0, 0x200, "DATA_PTS_MEM", "128 Values DAC memory for modulation")),
    r_max_nb_points (REGDEF(0x200, "MAX_NB_POINTS",   "Number of points to use for modulation (<= 128)")),
    r_configuration (REGDEF(0x204, "CONFIGURATION",   "Source configuration register"))
{

}
