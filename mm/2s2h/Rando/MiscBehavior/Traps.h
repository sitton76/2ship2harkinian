#ifndef RANDO_TRAP_H
#define RANDO_TRAP_H

#include "Rando/Rando.h"<<<<<<< time_trap
typedef enum { TRAP_FREEZE, TRAP_BLAST, TRAP_SHOCK, TRAP_TIME, TRAP_MAX } TrapTypes;

extern int RollTrapType();
extern std::string GetTrapMessage();

#endif