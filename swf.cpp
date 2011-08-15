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
	
	if (value & 0x00ff)
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
void swf::U32::fromSWF( char *& ptr ) {
    valueSet = true;
    
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

void swf::U32::setValue(unsigned int value) {
    if ( !valueSet ) {
		(*this).value = value;
	}
}

double swf::U32::toFixed16() {
	double decimal = 0;
	
	if (value & 0x0000ffff)
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
    
    unsigned int shortLen = tagCodeAndLength.getValue() & 0x3f;
    if ( shortLen == 0x3f ) {
		isShort = false;
		tagLength.fromSWF( stream );
	} else {
		isShort = true;
        tagLength.setValue( shortLen );
	}
}

short int swf::RecordHeader::type() {
    return tag;
}

unsigned swf::RecordHeader::length() {
    return tagLength.getValue();
}

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
	//(*_buffer) += 4;//want to do this but outBuf is cropped at the moment
	header.continueWith(stream);
    
    std::vector<RecordHeader> v;
    RecordHeader * t;
    short tv;
    do {
        t = new RecordHeader();
        t->fromSWF(stream);
        
        //printf("tag %i\n", t->type());
        tv = t->type();
        switch (tv) {
            case 0:
                printf("%2i End\n", tv);
                break;
            case 1:
                printf("%2i ShowFrame\n", tv);
                break;
            case 2:
                printf("%2i DefineShape\n", tv);
                break;
            case 4:
                printf("%2i PlaceObject\n", tv);
                break;
            case 5:
                printf("%2i RemoveObject\n", tv);
                break;
            case 6:
                printf("%2i DefineBits\n", tv);
                break;
            case 7:
                printf("%2i DefineButton\n", tv);
                break;
            case 8:
                printf("%2i JPEGTables\n", tv);
                break;
            case 9:
                printf("%2i SetBackgroundColor\n", tv);
                break;
            case 10:
                printf("%i DefineFont\n", tv);
                break;
            case 11:
                printf("%i DefineText\n", tv);
                break;
            case 12:
                printf("%i DoAction\n", tv);
                break;
            case 13:
                printf("%i DefineFontInfo\n", tv);
                break;
            case 14:
                printf("%i DefineSound\n", tv);
                break;
            case 15:
                printf("%i StartSound\n", tv);
                break;
            case 17:
                printf("%i DefineButtonSound\n", tv);
                break;
            case 18:
                printf("%i SoundStreamHead\n", tv);
                break;
            case 19:
                printf("%i SoundStreamBlock\n", tv);
                break;
            case 20:
                printf("%i DefineBitsLossless\n", tv);
                break;
            case 21:
                printf("%i DefineBitsJPEG2\n", tv);
                break;
            case 22:
                printf("%i DefineShape2\n", tv);
                break;
            case 23:
                printf("%i DefineButtonCxform\n", tv);
                break;
            case 24:
                printf("%i Protect\n", tv);
                break;
            case 26:
                printf("%i PlaceObject2\n", tv);
                break;
            case 28:
                printf("%i RemoveObject2\n", tv);
                break;
            case 32:
                printf("%i DefineShape3\n", tv);
                break;
            case 33:
                printf("%i DefineText2\n", tv);
                break;
            case 34:
                printf("%i DefineButton2\n", tv);
                break;
            case 35:
                printf("%i DefineBitsJPEG3\n", tv);
                break;
            case 36:
                printf("%i DefineBitsLossless2\n", tv);
                break;
            case 37:
                printf("%i DefineEditText\n", tv);
                break;
            case 39:
                printf("%i DefineSprite\n", tv);
                break;
            case 43:
                printf("%i FrameLabel\n", tv);
                break;
            case 45:
                printf("%i SoundStreamHead2\n", tv);
                break;
            case 46:
                printf("%i DefineMorphShape\n", tv);
                break;
            case 48:
                printf("%i DefineFont2\n", tv);
                break;
            case 56:
                printf("%i ExportAssets\n", tv);
                break;
            case 57:
                printf("%i ImportAssets\n", tv);
                break;
            case 58:
                printf("%i EnableDebugger\n", tv);
                break;
            case 59:
                printf("%i DoInitAction\n", tv);
                break;
            case 60:
                printf("%i DefineVideoStream\n", tv);
                break;
            case 61:
                printf("%i VideoFrame\n", tv);
                break;
            case 62:
                printf("%i DefineFontInfo2\n", tv);
                break;
            case 64:
                printf("%i EnableDebugger2\n", tv);
                break;
            case 65:
                printf("%i ScriptLimits\n", tv);
                break;
            case 66:
                printf("%i SetTabIndex\n", tv);
                break;
            case 69:
                printf("%i FileAttributes\n", tv);
                break;
            case 70:
                printf("%i PlaceObject3\n", tv);
                break;
            case 71:
                printf("%i ImportAssets2\n", tv);
                break;
            case 73:
                printf("%i DefineFontAlignZones\n", tv);
                break;
            case 74:
                printf("%i CSMTextSettings\n", tv);
                break;
            case 75:
                printf("%i DefineFont3\n", tv);
                break;
            case 76:
                printf("%i SymbolClass\n", tv);
                break;
            case 77:
                printf("%i Metadata\n", tv);
                break;
            case 78:
                printf("%i DefineScalingGrid\n", tv);
                break;
            case 82:
                printf("%i DoABC\n", tv);
                break;
            case 83:
                printf("%i DefineShape4\n", tv);
                break;
            case 84:
                printf("%i DefineMorphShape2\n", tv);
                break;
            case 86:
                printf("%i DefineSceneAndFrameLabelData\n", tv);
                break;
            case 87:
                printf("%i DefineBinaryData\n", tv);
                break;
            case 88:
                printf("%i DefineFontName\n", tv);
                break;
            case 89:
                printf("%i StartSound2\n", tv);
                break;
            case 90:
                printf("%i DefineBitsJPEG4\n", tv);
                break;
            case 91:
                printf("%i DefineFont4\n", tv);
                break;
                
            default:
                printf("\nEncountered unknown tag\n");
                break;
        }
        
        
        stream += t->length();
        
        v.push_back( *t );
    } while ( t->type() != 0 );
}

void swf::SWF::toSWF() {}
