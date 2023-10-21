
#ifndef KLINGEL_H_
#define KLINGEL_H_

#define JOIN_(X,Y) X##Y
#define JOIN(X,Y) JOIN_(X,Y)
#define JOIN3_(X,Y,Z) X##Y##Z
#define JOIN3(X,Y,Z) JOIN3_(X,Y,Z)
#define JOIN4_(W,X,Y,Z) W##X##Y##Z
#define JOIN4(W,X,Y,Z) JOIN4_(W,X,Y,Z)
#define JOIN5_(V,W,X,Y,Z) V##W##X##Y##Z
#define JOIN5(V,W,X,Y,Z) JOIN5_(V,W,X,Y,Z)

#define FENSTER_PORT_C    C
#define FENSTER0_INPUT_C   6
#define FENSTER1_INPUT_C   7
#define FENSTER_INT_NUM_C 1


#define FENSTER_PORT        JOIN(PORT,FENSTER_PORT_C)
#define FENSTER0_INPUT		    JOIN3(PIN,FENSTER0_INPUT_C,_bm)   // PINx_bm
#define FENSTER1_INPUT		    JOIN3(PIN,FENSTER1_INPUT_C,_bm)   // PINx_bm
#define FENSTER0_INTPIN      JOIN3(PIN,FENSTER0_INPUT_C,CTRL)  // PINxCTRL
#define FENSTER1_INTPIN      JOIN3(PIN,FENSTER1_INPUT_C,CTRL)  // PINxCTRL
#define FENSTERINPUTSETUP   FENSTER_PORT.DIRCLR=FENSTER0_INPUT ; FENSTER_PORT.DIRCLR=FENSTER1_INPUT
#define FENSTER_INT_VEC     JOIN5( PORT,FENSTER_PORT_C,_INT,FENSTER_INT_NUM_C,_vect ) //PORTx_INTx_vect

void init_Fenster(void);
void jobFenster(ComReceiver *comRec, char function,char address,char job, void * pMem);
void entprelltFenster(uint8_t test);

#endif /* KLINGEL_H_ */
