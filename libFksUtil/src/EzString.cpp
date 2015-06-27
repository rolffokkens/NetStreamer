/*
 * file:  EzString.cpp
 *
 * This file is distributed under the GNU GENERAL PUBLIC LICENSE.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *                    fokkensr@freemail.nl
 *                    fokkensr@vertis.nl
 *
 * Rel  When   who  What
 * 0.9  980807 Rolf Moved the stuff about EzCommon from EzStrimg.cpp to
 *                  EzString.h
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>

#include "EzString.h"

using namespace std;

#ifdef EZ_STRING_MEMCOUNT

static int    EzCommonCount       = 0;
static int    EzStringCount       = 0;
static size_t EzCommonTotalLength = 0;
static size_t EzStringTotalLength = 0;

inline void IncCommonCount () { EzCommonCount++; };

inline void DecCommonCount () { EzCommonCount--; };

inline void IncStringCount () { EzStringCount++; };

inline void DecStringCount () { EzStringCount--; };

inline void AddCommonLength (size_t Length) { EzCommonTotalLength += Length; };

inline void SubCommonLength (size_t Length) { EzCommonTotalLength -= Length; };

int EzString::GetStringCount (void) { return EzStringCount; };

int EzString::GetCommonCount (void) { return EzCommonCount; };

int EzString::GetTotalStringLength (void) { return EzStringTotalLength; };
int EzString::GetTotalCommonLength (void) { return EzCommonTotalLength; };

#else

inline void IncCommonCount () { };

inline void DecCommonCount () { };

inline void IncStringCount () { };

inline void DecStringCount () { };

inline void AddLenght (size_t Length) { };

inline void SubLenght (size_t Length) { };

int EzString::GetStringCount (void) { return 0; };

int EzString::GetCommonCount (void) { return 0; };

int EzString::GetTotalStringLength (void) { return 0; };

int EzString::GetTotalCommonLength (void) { return 0; };

#endif

class EzString::EzCommon {
private:
    void constr (const char *s, unsigned int size);
    void constr (unsigned int size);
public:
    int          pCount; // signed for error detection
    unsigned int size;
    char         *text;
    EzCommon (void) {constr (0);};
    EzCommon (unsigned int size) {constr (size);};
    EzCommon (char);
    EzCommon (const char *s);
    EzCommon (const char *s, unsigned int size);
    ~EzCommon (void);

    static void memcpy (void *dst, const void *src, long amount) {
        ::memcpy (dst, src, amount);
    };

    char operator[] (unsigned int pos) const {
        return (pos < size ? text[pos] : '\0');
    };

    static void disconnect (EzCommon *&pCommon);
};

void EzString::EzCommon::constr (unsigned int size)
{
    EzCommon::pCount = 1;
    EzCommon::size   = size;
    EzCommon::text   = new char [size + 1];

    AddCommonLength (size);
    IncCommonCount ();
}

void EzString::EzCommon::constr (const char *s, unsigned int size)
{
    constr (size);

    memcpy (text, s, size);
}

EzString::EzCommon::EzCommon (char c)
{
    constr (&c, 1);
}

EzString::EzCommon::EzCommon (const char *s, unsigned int size)
{
    constr (s, size);
}

EzString::EzCommon::EzCommon (const char *s)
{
    if (s == NULL) {
	constr ("", 0);
    } else {
	constr (s, strlen (s));
    };
}

EzString::EzCommon::~EzCommon (void)
{
    if (text != NULL) {
        delete [] text;
        SubCommonLength (size);
    };

    DecCommonCount ();
}

void EzString::memcpy (void *dst, const void *src, long amount)
{
    EzCommon::memcpy (dst, src, amount);
}

const char *EzString::Text (void) const
{
    pCommon->text[pCommon->size] = 0;

    return pCommon->text;
};

char EzString::operator[] (int pos) const
{
    return (*pCommon)[pos];
};

unsigned int EzString::Length (void) const
{
    return pCommon->size;
};

EzString::EzString (void)
{
    pCommon = new EzCommon;

    IncStringCount ();
}

EzString::EzString (char c)
{
    pCommon = new EzCommon (c);

    IncStringCount ();
}

EzString::EzString (const char *s)
{
    pCommon = new EzCommon (s);

    IncStringCount ();
}

EzString::EzString (const char *s, int size)
{
    pCommon = new EzCommon (s, size);

    IncStringCount ();
}

void EzString::Constr (const EzString &s)
{
    pCommon = s.pCommon;

    pCommon->pCount++;
}

EzString::EzString (const EzString &s)
{
    Constr (s);

    IncStringCount ();
}

void EzString::EzCommon::disconnect (EzCommon *&pCommon)
{
    int retval;

    if (pCommon == NULL || pCommon->pCount == 0) {
        if (pCommon == NULL) {
            cout << "disconnect while pCommon == NULL" << endl;
        };

        if (pCommon->pCount == 0) {
            cout << "disconnect while count == 0" << endl;
        };

        exit (1);
    }

    retval = (--(pCommon->pCount) == 0);

    if (retval) {
        delete pCommon;
    }
    pCommon = NULL;
}

EzString::~EzString (void)
{
    EzCommon::disconnect (pCommon);

    DecStringCount ();
};

ostream &EzString::print (ostream &os) const
{
    char *cp;
    int  siz;

    cp	= pCommon->text;
    siz	= pCommon->size;

    while (siz--) {
        if (*cp >= ' ' || *cp == 10 || *cp == 13) {
            if (*cp == '\\') os << "\\";
            os << *cp++;
        } else {
            os << "\\" << setw (3) << setfill ('0') << dec << (*cp++ & 0x00ff);
        }
    }

    return os;
}

istream &EzString::scan (istream &is)
{
	char	ch, *buf;
	int		bufsize, cursize;

	cursize = 0;
	bufsize	= 1;
	buf		= new char [bufsize];

	while (is.get (ch)) {
		if (ch <= ' ') {
			is.putback (ch);
			break;
		}

		if (cursize == bufsize) {
			char *tmp = new char [bufsize * 2];

			memcpy (tmp, buf, bufsize);

			delete [] buf;
			buf		 = tmp;
			bufsize *= 2;
		}

		buf[cursize++] = ch;
	}

	while (is.get (ch)) {
		if (ch > ' ') {
			is.putback (ch);
			break;
		}
	}

	EzCommon::disconnect (pCommon);

	pCommon = new EzCommon (buf, cursize);

	delete [] buf;

	return is;
}

EzString &EzString::operator= (const EzString &s)
{
    EzCommon *tCommon;

    tCommon = s.pCommon;
    tCommon->pCount++;

    EzCommon::disconnect (pCommon);
    pCommon = tCommon;

    return *this;
}

int EzString::sign (const EzString &s) const
{
	int tsiz, sgn;

	if (pCommon == s.pCommon) return 0;

	tsiz = minus (pCommon->size, s.pCommon->size);

	if ((sgn = memcmp (pCommon->text, s.pCommon->text, tsiz)) != 0) {
		return sgn;
	}

	return ( pCommon->size == s.pCommon->size
		   ? 0 : (pCommon->size < s.pCommon->size ? -1 : 1));
};

EzString operator+ (const EzString &l, const EzString &r)
{
    EzString RetVal;
    char  *pBuf;
    int   Size;

    Size = l.pCommon->size + r.pCommon->size;
    pBuf = new char [Size];
    
    memcpy ( pBuf                  , l.pCommon->text, l.pCommon->size);
    memcpy ( pBuf + l.pCommon->size, r.pCommon->text, r.pCommon->size);

    RetVal = EzString (pBuf, Size);

    delete pBuf;

    return RetVal;
/*
	EzString::EzCommon::disconnect (retval.pCommon);

	retval.pCommon = new EzString::EzCommon (l.pCommon->size + r.pCommon->size);

	memcpy ( retval.pCommon->text
		   , l.pCommon->text
		   , l.pCommon->size
		   );
	memcpy ( retval.pCommon->text + l.pCommon->size
		   , r.pCommon->text
		   , r.pCommon->size
		   );
	return retval;
*/
}

