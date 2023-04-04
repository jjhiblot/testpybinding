#include "potdrsequencercore.h"
#include "potdrtimebase.h"

PotdrSequencerCore::PotdrSequencerCore(LogicBlock *bdt, uint32_t offset, std::string name):
    LogicBlock(name, offset, bdt),
    r_teeth         (REGDEF(0x00, "TEETH_NUMBER",   "Active teeth number per comb")),
    r_laser_delay   (REGDEF(0x04, "LASER_DELAY", 	"Teeth number before Laser Shot")),
    r_masq_duration (REGDEF(0x08, "MASQ_DURATION", 	"Teeth Number with APD Gain reduced")),
    r_wait_duration (REGDEF(0x0C, "WAIT_DURATION", 	"Teeth number wait between end of measure noise zone")),
    r_noise_duration(REGDEF(0x10, "NOISE_DURATION", "Teeth number reserved after measure for noise zone")),
    r_average_number(REGDEF(0x14, "AVERAGE_NUMBER", "Nb average until interrupt")),
    r_ramp_incr     (REGDEF(0x18, "RAMP_INCR",      "Increment (in DAC Step unit) between each average")),
    r_ramp_reset_val(REGDEF(0x1C, "RAMP_RESET_VAL", "Initial value of DAC Step")),
    r_comb_number   (REGDEF(0x20, "COMB_NUMBER", 	"Comb number in order to have a resolution superior to ADC clock")),
    r_comb_incr     (REGDEF(0x24, "COMB_INCR",      "Increment in precalculated fine delay array")),
    r_misc          (REGDEF(0x48, "MISC",           "Command a status REGDEFister")),
    r_hw_cfg        (REGDEF(0x4C, "HARDWARE_CFG", 	"Misc Hardware config REGDEFister")),
    r_total_teeth   (REGDEF(0x50, "TOTAL_TEETH_NB_CAL", "Automatically calculated total teeth nb (nb total of points(laser_delay + teeth + noise)")),
    r_force_comb_id (REGDEF(0x54, "FORCE_COMB_ID", 	"For debug, force a comb, will override Comb_Number/Comb_Incr")),
    r_recure_prime_number(REGDEF(0x60, "RECUR_PRIME_NUMBER", "First prime number > recurrence")),
    r_state_debug   (REGDEF(0x64, "STATE_DEBUG", ""))
{
}
