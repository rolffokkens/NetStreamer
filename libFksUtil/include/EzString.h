/*
 * file:  EzString.h
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

/*

EzString
========

EzString is a C++ Class that is intended to be a convenient alternative
for the standard C string (char *) type.

Major differences
-----------------

1) EzString is not zero-terminated
   EzString can contain all character, including '\0'. So it can be used
   to contain any kind of data, not especially character strings.
   
2) EzString automatically allocates and deallocates memory
   No more malloc en free calls to precess strings.
   ... so no more memory leaks and core dumps (if EzString itself is OK :-) )
   No more "to long strings to fit in the allocated chunk of memory"
   ... so no more complex code to handle this *shit*
   
3) EzString has powerful (overloaded) string-operators
   EzString has string operators that resemble operators from other languages,
   like '+' for concatenating strings, and Substr for selecting a part of
   a string.
   This should make your code more readable.

Peformance
----------

EzString is optimized to make sure that parameter-passing and assignment
don't result in severe performance penalties for long strings. This is realized
by breaking up the string in two parts: The data itself and just a pointer to
it. When an assignment is executed or when a parameter is passed the pointer is
copied, not the data.

Of course this introduces other problems like having to count the number of
references to the data, in order to only free the memory it occupies when no
more references exist, this is taken care of in the datatype EzCommon, which
represents the data-part of a string that can be common to several EzStrings.

How to use EzString
-------------------

Include the files EzString.h in your sources, compile EzString.cpp and link it
into your executables. Use the datatype EzString to declate an object of that
type. Example

#include "EzString.h"

....

int main (void)
{
...
    EzString String1, String2, String3;
...
    String1 = "Abcdef";
    String2 = "12345";
    String3 = String1 + String2;
};

EzString Operations
-------------------

Type conversions:
    EzString is "interchangeable" with (char *) strings. This means that
    EzString can automatically be converted from (char *) (which is
    demonstrated in the example above). 
    The other way around also works, but to (const char *), not to (char *).
    This is because the result of the conversion is a pointer to
    the data-part of the EzSting. T be a true (const char *) the Data part
    is terminated with a '\0'.
Concatenation:
    With the '+' operator EzStrings can be concatenated.
The Comparision operators:
    With the '<', '==', '>' and '!=' EzStrings can be compared
The Upper and Lower functions:
    With the Upper and Lower functions characters in an EzString can
    (all) be converted to resp. upper- and lowercase.
The Instr function:
    Substrings in a string can be found with the Instr function.
The Remove function:
    Occurences of a substring can be removed from an EzString
The Rpad function:
    The Rpad functions fills up da string on the right up to a specified
    size.

*/

#ifndef H_EZ_STRING
#define H_EZ_STRING

#define EZ_STRING_MEMCOUNT

#include <iostream>
#include <string.h>

class EzSubstr;

class EzString;

extern EzString Rpad (EzString s, unsigned int size, char c = ' ');

class EzString {
    friend void EzStringSetTrace (EzString &Str);
    friend class EzSubstr;
    friend EzString operator+ (const EzString &l, const EzString &r);
    friend int operator< (const EzString &l, const EzString &r);
    friend int operator== (const EzString &l, const EzString &r);
    friend int operator> (const EzString &l, const EzString &r);
    friend int operator!= (const EzString &l, const EzString &r);
    friend EzString Upper (const EzString &s);
    friend EzString Lower (const EzString &s);
    friend int Instr (const EzString &s1, const EzString &s2);
    friend EzString Rpad (EzString s, unsigned int size, char c);
    friend EzString Remove (const EzString &s1, const EzString &s2);
private:
    class EzCommon;

    static void memcpy (void *dst, const void *src, long amount);
    EzCommon *pCommon;

    int minus (int a, int b) const { return (a < b ? a : b); };
    int sign (const EzString &s) const;
    void LtoSTR (int num);

    void Constr (const EzString &);
public:
    std::ostream &print (std::ostream &os) const;
    std::istream &scan (std::istream &is);
    EzString (void);
    EzString (char);

    EzString (long);
    EzString (int);

    EzString (const char *);
    EzString (const char *, int);
    EzString (const EzString &);

    virtual ~EzString (void);

    EzString &operator= (const EzString &);
    EzString &operator+= (const EzString &s) {return *this = (*this + s);};

    operator long (void) const;
    operator int  (void) const {return int  (operator long ());};
    operator char (void) const {return char (operator long ());};

    EzSubstr Substr (int pos, int size);
    EzSubstr Substr (int pos);

    const char *Text (void) const;

    operator const char * () { return Text (); };

    char operator[] (int pos) const;

    unsigned int Length (void) const;

    static int GetCommonCount (void);
    static int GetStringCount (void);
    static int GetTotalCommonLength (void);
    static int GetTotalStringLength (void);
};

EzString operator+ (const EzString &l, const EzString &r);

int operator< (const EzString &l, const EzString &r);

int operator== (const EzString &l, const EzString &r);

int operator> (const EzString &l, const EzString &r);

int operator!= (const EzString &l, const EzString &r);

class EzSubstr {
    friend class EzString;
private:
    EzString *pString;
    int	pos, size;

    void constr (EzString &str, int pos, int size) {
        EzSubstr::pString	= &str;
        EzSubstr::pos	= pos;
        EzSubstr::size	= size;
    }
public:
    EzSubstr (EzString &str, int pos, int size);

    EzSubstr (EzString &str, int pos);

    EzSubstr &operator= (const EzString &str);

    EzSubstr &Substr (int pos, int size);

    EzSubstr &Substr (int pos) {return Substr (pos, EzSubstr::size);};

    operator EzString () const;

    operator long (void) const {return operator EzString ();};
    operator int  (void) const {return int  (operator long ());};
    operator char (void) const {return char (operator long ());};
};



inline std::ostream &operator<< (std::ostream &os, const EzString &str)
{
    return str.print (os);
}

inline std::istream &operator>> (std::istream &is, EzString &str)
{
    return str.scan (is);
}

inline EzSubstr EzString::Substr (int pos)
{
    return EzSubstr (*this, pos);
};

inline EzSubstr EzString::Substr (int pos, int size)
{
    return EzSubstr (*this, pos, size);
};

int Instr (const EzString &s1, const EzString &s2);
EzString Upper (const EzString &s);
EzString Lower (const EzString &s);

/* MSVC : already declared??
EzString Rpad (const EzString s, int size, char c = ' ');
*/

inline EzString Substr (EzString s, int pos, int size)
{
    // expicit Typecast for MSVC4.0 (Bug)
    // return EzString (s.Substr (pos, size));
    return s.Substr (pos, size);
}

inline EzString Substr (EzString s, int pos)
{
    // expicit Typecast for MSVC4.0 (Bug)
    // return EzString (s.Substr (pos));
    return s.Substr (pos);
}

EzString NumberToString (long number, int radix = 10);

EzString Remove (const EzString &s1, const EzString &s2);

#endif
