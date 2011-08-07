#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <math.h>
#include "swfamf.h"

namespace swf {

class TypeBase {
public:
	static unsigned int getUBits(unsigned char * ptr, unsigned int n, unsigned int startAt);
	static   signed int getSBits(unsigned char * ptr, unsigned int n, unsigned int startAt);
	
	virtual void fromSWF( char *& ptr) = 0;
	
	/* since fromSWF is pure abstract i don't have to define the destructor
	   to be pure abstract and can provide the default impl inline */
	virtual ~TypeBase() {};
};

//-----------------------------------------
//                   U8
//-----------------------------------------		
class U8 : public TypeBase {
	unsigned char value;
public:
	void fromSWF( char *& ptr );
	unsigned char toBE();
};

//-----------------------------------------
//                   U16
//-----------------------------------------	
class U16 : public TypeBase {
	unsigned short int value;
public:
	void fromSWF( char *& ptr );
	double toFixed8();
	unsigned short int getValue();
};

//-----------------------------------------
//                   U32
//-----------------------------------------	
class U32 : public TypeBase {
	unsigned int value;
public:
	void fromSWF( char *& ptr );
	double toFixed16();
	unsigned int getValue();
};
	
//-----------------------------------------
//                   Twip
//-----------------------------------------	
class Twip {
	
public:
	signed int value;
	signed int toPX();
};

//-----------------------------------------
//                   RECT
//-----------------------------------------
class RECT : public TypeBase {
public:
	unsigned char nBits;
	Twip xMin;
	Twip xMax;
	Twip yMin;
	Twip yMax;
	void fromSWF( char *& ptr );
	unsigned int toBE();
};

}

#endif