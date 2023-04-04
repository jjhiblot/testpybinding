#include "potdraccumulatorcore.h"

PotdrAccumulatorCore::PotdrAccumulatorCore(LogicBlock *bdt, uint32_t offset, int idx, std::string name):
    LogicBlock(name, offset, bdt, idx),
    r_axi_addr_offset   (REGDEF( 0x00, "AXI_ADDR_OFFSET",    "Pointer to PS7 shared memory (target to final trace)")),
    r_crunch_number     (REGDEF( 0x04, "CRUNCH_NUMBER",      "Nb Average between consecutive samples (in order to have less resolution than ADC clock)")),
    r_offset_continu    (REGDEF( 0x08, "OFFSET_CONTINU",     "Offset to subtract on each average")),
    r_state_machine_dbg (REGDEF( 0x0C, "STATE_MACHINE DBG",  ""))
{

}




