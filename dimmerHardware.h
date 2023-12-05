
#undef DIMMERB
#undef DIMMERC

#define DIM_NUM    1                // Anzahl der Dimmer (muss konsistent zuden DIMMERA ... sein)
#define DIM_TIMER  TCD0
#define DIM_CLOCK  TC_CLKSEL_DIV64_gc

#define DIMMER_MIN_STATE 50
#define DIMMER_STEP 2

#ifdef DIMMERA
  #define DIMA_COUNT CCBL
  #define DIMA_ENABLE TC0_CCAEN_bm  // TC0_CCAEN_bm = PIN0 ...: muss zur nachfolgenden Definition passen.
  #define DIMA_PORT  PORTD          // definiert den PWM-Dimmer
  #define DIMA_PIN   PIN0
  #define SWA_PORT   PORTD          // definiert den Ein-Aus-Schalter
  #define SWA_PIN    PIN1
#else
  #define DIMA_ENABLE 0
#endif // DIMMERA

#ifdef DIMMERB
  #define DIMB_COUNT CCCL
  #define DIMB_ENABLE TC0_CCCEN_bm
  #define DIMB_PORT  PORTC          // definiert den PWM-Dimmer
  #define DIMB_PIN   PIN1
  #define SWB_PORT   PORTC          // definiert den Ein-Aus-Schalter
  #define SWB_PIN    PIN1
#else
  #define DIMB_ENABLE 0
#endif // DIMMERB

#ifdef DIMMERC
  #define DIMC_COUNT CCDL
  #define DIMC_ENABLE TC0_CCDEN_bm
  #define DIMC_PORT  PORTC          // definiert den PWM-Dimmer
  #define DIMC_PIN   PIN2
  #define SWC_PORT   PORTC          // definiert den Ein-Aus-Schalter
  #define SWC_PIN    PIN2
#else
  #define DIMC_ENABLE 0
#endif // DIMMERC



