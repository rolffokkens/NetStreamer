#include <unistd.h>
#include <pwd.h>
#include <iostream>
#include <fstream>
#include "EzString.h"
#include "NrPresetMemory.h"

using namespace std;

static EzString GetPresetFile (void)
{
    passwd   *pPwd;

    pPwd = getpwuid (getuid());

    return EzString (pPwd->pw_dir) + EzString ("/.NetStreamer");
};

NrPresetMemory::NrPresetMemory (int Size)
{
    fstream  fStr;
    int      i;

    NrPresetMemory::Size = Size;
    NrPresetMemory::Data = new int [Size];

    for (i = 0; i < Size; i++) Data[i] = 880;

    fStr.open (GetPresetFile (), ios::in);

    i = 0;

    while (i < Size && fStr) {
        fStr >> Data[i];
        i++;
    };
};

NrPresetMemory::~NrPresetMemory (void)
{
    fstream  fStr;
    int      i;

    fStr.open (GetPresetFile (), ios::out);

    if (fStr) {
        for (i = 0; i < Size; i++) {
            fStr << Data[i] << endl;
        };
        fStr.close ();
    };

    delete [] Data;
};

void NrPresetMemory::SetVal (int Index, int  Val)
{
    if (Index >= 0 && Index < Size) Data [Index] = Val;
};

void NrPresetMemory::GetVal (int Index, int &Val)
{
    if (Index >= 0 && Index < Size) {
        Val = Data [Index];
    } else {
        Val = 880;
    };
};
