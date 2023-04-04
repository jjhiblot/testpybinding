#include "potdrspi.h"

PotdrSpi::PotdrSpi(LogicBlock *bdt, uint32_t offset, std::string name):
    LogicBlock(name, offset, bdt),
    r_configuration (REGDEF(0x00, "Configuration register", "Destination and start init command")),
    r_status        (REGDEF(0x04, "Status register", "Status register – init or request in process")),
    r_direct_request(REGDEF(0x08, "Direct SPI request register", "Register to generate a request directly over SPI")),
    r_last_read     (REGDEF(0x0C, "Last read register", "Read data from last read request over SPI"))
{

}
