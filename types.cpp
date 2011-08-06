#include "types.h"

unsigned int swf::TypeBase::getN32Bits(unsigned char * ptr, unsigned int n, unsigned int startAt = 0) {
	unsigned int ret = 0;
	unsigned int i = 0;
	unsigned int pos;
	unsigned int t;
	unsigned int scale;
	
	//pointer adjustment
	ptr += (unsigned int) floor(startAt / 8);
	
	//scaling
	startAt = startAt % 8;
	n = n % 32;
	
	do {
		scale = (i + startAt) % 8;
		
		/* shift sizeof(char) * 8 - 1 scaled by i plus a starting position */
		pos = 8 - 1 - scale;
		if( i > 0 && scale == 0 )
		{
			ptr++;
		}
		
		t = *ptr >> pos;
		
		#ifdef DEBUG
		printf("%i", t & 1);
		#endif
		
		ret <<= 1;
		ret |= t & 1;
	} while (++i < n);
	
	#ifdef DEBUG
	printf("\n");
	#endif
	
	return ret;
}

//-----------------------------------------
//                   U8
//-----------------------------------------
void swf::U8::fromSWF( char *& ptr ) {
	value = ptr[0] & 0x000000ff;
	ptr++;
}

unsigned char swf::U8::toBE() {
	return value;
}

//-----------------------------------------
//                   U16
//-----------------------------------------
void swf::U16::fromSWF( char *& ptr ) {
	value = 
		ptr[0] <<  0 & 0x000000ff |
		ptr[1] <<  8 & 0x0000ff00
	;
	ptr += 2;
}

unsigned short int swf::U16::toBE() {
	return value;
}

//-----------------------------------------
//                   U32
//-----------------------------------------
void swf::U32::fromSWF( char *& ptr ) {
	value = 
		ptr[0] <<  0 & 0x000000ff |
		ptr[1] <<  8 & 0x0000ff00 |
		ptr[2] << 16 & 0x00ff0000 |
		ptr[3] << 24 & 0xff000000
	;
	ptr += 4;
}

unsigned int swf::U32::toBE() {
	return value;
}

//-----------------------------------------
//                   Twip
//-----------------------------------------

//-----------------------------------------
//                   RECT
//-----------------------------------------
void swf::RECT::fromSWF( char *& ptr ) {
	int i = 0;
	
	nBits      = TypeBase::getN32Bits((unsigned char *)ptr, 5);
	xMin.value = TypeBase::getN32Bits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	xMax.value = TypeBase::getN32Bits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	yMin.value = TypeBase::getN32Bits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	yMax.value = TypeBase::getN32Bits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	
	double shift = ceil( (5 + (double)(nBits * i)) / 8);
	ptr += (unsigned int)shift;
	
	#ifdef DEBUG
	printf("i: %i\n", i);
	printf("nBits: %i\n", nBits);
	printf("xMin: %i\n", xMin.value/20);
	printf("xMax: %i\n", xMax.value/20);
	printf("yMin: %i\n", yMin.value/20);
	printf("yMax: %i\n", yMax.value/20);
	printf("shift %f\n", shift);
	#endif
}

unsigned int swf::RECT::toBE() {
	return 0;
}