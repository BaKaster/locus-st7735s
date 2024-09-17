//
// sync.h
//
// Created on: Feb 03, 2022
//     Author: Dmitry Murashov (d.murashov@geoscan.aero)
//

#ifndef CORE_INC_SYNC_H
#define CORE_INC_SYNC_H

#ifdef __cplusplus
extern "C" {
#endif

void syncEnterCritical();  ///< Enter critical section from user context
void syncExitCritical();  ///< Exit critical section from user context

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

namespace Sync {

struct Critical final {
	Critical();
	~Critical();
};

}  // namespace Sync

#endif  // __cplusplus

#endif // CORE_INC_SYNC_H
