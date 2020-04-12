/**
 * \author van Kempen Alexandre
 * \mainpage PTPd v2 Documentation
 * \version 2.0.1
 * \date 17 nov 2010
 * \section implementation Implementation
 * PTPd is full implementation of IEEE 1588 - 2008 standard of ordinary clock.
*/



/**
*\file
* \brief Main functions used in ptpdv2
*
* This header file includes all others headers.
* It defines functions which are not dependant of the operating system.
 */

#ifndef PTPD_H_
#define PTPD_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/mem.h"
#include "lwip/udp.h"
#include "lwip/igmp.h"
#include "lwip/arch.h"

#include "constants.h"
#include "dep/constants_dep.h"
#include "dep/datatypes_dep.h"
#include "datatypes.h"
#include "dep/ptpd_dep.h"

#define max(a, b)           (((a) > (b)) ?  (a) : (b))
#define min(a, b)           (((a) < (b)) ?  (a) : (b))

/**
 * \brief Convert scaled nanoseconds into TimeInternal structure
 */
void scaledNanosecondsToInternalTime(const int64_t*, TimeInternal*);
/**
 * \brief Convert TimeInternal into Timestamp structure (defined by the spec)
 */
void fromInternalTime(const TimeInternal*, Timestamp*);

/**
 * \brief Convert Timestamp to TimeInternal structure (defined by the spec)
 */
void toInternalTime(TimeInternal*, const Timestamp*);

/**
 * \brief Add two TimeInternal structure and normalize
 */
void addTime(TimeInternal*, const TimeInternal*, const TimeInternal*);

/**
 * \brief Substract two TimeInternal structure and normalize
 */
void subTime(TimeInternal*, const TimeInternal*, const TimeInternal*);

/**
 * \brief Divide the TimeInternal by 2 and normalize
 */
void div2Time(TimeInternal*);

/**
 * \brief Returns the floor form of binary logarithm for a 32 bit integer.
 * -1 is returned if ''n'' is 0.
 */
int32_t floorLog2(uint32_t);

/**
 * \brief Compare data set of foreign masters and local data set
 * \return The recommended state for the port
 */
uint8_t bmc(PtpClock*);

/**
 * \brief When recommended state is Master, copy local data into parent and grandmaster dataset
 */
void m1(PtpClock*);

/**
 * \brief When recommended state is Passive
 */
void p1(PtpClock*);

/**
 * \brief When recommended state is Slave, copy dataset of master into parent and grandmaster dataset
 */
void s1(PtpClock*, const MsgHeader*, const MsgAnnounce*);

/**
 * \brief Initialize datas
 */
void initData(PtpClock*);

/**
 * \brief Compare two port identities
 */
bool  isSamePortIdentity(const PortIdentity*, const PortIdentity*);

/**
 * \brief Add foreign record defined by announce message
 */
void addForeign(PtpClock*, const MsgHeader*, const MsgAnnounce*);

/**
 * \brief Run PTP stack in current state
 */
void doState(PtpClock*);

/**
 * \brief Change state of PTP stack
 */
void toState(PtpClock*, uint8_t);
/** \}*/

// Initialize PTP daemon thread.
void ptpd_init(void);

void ptpd_task(void);
void ptpd_alert(void);

#endif /* PTPD_H_*/
