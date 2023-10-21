#ifdef FENSTER


#include "External.h"

void init_Fenster()
{
	FENSTERINPUTSETUP;
	FENSTER_PORT.INTCTRL  |= PORT_INT1LVL0_bm ; // Low-Level interrupt 0 for PORTA
	FENSTER_PORT.INT1MASK |= FENSTER0_INPUT;
	FENSTER_PORT.INT1MASK |= FENSTER1_INPUT;
	FENSTER_PORT.FENSTER0_INTPIN = PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
	FENSTER_PORT.FENSTER1_INTPIN = PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc | PORT_SRLEN_bm;
}

void jobFenster(ComReceiver *comRec, char function,char address,char job, void * pMem)
{
}

void entprelltFenster(uint8_t test)
{
  if( (FENSTER_PORT.IN & FENSTER0_INPUT)==0 )
	  fensterStatus[0] = 0;
  else
    fensterStatus[0] = 1;
  if( (FENSTER_PORT.IN & FENSTER1_INPUT)==0 )
	  fensterStatus[1] = 0;
  else
    fensterStatus[1] = 1;
  nextReportStatus(0);
  LEDBLAU_OFF;
}

SIGNAL(FENSTER_INT_VEC)
{
	MyTimers[TIMER_ENTPRELLT_FENSTER].state = TM_START;
	LEDBLAU_ON;
}

#endif // FENSTER
