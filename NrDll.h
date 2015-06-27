#ifndef H_NETRADIO
#define H_NETRADIO

#ifdef _WINDOWS
#   define CDECL  __cdecl
#   define DLLEXP __declspec( dllexport )
#else
#   define CDECL
#   define DLLEXP
#endif

#ifdef __cplusplus
#define EXTC extern "C" DLLEXP
#else
#define EXTC extern     DLLEXP
#endif

/*
 * PutData
 *
 * Vanaf de Netwerk verbinding (Socket) de inkomende data
 * ongezien door lussen naar deze Dll via PutData
 * Paramaters:
 * pData: een pointer naar de data
 * Size : de hoeveelheid Data
 */
EXTC void CDECL PutData
    (const char *pData, int Size);

/*
 * GetMessage
 *
 * Na aanroep van PutData d.m.v. GetMessage eventuele
 * boodschappen ophalen, b.v.:
 * 
 * PutData (....);
 * while (GetMessage (....)) {
 * };
 *
 * Parameters: 
 * MsCurTime: Tijd in ms gemeten sinds opstarten programma
 * pMsgCode : een pointer naar de te retourneren MSGCODE
 * pData    : een pointer naar de plek waar de data heenmoet
 * pSize    : een pointer naar de int waar de 
 *            hoeveelheid data moet worden teruggeven
 *
 * Return Value: 1: als er een  boodschap was
 *               0: als er geen boodschap meer is
 */
EXTC int CDECL GetMessage
    (int MsCurTime, int *pMsgCode, char *pData, int *pSize);

/*
 * PutMessage
 *
 * Het coderen van een boodschap. Alle boodschappen langs deze
 * weg coderen, en dan de gecodeerde data (pRetVal) doorgeven
 * naar het netwerk
 *
 * Parameters:
 * MsgCode: De MSGCODE
 * pData  : De mee te geven data
 * Size   : de hoveelheid data
 * pRetVal: Een pointer naar een plek waar de gecodeerde tekst
 *          kan worden gelaten door PutMessage
 *
 * Return Value: De lengte van de gecodeerde tekst
 */
EXTC int CDECL PutMessage
    (int MsgCode, const char *pData, int Size, char *pRetVal);

/*
 * GetPCM
 *
 * Aan te roepen als de sounddevice van data moet worden voorzien
 *
 * Parameters:
 * MsCurTime: Tijd in ms gemeten sinds opstarten programma
 * Amount   : De te vragen hoeveelheid 'geluid', bv, 2048 bytes
 * pData    : Een pointer naar de plek waar de data terecht
 *            moet komen.
 *
 * Return Value: de werkelijk opgeleverde hoeveelheid data
 */
EXTC int CDECL GetPCM
    (int MsCurTime, int Amount, char *pData);

/*
 * GetBufSize
 *
 * De thans gebufferde hoeveelheid geluid wordt door deze
 * functie opgeleverd.
 * Voor de liefhebber. Leuk grafisch metertje op de radio??
 */
EXTC int CDECL GetBufSize (void);

#endif
