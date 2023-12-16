/*
* LuefterSteuerung.cpp
*
* Created: 16.03.2017 13:03:01
* Author : a16007
*/

#include "KlimaStationSHTC3.h"

void setup()
{
  init_clock(SYSCLK,PLL,true,CLOCK_CALIBRATION);
	PORTA_DIRSET = PIN2_bm | PIN3_bm | PIN4_bm;
	PORTA_OUTSET = 0xff;

	PORTB_DIRSET = 0xff;

	PORTC_DIRCLR = 0xff;
	PORTC_DIRSET = PIN1_bm; // I2C-Clock
#ifdef FENSTER
	PORTC_DIRSET = PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm;   // PIN6 und 7 sind die Fenstereingänge
#else
	PORTC_DIRSET = PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm;
#endif // FENSTER


	PORTD_DIRCLR = 0xff;
#if BOARD == 1
  PORTD_DIRSET = PIN0_bm | PIN1_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm ;
#elif BOARD == 2
  PORTD_DIRSET = 0xff;
#endif // BOARD
  PORTD_DIRSET = 0xff;
  PORTE_DIRCLR = 0xff;
#if BOARD == 1
  PORTE_DIRSET = 0xff;
#elif BOARD == 2
  PORTE_DIRSET = PIN0_bm | PIN1_bm | PIN3_bm;
#endif // BOARD


  LEDROTSETUP;
  LEDGRUENSETUP;
  LEDBLAUSETUP;

#ifdef FENSTER
  init_Fenster();
#endif // FENSTER

	uint8_t i;

#ifdef DIMMERA
  initDimmer();
  initDimmerStatus();
  updateDimmer();
#endif // DIMMERA

  TWI_MasterInit(&twiC_Master, &TWIC, TWI_MASTER_INTLVL_LO_gc, TWI_BAUDSETTING);


	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();

	for(i=0;i<20;i++)
	{
		LEDGRUEN_TOGGLE;
		_delay_ms(50);
	}
  LEDGRUEN_OFF;


	cnet.open(Serial::BAUD_57600,F_CPU);
}

int main(void)
{
uint8_t reportStarted = false;

	setup();

	cnet.broadcastUInt8((uint8_t) RST.STATUS,'S','0','R');

	init_mytimer();

	uint8_t sensorReady=SENSOR_READY;
  _delay_ms(100);

  humiSensor.begin(&twiC_Master);
  humiSensor.setMode(SHTC3_NORMAL_T_FIRST);
  cnet.broadcastUInt16(humiSensor.getID(),'S','I','D');

	while (1)
	{
#ifdef DIMMERA
    updateDimmer();
#endif // DIMMERA
		cnetRec.comStateMachine();
		cnetRec.doJob();

		switch(statusSensoren)
		{
			case KLIMASENSOR:
				sensorReady = doClima();
			break;
			case LASTSENSOR:
				LEDROT_OFF;
				sensorReady = doLastSensor();
			break;
		}
		if (sensorReady==SENSOR_READY)
		{
			statusSensoren++;
			if (statusSensoren>LASTSENSOR)
			{
				statusSensoren = KLIMASENSOR;
				if(reportStarted==false)
                {
                    MyTimers[TIMER_REPORT].state = TM_START;
                    reportStarted = true;
                }
			}
		}
		if( sendStatusReport )
    {
        char buffer[16];
        sendStatusReport = false;
        MyTimers[TIMER_REPORT].value = actReportBetweenSensors;
        MyTimers[TIMER_REPORT].state = TM_START;
        switch(statusReport)
        {
            case TEMPREPORT:
                LEDGRUEN_ON;
                sprintf(buffer,"%.1f",(double)fTemperatur);
                cnet.sendStandard(buffer,BROADCAST,'C','1','t','F');
                LEDGRUEN_OFF;
            break;
            case HUMIREPORT:
                LEDGRUEN_ON;
                sprintf(buffer,"%.1f",(double)fHumidity);
                cnet.sendStandard(buffer,BROADCAST,'C','1','h','F');
                LEDGRUEN_OFF;
            break;
            case ABSHUMIREPORT:
                LEDGRUEN_ON;
                sprintf(buffer,"%.1f",(double)fAbsHumitdity);
                cnet.sendStandard(buffer,BROADCAST,'C','1','a','F');
                LEDGRUEN_OFF;
            break;
            case DEWPOINTREPORT:
                LEDGRUEN_ON;
                sprintf(buffer,"%.1f",(double)fDewPoint);
                cnet.sendStandard(buffer,BROADCAST,'C','1','d','F');
                LEDGRUEN_OFF;
            break;
#ifdef FENSTER
            case FENSTERREPORT0:
                LEDGRUEN_ON;
                cnet.sendStandard(fensterStatusText[fensterStatus[0]],BROADCAST,'F','0','a','F');
                LEDGRUEN_OFF;
            break;
            case FENSTERREPORT1:
                LEDGRUEN_ON;
                cnet.sendStandard(fensterStatusText[fensterStatus[1]],BROADCAST,'F','1','a','F');
                LEDGRUEN_OFF;
            break;
#endif // FENSTER
#ifdef DIMMERA
            case DIMMERREPORTA:
                LEDGRUEN_ON;
                sendDimStatus(0, actualStatus.dimmer[0]);
                LEDGRUEN_OFF;
            break;
#endif // DIMMERA
            case LASTREPORT:
                MyTimers[TIMER_REPORT].value = actReportBetweenBlocks;
                MyTimers[TIMER_REPORT].state = TM_START;
            break;
        }
    }

	}
}

