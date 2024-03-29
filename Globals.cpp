/*
 * Globals.cpp
 *
 * Created: 19.03.2017 09:24:37
 *  Author: Christof
 */
#define EXTERNALS_H_

#include "KlimaStationSHTC3.h"

#define CF "CF"
#define CB "CB"



const char *Node = NODE_STRING;
char IDNumber[12] EEMEM = "1784324-01";
char SerialNumber[12] EEMEM = "1958632254";
char IndexNumber[2] EEMEM = "A";

/*
#if NODE_STRING == CB
  const char infoString[]="";
#endif // NODE_STRING
*/

#ifdef FENSTER
const char *fensterStatusText[2]={"zu","offen"};
uint8_t fensterStatus[2] = {0, 0};
#endif // FENSTER

uint16_t actReportBetweenBlocks  = REPORT_BETWEEN_BLOCKS;
uint16_t actReportBetweenSensors = REPORT_BETWEEN_SENSORS;
uint16_t actWaitAfterLastSensor  = WAIT_AFTER_LAST_SENSOR;


volatile double  fTemperatur=-999,fHumidity=-999,fDewPoint=-999,fAbsHumitdity=-999;
volatile double  fF1Swell=25,fF1Hysterese=1,fF2Swell=35,fF2Hysterese=2;

volatile uint8_t statusSensoren = KLIMASENSOR;
volatile uint8_t statusReport = TEMPREPORT;
volatile bool    sendStatusReport = false;
volatile uint8_t statusKlima = NOTHING_CLIMA_TODO;
volatile uint8_t statusLastSensor = NOTHING_LAST_TODO;

char SecurityLevel = 0;

uint8_t actNumberSensors = 0;

volatile bool nextSendReady=false;

Communication cnet(0,Node,5,true);
ComReceiver cnetRec(&cnet,Node,cnetCommands,NUM_COMMANDS,NULL,0,NULL,NULL);

/* Global variables for TWI */
TWI_MasterDriver_t twiC_Master;    /*!< TWI master module. */
TWI_MasterDriver_t twiE_Master;    /*!< TWI master module. */


shtc3 humiSensor;

#ifdef DIMMERA
  LAMP_STATUS actualStatus,oldStatus,lastUpdateStatus;
  const  LAMP_STATUS backupStatus PROGMEM = {1,{243},146};
  LAMP_STATUS saveStatus[2] EEMEM= {  {1,{254},146},{2,{254},252}  };
#endif
