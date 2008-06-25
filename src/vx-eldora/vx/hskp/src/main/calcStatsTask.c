/*
 * $Id$
 *
 * Module: calcStatsTask
 * Description: This task regularly calculates statistics on delivery
 *              of housekeeping.
 */

#define scope extern
#include "hskpAll.h"

// ID for our watchdog timer
static WDOG_ID Wid;

/*
 * Calculate statistics over the last period.  Only one call here is
 * required; further calls are made by a watchdog timer callback that gets
 * set up here.
 */
static float STAT_UPDATE_INTERVAL = 1.0; /* seconds */

void calcStats(int ignored)
{
    int hr, min, sec, msec, dummy;
    float intervalMsec;
    int nowMsec;
    static int lastNowMsec = -1;
    int wakeIntervalTicks;
    
    // get current time
    get_time(&hr,&minute,&sec,&msec,&dummy,&dummy,&dummy,&dummy);
    nowMsec = (hr * 3600 * 1000) + (min * 60 * 1000) + (sec * 1000) + msec;
    nowTicks = (int)(0.001 * nowMsec * sysClkGetRate())
    
    if (lastNowMsec >= 0) {
        // seconds since stats collection began
        float interval = 0.001 * (nowMsec - lastNowMsec);
        if (interval < 0)
            interval += 86400.0; // secs per day
        // now calculate the send rate, in rays/sec
        hskpSendRate = hskpSentCount / interval;
        hskpSentCount = 0;
        printf("hskp send rate %.2f rays/sec\n", hskpSendRate); 
    } else {
        // start fresh on first call here
        hskpSendRate = 0.0;
        hskpSentCount = 0;
    }
    lastNowMsec = nowMsec;
    
    /* Schedule the next callback */
    wdStart(Wid, (int)(STAT_UPDATE_INTERVAL * sysClkRateGet()), 
            calcStats, 0);
}


void calcStatsTask()
{
    /* Create the watchdog timer */
    Wid = wdCreate();
    bool statsRunning = false;
    int sleepTime = 2.0 * sysClkRateGet();  // sleep for a second below
    
    while (1) {
        // If we're in a started state and not calculating stats, start
        // that now.
        if (!stop_flag && !StatsRunning) {
            hskpSentCount = 0;
            hskpSendRate = 0.0;
            calcStats; // start the regular stats calculation
            StatsRunning = true;
        }
        // If we're in a stopped state and stat calculation is going, stop
        // it now.
        if (stop_flag && StatsRunning) {
            hskpSendRate = 0.0;
            wdCancel(wid);
            StatsRunning = false;
        }
        taskDelay(sleepTime);
    }
}

