/*
 * MyTimer.h
 *
 * Created: 11.02.2016 20:20:03
 *  Author: Christof
 */


#ifndef MYTIMERS_H_
#define MYTIMERS_H_

#include "timer.h"
#include "External.h"
#include "myconstants.h"


#ifdef FENSTER
  #define MYTIMER_NUM	3
  void checkFensterStatus(uint8_t test);
#else
  #define MYTIMER_NUM	2
#endif // FENSTER


enum{TIMER_TEMPERATURE,TIMER_REPORT,TIMER_ENTPRELLT_FENSTER};


void nextTemperatureStatus(uint8_t test);
void nextReportStatus(uint8_t test);

extern volatile TIMER MyTimers[MYTIMER_NUM];

#endif /* MYTIMERS_H_ */
