#include "potdrpowermeter.h"

PotdrPowermeter::PotdrPowermeter(LogicBlock *bdt, uint32_t offset, int idx, std::string name):
    LogicBlock(name, offset, bdt, idx),
    r_cmd_dark_g1       (REGDEF( 0x00,	"CMD_DARK_G1",	"Command Dark register G1")),
    r_cmd_dark_g2       (REGDEF( 0x04,	"CMD_DARK_G2",	"Command Dark register G2")),
    r_cmd_dark_g3       (REGDEF( 0x08,	"CMD_DARK_G3",	"Command Dark register G3")),
    r_cmd_dark_g4       (REGDEF( 0x0C,	"CMD_DARK_G4",	"Command Dark register G4")),
    r_value_min_g1      (REGDEF( 0x10,	"VALUE_MIN_G1",	"Value Min for gain change G1")),
    r_value_min_g2      (REGDEF( 0x14,	"VALUE_MIN_G2",	"Value Min for gain change G2")),
    r_value_max_g2      (REGDEF( 0x18,	"VALUE_MAX_G2",	"Value Max for gain change G2")),
    r_value_min_g3      (REGDEF( 0x1C,	"VALUE_MIN_G3",	"Value Min for gain change G3")),
    r_value_max_g3      (REGDEF( 0x20,	"VALUE_MAX_G3",	"Value Max for gain change G3")),
    r_value_max_g4      (REGDEF( 0x24,	"VALUE_MAX_G4",	"Value Max for gain change G4")),
    r_it_rate           (REGDEF( 0x28,	"IT_RATE",	"Rate of PWM IT")),
    r_laser_freq        (REGDEF( 0x2C,	"LASER_FREQ",	"Laser frequency")),
    r_nb_avg_pwm        (REGDEF( 0x30,	"NB_AVG_PWM",	"")),
    r_nb_avg_zero_auto  (REGDEF( 0x34,	"NB_AVG_ZERO_AUTO", "")),
    r_data_zero_auto_high(REGDEF( 0x38,	"DATA_ZERO_AUTO_HIGH", "")),
    r_data_zero_auto_low(REGDEF( 0x3C,	"DATA_ZERO_AUTO_LOW", "")),
    r_pmw_tr_avg_high   (REGDEF( 0x40,	"PWM_TR_AVG_HIGH", "")),
    r_pmw_tr_avg_low    (REGDEF( 0x44,	"PWM_TR_AVG_LOW", "")),
    r_cmd_freq          (REGDEF( 0x48,	"CMD_FREQ",	"Command frequency meter")),
    r_cmd_pwm           (REGDEF( 0x4C,	"CMD_PWM",	"Command radiometer")),
    r_info_gain         (REGDEF( 0x50,	"INFO_GAIN",	"Applied Gain")),
    r_half_period       (REGDEF( 0x54,	"HALF_PERIOD",	"On the fly half period")),
    r_period            (REGDEF( 0x58,	"PERIOD",	"On the fly period")),
    r_half_period_mem   (REGDEF( 0x5C,	"HALF_PERIOD_MEM", "Hold half period")),
    r_period_mem        (REGDEF( 0x60,	"PERIOD_MEM",	"Hold period")),
    r_avg_level         (REGDEF( 0x64,	"AVG_LEVEL",	"Average ADC level"))
{

}
