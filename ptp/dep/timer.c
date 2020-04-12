/* timer.c */

#include "../ptpd.h"

/* An array to hold the various system timer handles. */
volatile uint16_t ptpdTimers[TIMER_ARRAY_SIZE];
volatile uint16_t ptpdTimersCounter[TIMER_ARRAY_SIZE];
volatile bool ptpdTimersExpired[TIMER_ARRAY_SIZE];
 
void initTimer(void)
{
	int32_t i;

	DBG("initTimer\n");

	/* Create the various timers used in the system. */
  for (i = 0; i < TIMER_ARRAY_SIZE; i++)
  {
		// Mark the timer as not expired.
		// Initialize the timer.
		ptpdTimers[i] = 0;
		ptpdTimersCounter[i] = 0;
		ptpdTimersExpired[i] = FALSE;
	}
}

void timerStop(int32_t index)
{
	/* Sanity check the index. */
	if (index >= TIMER_ARRAY_SIZE) return;

	// Cancel the timer and reset the expired flag.
	DBGV("timerStop: stop timer %d\n", index);
	ptpdTimers[index] = 0;
	ptpdTimersCounter[index] = 0;
	ptpdTimersExpired[index] = FALSE;
}

void timerStart(int32_t index, uint32_t interval_ms)
{
	/* Sanity check the index. */
	if (index >= TIMER_ARRAY_SIZE) return;

	// Set the timer duration and start the timer.
	DBGV("timerStart: set timer %d to %d\n", index, interval_ms);
	ptpdTimersExpired[index] = FALSE;
	ptpdTimers[index] = interval_ms;
	ptpdTimersCounter[index] = interval_ms;
}

bool timerExpired(int32_t index)
{
	/* Sanity check the index. */
	if (index >= TIMER_ARRAY_SIZE) return FALSE;

	/* Determine if the timer expired. */
	if (!ptpdTimersExpired[index]) return FALSE;
	DBGV("timerExpired: timer %d expired\n", index);
	ptpdTimersExpired[index] = FALSE;

	return TRUE;
}