int operator< (const EzString &l, const EzString &r)
{
	return (l.sign (r) < 0);
};

int operator== (const EzString &l, const EzString &r)
{
	return (l.sign (r) == 0);
};

int operator> (const EzString &l, const EzString &r)
{
	return (l.sign (r) > 0);
};

int operator!= (const EzString &l, const EzString &r)
{
	return (l.sign (r) != 0);
};

int Instr (const EzString &s1, const EzString &s2)
{
	unsigned int i;
	EzString     s1c;

	if (s2 == EzString ("")) return -1;

	s1c = s1;

	for (i = 0; i < (s1.Length () - s2.Length () + 1); i++) {
		if (Substr (s1c, i, s2.Length ()) == s2) return i;
	};

	return -1;
};

EzString Remove (const EzString &s1, const EzString &s2)
{
    EzString     retval;
    unsigned int i;

    for (i = 0; i < s1.Length (); i++) {
        if (Instr (s2, s1[i]) == -1) retval += s1[i];
	};

    return retval;
};



EzString Upper (const EzString &s)
{
	EzString     retval;
	char         *p1, *p2;
	unsigned int size = s.pCommon->size;

	EzString::EzCommon::disconnect (retval.pCommon);

	retval.pCommon = new EzString::EzCommon (size);
	p2 = retval.pCommon->text;
	p1 = s.pCommon->text;

	while (size-- != 0) *p2++ = toupper (*p1++);

	return retval;
};