uint8_t doLastSensor()
{
	switch( statusLastSensor )
	{
		case NOTHING_LAST_TODO:
			MyTimers[TIMER_TEMPERATURE].value = actWaitAfterLastSensor;
			MyTimers[TIMER_TEMPERATURE].state = TM_START;
			statusLastSensor = WAIT_LAST;
		break;
		case READY_LAST:
			statusLastSensor = NOTHING_LAST_TODO;
		break;
	}
	return statusLastSensor;
}

uint8_t doClima()
{
bool noError;

	switch(statusKlima)
	{
		case NOTHING_CLIMA_TODO:
			statusKlima = WAKEUP;
		break;
		case WAKEUP:
		  humiSensor.wakeup();
		  statusKlima = WAIT_WAKEUP;
      MyTimers[TIMER_TEMPERATURE].value = 2; //22
      MyTimers[TIMER_TEMPERATURE].state = TM_START;
    break;

		case START_CONVERSION: //
			LEDROT_ON;

			noError=humiSensor.startMeasure();
			if (noError==true)
			{
				statusKlima = WAIT_CONVERSION;
				MyTimers[TIMER_TEMPERATURE].value = 2; //22
				MyTimers[TIMER_TEMPERATURE].state = TM_START;
			}
			else
				statusKlima = NOTHING_CLIMA_TODO;
		break;

		case READ_CONVERSION:  // Durchlaufzeit ca.
			noError = humiSensor.readResults();
			if (noError==true)
			{
				statusKlima = WAIT_READ;
				MyTimers[TIMER_TEMPERATURE].value = 2; //22
				MyTimers[TIMER_TEMPERATURE].state = TM_START;
			}
			else
				statusKlima = NOTHING_CLIMA_TODO;
		break;
		case CALC_CONVERSION0:  // Durchlaufzeit ca.
		  humiSensor.getResults(fTemperatur,fHumidity);
		  humiSensor.sleep();
			statusKlima = CALC_CONVERSION1;
		break;
		case CALC_CONVERSION1:  // Durchlaufzeit ca.
		  fAbsHumitdity =  humiSensor.calcAbsHumi(fTemperatur,fHumidity);
			statusKlima = CALC_CONVERSION2;
		break;
		case CALC_CONVERSION2:  // Durchlaufzeit ca.
		  fDewPoint =  humiSensor.calcDewPoint(fTemperatur,fHumidity);
			statusKlima = NOTHING_CLIMA_TODO;
		break;
	}
	return(statusKlima);
}

void writeEEData()
{
  LEDBLAU_ON;
	MyTimers[TIMER_SAVE_DELAY].state = TM_START; // Speicherverzögerung läuft los
}


