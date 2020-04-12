#include <asf.h>

#include "ptpd_dep.h"

extern uint32_t aes67_timestamp;

void ETH_PTPTime_GetTime(struct ptptime_t * timestamp)
{
	timestamp->tv_nsec = GMAC->GMAC_TN;
	timestamp->tv_sec = GMAC->GMAC_TSL;
}

/*******************************************************************************
* Function Name  : ETH_PTPStart
* Description    : Initialize timestamping ability of ETH
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPStart(void)
{
	// ieee 1588 time stamp unit increment value every mck cycle, 6ns x 2 + 8 ns
	GMAC->GMAC_TI = 0x020806;
}

/*******************************************************************************
* Function Name  : ETH_PTPTimeStampUpdateOffset
* Description    : Updates time base offset
* Input          : Time offset with sign
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPTime_UpdateOffset(struct ptptime_t * timeoffset)
{
	volatile uint32_t gmac_seconds;

	// if offset more than a second
	if (timeoffset->tv_sec != 0)
	{
		gmac_seconds = GMAC->GMAC_TSL;
		gmac_seconds += timeoffset->tv_sec;
		GMAC->GMAC_TSL = gmac_seconds;
		return;
	}

	/* determine sign and correct Nanosecond values */
	if(timeoffset->tv_nsec > 0)
	{
		// negative offset so we subtract
		GMAC->GMAC_TA = 1<<31 | timeoffset->tv_nsec;
	}
	else
	{
		// positive offset so we add
		GMAC->GMAC_TA = 0x7FFFF & timeoffset->tv_nsec;
	}
}

/*******************************************************************************
* Function Name  : ETH_PTPTimeStampSetTime
* Description    : Initialize time base
* Input          : Time with sign
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPTime_SetTime(struct ptptime_t * timestamp)
{
	GMAC->GMAC_TSH = 0;
	GMAC->GMAC_TSL = timestamp->tv_sec;
	GMAC->GMAC_TN = timestamp->tv_nsec;
	
	// AVB clock timer reset
	GMAC->GMAC_NSC = 0;
	GMAC->GMAC_SCL = GMAC->GMAC_TSL+1;
	GMAC->GMAC_SCH = GMAC->GMAC_TSH;
}

/*******************************************************************************
* Function Name  : ETH_PTPTimeStampAdjFreq
* Description    : Updates time stamp addend register
* Input          : Correction value in thousandth of ppm (Adj*10^9)
* Output         : None
* Return         : None
*******************************************************************************/
void ETH_PTPTime_AdjFreq(int32_t Adj)
{
	uint32_t addend;
	Adj = -Adj;
	
	if( Adj > ADJ_FREQ_MAX) Adj = ADJ_FREQ_MAX;
	if( Adj < -ADJ_FREQ_MAX) Adj = -ADJ_FREQ_MAX;
	
	if (Adj == 0)
	{
		GMAC->GMAC_TI = 0x020806;
		GMAC->GMAC_TISUBN = 0;
	}
	else if (Adj > 0)
	{
		GMAC->GMAC_TI = 0x020805;
		GMAC->GMAC_TISUBN = 65535-Adj;
	}
	else
	{
		GMAC->GMAC_TI = 0x020806;
		GMAC->GMAC_TISUBN = (uint16_t)-Adj;
	}
}