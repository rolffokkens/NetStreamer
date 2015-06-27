#include "NrDll.h"
#include "NrNetRadio.h"
#include "NrRecPump.h"

static NrRecPump Pump (50000, "Pump");

void PutData (const char *pData, int Size)
{
    Pump.PutData (EzString (pData, Size));
};

int GetMessage (int MsCurTime, int *pMsgCode, char *pData, int *pSize)
{
    int     RetVal;
    EzString  Data;
    NrMsgCode MsgCode;

    RetVal = Pump.GetMessage (MsgCode, Data, MsCurTime);

    if (RetVal) {
        memcpy (pData, Data.Text (), Data.Length ());
        *pSize    = Data.Length ();
        *pMsgCode = MsgCode;
    };

    return RetVal;
};

int PutMessage (int MsgCode, const char *pData, int Size, char *pRetVal)
{
    EzString RetVal;

    RetVal = Pump.PutMessage (NrMsgCode (MsgCode), EzString (pData, Size));

    memcpy (pRetVal, RetVal.Text (), RetVal.Length ());

    return RetVal.Length ();
};

int GetPCM (int MsCurTime, int Amount, char *pData)
{
    EzString RetVal;

    RetVal = Pump.GetPCM (Amount, MsCurTime);

    memcpy (pData, RetVal.Text (), RetVal.Length ());

    return RetVal.Length ();
};

int GetBufSize (void)
{
    return Pump.GetBufRate ();
};
