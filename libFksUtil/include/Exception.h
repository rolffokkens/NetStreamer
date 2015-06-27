/*
 * file:  Exception.h
 *
 * This file is distributed under the GNU GENERAL PUBLIC LICENSE.
 *
 * Copyright (C) 1997 Rolf Fokkens
 *                    fokkensr@freemail.nl
 *                    fokkensr@vertis.nl
 *
 */

#ifndef H_EXCEPTION
#define H_EXCEPTION

#include "EzString.h"

class Exception {
    EzString Text;
private:
public:
    Exception (EzString Text) {
        Exception::Text = Text;
    };
    virtual ~Exception (void) { };
    virtual EzString GetText (void) {
        return Text;
    };
};

#endif
