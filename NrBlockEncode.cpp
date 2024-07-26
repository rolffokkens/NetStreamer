/*
 * file: NrBlockEncode.cpp
 *
 * This file is part of the NetStreamer software. This file is distributed
 * under the GNU GENERAL PUBLIC LICENSE, see the accompanying COPYING file.
 *  
 * Copyright (C) 2024 Rolf Fokkens
 *
 */
  
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main (int argc, char *argv[])
{
    char buf[256], id;
    unsigned char lenc;
    int len;


    if (argc < 1) {
        cerr << "Missing argument ID" << endl;
        exit(1);
    }
    id = atoi(argv[1]);

    for (;;) {
        cin.read (buf, sizeof(buf));
	len = cin.gcount() ;
        if (!len) break;
	lenc = len;
	cout.write (&id, 1);
	cout.write ((char *)&lenc, 1);
	cout.write (buf, len);
        if (cin.rdstate() & ifstream::eofbit) break;
    }
}
