/*
 * $Id$
 *
 * Module: calcStatsTask
 * Description: This task regularly calculates statistics on delivery
 *              of housekeeping.
 */

#define scope extern
#include "hskpAll.h"
#include <wdLib.h>
#include <taskLib.h>

// ID for our watchdog timer
static WDOG_ID Wid;

/*
 * Calculate statistics over the last period.  Only one call here is
 * required; further calls are made by a watchdog timer callback that gets
 * set up here.
 */
static float STAT_UPDATE_INTERVAL = 2.0; /* seconds */

void calcStatsTask()
{
    unsigned char hr, min, sec;
    unsigned short msec;
    unsigned int dummy;
    int nowMsec;
    int lastNowMsec = -1;
    int statTickInterval = (int)(STAT_UPDATE_INTERVAL * sysClkRateGet());
    
    /* Create the watchdog timer */
    Wid = wdCreate();
   
    while (1) {
        if (stop_flag) {
            hskpSentCount = 0;
            hskpSendRate = 0.0;
            lastNowMsec = -1;
        } else {
            // Calculate the send rate since the last time we got here
            
            // get current time
            get_time(&hr,&min,&sec,&msec,&dummy,&dummy,&dummy,&dummy);
            nowMsec = (hr * 3600 * 1000) + (min * 60 * 1000) + (sec * 1000) + msec;
            
            if (lastNowMsec >= 0) {
                // seconds since stats collection began
                float interval = 0.001 * (nowMsec - lastNowMsec);
                if (interval < 0)
                    interval += 86400.0; // secs per day
                // now calculate the send rate, in rays/sec
                hskpSendRate = hskpSentCount / interval;
            } else {
                hskpSendRate = 0.0;
            }
            hskpSentCount = 0;
            lastNowMsec = nowMsec;           
        }
        taskDelay(statTickInterval);
    }
}