EzString Lower (const EzString &s)
{
	EzString     retval;
	char         *p1, *p2;
	unsigned int size = s.pCommon->size;

	EzString::EzCommon::disconnect (retval.pCommon);

	retval.pCommon = new EzString::EzCommon (size);

	p2 = retval.pCommon->text;
	p1 = s.pCommon->text;

	while (size-- != 0) *p2++ = tolower (*p1++);

	return retval;
};



EzString Rpad (EzString s, int unsigned size, char)
{
	EzString     retval;
	unsigned int tSize;

	EzString::EzCommon::disconnect (retval.pCommon);
	retval.pCommon = new EzString::EzCommon (size);

	tSize = s.pCommon->size;

	if (tSize > size) tSize = size;

	memcpy (retval.pCommon->text, s.pCommon->text, tSize);
	memset (retval.pCommon->text + tSize, ' ', size - tSize);

	return retval;
}


EzSubstr::EzSubstr (EzString &str, int pos, int size)
{
    constr (str, pos, size);
}

EzSubstr::EzSubstr (EzString &str, int pos)
{
    constr (str, pos, str.pCommon->size);
}


EzSubstr::operator EzString () const
{
    EzString     retval;
    int	       tSize, pos;

    pos = EzSubstr::pos;

    if (pos < 0) {
        pos += pString->Length ();

        if (pos < 0) pos = 0;
    };

    tSize = pString->Length () - pos;

    if (size < tSize) tSize = size;

    if (tSize < 0) tSize = 0;

    retval = EzString (pString->Text () + pos, tSize);

    return retval;
};



EzSubstr &EzSubstr::Substr (int pos, int size)

{
	int tsize;

	if (pos < 0) {
	    if (pos < 0) pos = 0;
    };

	tsize = EzSubstr::size - pos;

	if (tsize > size) tsize = size;

	EzSubstr::size = tsize;
	EzSubstr::pos += pos;

	return *this;
}



EzSubstr &EzSubstr::operator= (const EzString &str)
{
	int s_size = pString->Length ();

	if (pos > s_size) pos = s_size;

	if (pos + size > s_size) size = s_size - pos;

	EzString retval;

	*pString =   ::Substr (*pString, 0, pos)
                   + str
                   + ::Substr (*pString, pos + size);

	return *this;
};



EzString NumberToString (long number, int radix)
{
    int i, sign;
    static char buf[100];
    char *cp = buf + 100;

    if ((sign = number) < 0) number = -number;

    i     = 0;
    *--cp = '\0';

    do {
        *--cp = number % radix + '0';
    } while ((number /= radix) > 0);

    if (sign < 0) *--cp = '-';

    return EzString (cp);
};

EzString::operator long (void) const
{
    return atol (Text ());
};

EzString::EzString (long num)
{
    EzString Temp;

    Temp = NumberToString (num);

    Constr (Temp);

    IncStringCount ();
};

EzString::EzString (int  num)
{
    EzString Temp;

    Temp = NumberToString (num);

    Constr (Temp);

    IncStringCount ();
};
