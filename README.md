# PTPD FOR STM32H7 AND ATSAME70

IEEE 1588 PTP daemon for STM32H7 and ATSAME70 microcontrollers. The software is tested to work with LWIP 2.x.

## Usage
1. Enable IGMP in LWIP and make sure it is working. Typically this is done by defining LWIP_IGMP to 1 in lwipopts.h. You typically need to enable IGMP in your network interface as well (for example netif->flags |= NETIF_FLAG_IGMP)
2. Copy-paste the ptp folder to your solution.
3. From port folder copy the required ptpd_dep.c and ptpd_dep.h files to the ptp/dep folder.
4. Every 1 ms update the PTPD timers, for example as below
'// ptpd timers
	for (uint8_t i=0; i < TIMER_ARRAY_SIZE; i++)
	{
		switch (ptpdTimersCounter[i])
		{
			case 0:
			break;
			case 1:
			ptpdTimersExpired[i] = TRUE;
			ptpdTimersCounter[i] = ptpdTimers[i];
			break;
			default :
			ptpdTimersCounter[i]--;
			break;
		}
	}'
5. In the software initilization call function ptpd_init().
6. Continuously call function ptpd_task(). Remember to call igmp_timer() every 100 ms as well.
