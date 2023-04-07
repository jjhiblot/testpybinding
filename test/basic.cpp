#include <iostream>
#include "potdr.h"

int main(int argc, char* argv[])
{
    DrvWrapper d("/dev/potdr_pws2");
    Potdr module(d);

    try {
        module.init();
    } catch(const std::exception& e) {
        std::cerr << "Initialisation failed: " << e.what() << std::endl;
        return false;
    }

    // turn on the analog power for all bdt
    for (auto &&bdt : module.m_time_bases)
        bdt.analog_power_on(true);

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Dumping all regs" << std::endl;
    std::cout << d.m_regs.dump() << std::endl;

    std::cout << "-----------------------------" << std::endl;
    std::cout << "Dumping recorded operations" << std::endl;
    std::cout << d.dump_log() << std::endl;
}
