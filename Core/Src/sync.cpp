//
// sync.cpp
//
// Created on: Feb 03, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#include "sync.h"

inline void syncDisableInterrupts()
{
	__asm volatile( " cpsid i " ::: "memory" );
}

inline void syncEnableInterrupts()
{
	__asm volatile 	( " cpsie i " ::: "memory" );
}

extern "C" void syncEnterCritical()
{
	syncDisableInterrupts();
	__asm volatile( "dsb" ::: "memory" );
	__asm volatile( "isb" );
}

extern "C" void syncExitCritical()
{
	syncEnableInterrupts();
}

namespace Sync {

// Critical

Critical::Critical()
{
	syncEnterCritical();
}

Critical::~Critical()
{
	syncExitCritical();
}

}  // namespace Sync
