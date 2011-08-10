#include "swf.h"

//-----------------------------------------
//                 AbstractBase
//-----------------------------------------
swf::AbstractBase::~AbstractBase() {}

//-----------------------------------------
//                 AbstractData
//-----------------------------------------
unsigned int swf::AbstractData::getUBits(unsigned char * ptr, unsigned int n, unsigned int startAt = 0) {
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

signed int swf::AbstractData::getSBits(unsigned char * ptr, unsigned int n, unsigned int startAt = 0) {
	unsigned int ret = AbstractData::getUBits(ptr, n, startAt);
	
	if ( ret & (1<<(n-1)) )
	{//msb is 1
		ret |= ((~0) << n);
	}
	
	return ret;
}

swf::AbstractData::~AbstractData() {}

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

unsigned short int swf::U16::getValue() {
	return value;
}

double swf::U16::toFixed8() {
	double decimal = 0;
	
	if (value && 0x00ff)
	{
		decimal = (double)(value & 0x00ff) / pow(10, ceil(log10(pow(2, 8))));
	}
	
	return ((value & 0xff00) >> 8) + decimal;
}

float swf::U16::toFloat() {
	unsigned ret = 0;
	//int s = (value << 16) & 0x80000000;
	//int e = (value >> 10) & 0x1f;
	//int m = value & 0x3ff;
	
	
	
	return ret;
}

//-----------------------------------------
//                   U32
//-----------------------------------------
swf::U32::U32(unsigned value) {
	if (value) {
		(*this).value = value;
	}
}

void swf::U32::fromSWF( char *& ptr ) {
	value = 
	ptr[0] <<  0 & 0x000000ff |
	ptr[1] <<  8 & 0x0000ff00 |
	ptr[2] << 16 & 0x00ff0000 |
	ptr[3] << 24 & 0xff000000
	;
	ptr += 4;
}

unsigned int swf::U32::getValue() {
	return value;
}

double swf::U32::toFixed16() {
	double decimal = 0;
	
	if (value && 0x0000ffff)
	{
		decimal = (double)(value & 0x0000ffff) / pow(10, ceil(log10(pow(2, 16))));
	}
	
	return ((value & 0xffff0000) >> 16) + decimal;
}

float swf::U32::toFloat() {
	return *(float *)&value;
}

//-----------------------------------------
//                   Twip
//-----------------------------------------
signed int swf::Twip::toPX() {
	return value/20;
}

//-----------------------------------------
//                   RECT
//-----------------------------------------
void swf::RECT::fromSWF( char *& ptr ) {
	int i = 0;
	
	nBits      = AbstractData::getUBits((unsigned char *)ptr, 5);
	xMin.value = AbstractData::getSBits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	xMax.value = AbstractData::getSBits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	yMin.value = AbstractData::getSBits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	yMax.value = AbstractData::getSBits((unsigned char *)ptr, nBits, 5 + nBits * i++);
	
	double shift = ceil( (5 + (double)(nBits * i)) / 8);
	ptr += (unsigned int)shift;
	
#ifdef DEBUG
	printf("i: %i\n", i);
	printf("nBits: %i\n", nBits);
	printf("xMin: %i\n", xMin.toPX());
	printf("xMax: %i\n", xMax.toPX());
	printf("yMin: %i\n", yMin.toPX());
	printf("yMax: %i\n", yMax.toPX());
	printf("shift %f\n", shift);
#endif
}

unsigned int swf::RECT::toBE() {
	return 0;
}

//-----------------------------------------
//                RecordHeader
//-----------------------------------------
void swf::RecordHeader::fromSWF( char *& stream ) {
	tagCodeAndLength.fromSWF(stream);
	
	tag = (tagCodeAndLength.getValue() >> 6) & 0x3ff;
	
	/*
	 check the last 6 bits of U16
	 All 1's means this is a long header
	 */
	if ( ~(tagCodeAndLength.getValue() && 0x3f) == 0 ) {
		isShort = false;
		tagLength.fromSWF(stream);
	} else {
		isShort = true;
	}
}
/*class RecordHeader {
 U16 tcal;//TagCodeAndLength
 U32 length;
 bool isShort;
 public:
 void fromSWF( char * stream );
 short int type();
 unsigned int length();
 };*/


//-----------------------------------------
//                AbstractTag
//-----------------------------------------
swf::AbstractTag::~AbstractTag() {}

//-----------------------------------------
//                SWFHeader
//-----------------------------------------
void swf::SWFHeader::fromSWF( char *& stream) {
	_compressed = stream[0] == 'C';
	
	stream += 3;//skip 'W' and 'F'
	_version.fromSWF(stream);
	_fileLength.fromSWF(stream);
}

void swf::SWFHeader::continueWith(char *& stream) {
	_frameSize.fromSWF(stream);
	_frameRate.fromSWF(stream);
	_frameCount.fromSWF(stream);
	
	#ifdef DEBUG
	printf("fileLength %i\n", _fileLength.getValue());
	printf("frameRate %f\n", _frameRate.toFixed8());
	printf("frameCount %i\n", _frameCount.getValue());
	#endif
}

swf::SWFHeader::~SWFHeader() {}

swf::U32 swf::SWFHeader::fileLength() {
	return _fileLength;
}

bool swf::SWFHeader::compressed() {
	return _compressed;
}

//-----------------------------------------
//                  SWF
//-----------------------------------------
void swf::SWF::fromSWF(char *& stream) {
	_buffer = &stream;
	header.fromSWF(stream);
}

void swf::SWF::continueWith(char *& stream) {
	_buffer = &stream;
	//(*_buffer) += 4;//want to do this because outBuf is cropped at the moment
	header.continueWith(stream);
}

void swf::SWF::toSWF() {}
