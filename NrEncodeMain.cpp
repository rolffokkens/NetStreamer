/*
 * file: NrEncodeMain.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *
 */

#include <malloc.h>
#include <iostream>

#include "XxBase.h"
#include "XxStream.h"
#include "XxStdio.h"
#include "NrComp.h"
#include "NrRateAdjust.h"

using namespace std;

class NrEncoder;

class NrEncoder : public XxBase {
    class  EncIn;
    class  EncOut;
    friend EncIn;
    friend EncOut;
private:
    EzString     InBuf;
    NrComp       Comp;
    NrRateAdjust Adjust;

    void InEof  (void);
    void OutEof (void);
    void GetRWFlags (int &rFlag, int &wFlag);
    void HandleRead (EzString Data);
    void OnBufEmpty (void);

    class EncIn : public XxStdIn {
    private:
        NrEncoder *pEnc;
    protected:
        virtual void OnDisconnect (void) { pEnc->InEof (); };
        virtual void HandleRead (EzString Data) { pEnc->HandleRead (Data); };
    public:
        EncIn (NrEncoder *pEnc) {EncIn::pEnc = pEnc; };
        virtual ~EncIn (void) { };

        virtual void GetRWFlags (int &rFlag, int &wFlag) {
            pEnc->GetRWFlags (rFlag, wFlag);
        };
    };

    class EncOut : public XxStdOut {
    private:
        NrEncoder *pEnc;
    protected:
        virtual void OnDisconnect (void) { pEnc->OutEof (); };
        virtual void OnBufEmpty   (void) { pEnc->OnBufEmpty (); };
    public:
        EncOut (NrEncoder *pEnc) {EncOut::pEnc = pEnc; };
        virtual ~EncOut (void) { };

        void Write (EzString Data) { XxStream::Write (Data); };
    };

    EncIn  Input;
    EncOut Output;
public:
    NrEncoder (int InSampleRate, int OutSampleRate);
    virtual ~NrEncoder (void) { };
};

NrEncoder::NrEncoder (int InSampleRate, int OutSampleRate)
    : Comp (3), Adjust (InSampleRate, OutSampleRate)
    , Input (this), Output (this)
{
};

void NrEncoder::InEof  (void)
{
    if (Output.GetOutBufSize () == 0) Remove ();
};

void NrEncoder::OutEof (void)
{
    Remove ();
};

void NrEncoder::OnBufEmpty (void)
{
    if (Input.GetStatus () == XxBlocker::StatClosed) Remove ();
};

void NrEncoder::GetRWFlags (int &rFlag, int &wFlag)
{
    wFlag = 0;

    rFlag = (Output.GetOutBufSize () < 10000);
};

void NrEncoder::HandleRead (EzString Data)
{
    EzString Temp;

    InBuf += Adjust.ProcessData (Data);

    while (InBuf.Length () >= 2048) {
        Temp  = Substr (InBuf, 0, 2048);
        InBuf = Substr (InBuf, 2048);
        Temp = Comp.Compress (Temp);
        Output.Write (Temp);
    };
};

static void ShowArgErrorMessage (EzString ProgName)
{
    cerr
    << "Bad parameters\n"
    << "Usage:" << endl
    << "    " << ProgName
    << " InSampleRate OutSampleRate\n"
    << "Where:\n"
    << "    InSampleRate  : The Input SampleRate\n"
    << "                    e.g. 44\n"
    << "    OutSampleRate : 8 or 16 (kHz)\n";
};

int main (int argc, char *argv[])
{
    int InSampleRate, OutSampleRate;

    if (argc < 3) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };
    InSampleRate  = atoi (argv [1]);
    OutSampleRate = atoi (argv [2]);

    if (OutSampleRate != 8 && OutSampleRate != 16) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };

    if (InSampleRate < OutSampleRate) {
        ShowArgErrorMessage (argv[0]);
        return 1;
    };

    new NrEncoder (InSampleRate, OutSampleRate);

    XxBlocker::MainLoop ();

    return 0;
};
