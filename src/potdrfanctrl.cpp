#include "potdrfanctrl.h"

PotdrFanCtrl::PotdrFanCtrl(LogicBlock *potdr, uint32_t offset, std::string name):
    LogicBlock(name, offset, potdr),
    r_pwm_cmd           (REGDEF(0x00,	"PWM and command",	"Current PWM fan and Force command")),
    r_curr_temp1        (REGDEF(0x04,	"Current Temp 1",	"Current temperature 1")),
    r_curr_temp2        (REGDEF(0x08,	"Current Temp 2",	"Current temperature 2")),
    r_threshold_1_temp_1(REGDEF(0x0C,	"Threshold 1 Temp 1",	"Hysteresis and threshold 1 for temp 1")),
    r_threshold_2_temp_1(REGDEF(0x10,	"Threshold 2 Temp 1",	"Hysteresis and threshold 2 for temp 1")),
    r_threshold_3_temp_1(REGDEF(0x14,	"Threshold 3 Temp 1",	"Hysteresis and threshold 3 for temp 1")),
    r_threshold_1_temp_2(REGDEF(0x18,	"Threshold 1 Temp 2",	"Hysteresis and threshold 1 for temp 2")),
    r_threshold_2_temp_2(REGDEF(0x1C,	"Threshold 2 Temp 2",	"Hysteresis and threshold 2 for temp 2")),
    r_threshold_3_temp_2(REGDEF(0x20,	"Threshold 3 Temp 2",	"Hysteresis and threshold 3 for temp 2"))

{

}
