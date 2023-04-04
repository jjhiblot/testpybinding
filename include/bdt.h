#ifndef BDT_H
#define BDT_H

#include "drvwrapper.h"

class BdtConfig
{
    int nb_teeth;
    int nb_points;
};

class Bdt
{
public:
    Bdt(DrvWrapper& drv, int reg_offset);
    void set_config(const BdtConfig &cfg);
    void start_acq();

private:
    DrvWrapper &m_drv;
    unsigned long m_reg_offset;
};

#endif // BDT_H
