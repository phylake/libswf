#include "swf.h"

unsigned int swf::getUBits(buf_type * buf, unsigned int n, unsigned int startAt = 0) {
	unsigned int ret = 0;
	unsigned int i = 0;
	unsigned int pos;
	unsigned int t;
	unsigned int scale;
	
	//pointer adjustment
	buf += (unsigned int) floor(startAt / 8);
	
	//scaling
	startAt = startAt % 8;
	n = n % 32;
	
	do {
		scale = (i + startAt) % 8;
		
		/* shift sizeof(char) * 8 - 1 scaled by i plus a starting position */
		pos = 8 - 1 - scale;
		if( i > 0 && scale == 0 )
		{
			buf++;
		}
		
		t = *buf >> pos;
		
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

unsigned int swf::getUBits(char * buf, unsigned int n, unsigned int startAt = 0) {
    return getUBits((buf_type *)buf, n, startAt);
}

signed int swf::getSBits(buf_type * buf, unsigned int n, unsigned int startAt = 0) {
	unsigned int ret = getUBits(buf, n, startAt);
	
	if ( ret & (1<<(n-1)) )
	{//msb is 1
		ret |= ((~0) << n);
	}
	
	return ret;
}

signed int swf::getSBits(char * buf, unsigned int n, unsigned int startAt = 0) {
    return getSBits((buf_type *)buf, n, startAt);
}

//----------------------------------------------------
//                       Versioning
//----------------------------------------------------
unsigned char swf::Version::version() {
    return value;
}

unsigned short swf::Version::versionNum() {
    return (unsigned short)value;
}

void swf::MutableVersion::setVersion(unsigned char version) {
    value = version;
}

void swf::MutableVersion::setVersion(buf_type *& buf) {
    setVersion(*buf);
    buf++;
}

swf::VersionRequirement::VersionRequirement(Version & version) : _version(version) {}

//-----------------------------------------
//                 AbstractBase
//-----------------------------------------
swf::AbstractBase::~AbstractBase() {}

//-----------------------------------------
//                 AbstractData
//-----------------------------------------
swf::AbstractData::~AbstractData() {}

//-----------------------------------------
//         AbstractVData
//-----------------------------------------
swf::AbstractVData::AbstractVData(Version & version) : VersionRequirement(version) {}

swf::AbstractVData::~AbstractVData() {}
//-----------------------------------------
//                   U8
//-----------------------------------------
void swf::U8::fromSWF( buf_type *& buf ) {
	value = buf[0] & 0x000000ff;
	buf++;
}

unsigned char swf::U8::getValue() {
	return value;
}

//-----------------------------------------
//                   U16
//-----------------------------------------
void swf::U16::fromSWF( buf_type *& buf ) {
	value = 
        buf[0] << 0 & 0x000000ff |
        buf[1] << 8 & 0x0000ff00
	;
	buf += 2;
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
void swf::U32::fromSWF( buf_type *& buf ) {
    value = 
        buf[0] <<  0 & 0x000000ff |
        buf[1] <<  8 & 0x0000ff00 |
        buf[2] << 16 & 0x00ff0000 |
        buf[3] << 24 & 0xff000000
	;
	buf += 4;
}

unsigned int swf::U32::getValue() {
	return value;
}

void swf::U32::setValue(unsigned int value) {
    (*this).value = value;
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
//                 String
//-----------------------------------------
void swf::String::fromSWF( buf_type *& buf ) {
    value.assign((char *)buf);
    buf += value.length();
}

//-----------------------------------------
//                   Twip
//-----------------------------------------
signed int swf::Twip::toPX() {
	return value/20;
}

//-----------------------------------------
//                   RGB
//-----------------------------------------
swf::RGB::RGB( int type ) {
    (*this).type = type;
}

void swf::RGB::fromSWF( buf_type *& buf ) {
    value.fromSWF(buf);
    
    if( type == 0 ) buf--;//only 3 bytes were relevant
}

//-----------------------------------------
//                   RECT
//-----------------------------------------
void swf::RECT::fromSWF( buf_type *& buf ) {
	int i = 0;
	
	nBits      = getUBits(buf, 5);
	xMin.value = getSBits(buf, nBits, 5 + nBits * i++);
	xMax.value = getSBits(buf, nBits, 5 + nBits * i++);
	yMin.value = getSBits(buf, nBits, 5 + nBits * i++);
	yMax.value = getSBits(buf, nBits, 5 + nBits * i++);
	
	double shift = ceil( (5 + (double)(nBits * i)) / 8);
	buf += (unsigned int)shift;
	
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
//                   MATRIX
//-----------------------------------------
void swf::MATRIX::fromSWF(buf_type *& buf) {
    int i;
    unsigned offset = 0;
    signed sTemp;
    
    hasScale = *buf & 0x80;
    hasScale = getUBits(buf, 1, offset);
    if ( hasScale ) {
        i = 0;
        
        nScaleBits = (*buf >> 2) & 0x1f;
        offset += 6;//HasScale UB[1] + NScaleBits UB[5]
        
        sTemp = getSBits(buf, nScaleBits, offset + nScaleBits * i++);
        scaleX.setValue( sTemp );
        
        sTemp = getSBits(buf, nScaleBits, offset + nScaleBits * i++);
        scaleY.setValue( sTemp );
        
        offset += nScaleBits * i;
    }
    
    hasRotate = getUBits(buf, 1, offset);
    if ( hasRotate ) {
        i = 0;
        
        nRotateBits = *buf & 0x7c;
        offset += 6;//HasRotate UB[1] + NRotateBits UB[5]
        
        sTemp= getSBits(buf, nRotateBits, offset + nRotateBits * i++);
        rotateSkew0.setValue( sTemp );
        
        sTemp = getSBits(buf, nRotateBits, offset + nRotateBits * i++);
        rotateSkew1.setValue( sTemp );
    }
    
}

//-----------------------------------------
//                   CXFORM
//-----------------------------------------
void swf::CXFORM::fromSWF(buf_type *& buf) {
    int i = 0;
    hasAddTerms  = getUBits(buf, 1, i++);
    hasMultTerms = getUBits(buf, 1, i++);
    nBits        = getUBits(buf, 4, i++);
    
    i = 6;
    if( hasMultTerms ) {
        rm = getUBits(buf, 4, i);
        i += nBits;
        gm = getUBits(buf, 4, i);
        i += nBits;
        bm = getUBits(buf, 4, i);
        i += nBits;
    }
    
    if( hasAddTerms ) {
        ra = getUBits(buf, 4, i);
        i += nBits;
        ga = getUBits(buf, 4, i);
        i += nBits;
        ba = getUBits(buf, 4, i);
        i += nBits;
    }
    
    buf += (i / 8);
}

//-----------------------------------------
//              CXFORMWITHALPHA
//-----------------------------------------
void swf::CXFORMWITHALPHA::fromSWF(buf_type *& buf) {
    int i = 0;
    hasAddTerms  = getUBits(buf, 1, i++);
    hasMultTerms = getUBits(buf, 1, i++);
    nBits        = getUBits(buf, 4, i++);
    
    i = 6;
    if( hasMultTerms ) {
        rm = getUBits(buf, nBits, i);
        i += nBits;
        gm = getUBits(buf, nBits, i);
        i += nBits;
        bm = getUBits(buf, nBits, i);
        i += nBits;
        am = getUBits(buf, nBits, i);
        i += nBits;
    }
    
    if( hasAddTerms ) {
        ra = getUBits(buf, nBits, i);
        i += nBits;
        ga = getUBits(buf, nBits, i);
        i += nBits;
        ba = getUBits(buf, nBits, i);
        i += nBits;
        aa = getUBits(buf, nBits, i);
        i += nBits;
    }
    
    buf += (i / 8);
}

//-----------------------------------------
//              ActionRecord
//-----------------------------------------
void swf::ActionRecord::fromSWF( buf_type *& buf ) {
}

//-----------------------------------------
//              ClipEventFlags
//-----------------------------------------
swf::ClipEventFlags::ClipEventFlags(Version & version) : AbstractVData(version) {}

void swf::ClipEventFlags::fromSWF( buf_type *& buf ) {
    int i = 0;
    keyUp          = getUBits(buf, 1, i++);
    keyDown        = getUBits(buf, 1, i++);
    mouseUp        = getUBits(buf, 1, i++);
    mouseDown      = getUBits(buf, 1, i++);
    mouseMove      = getUBits(buf, 1, i++);
    unload         = getUBits(buf, 1, i++);
    enterFrame     = getUBits(buf, 1, i++);
    load           = getUBits(buf, 1, i++);
    dragOver       = getUBits(buf, 1, i++);
    rollOut        = getUBits(buf, 1, i++);
    rollOver       = getUBits(buf, 1, i++);
    releaseOutside = getUBits(buf, 1, i++);
    release        = getUBits(buf, 1, i++);
    press          = getUBits(buf, 1, i++);
    initialize     = getUBits(buf, 1, i++);
    data           = getUBits(buf, 1, i++);
}

//-----------------------------------------
//              ClipActionRecord
//-----------------------------------------
void swf::ClipActionRecord::fromSWF( buf_type *& buf ) {
    eventFlags.fromSWF(buf);
    size.fromSWF(buf);
    keyCode.fromSWF(buf);
    //TODO actions vector
}

//-----------------------------------------
//              ClipActions
//-----------------------------------------
swf::ClipActions::ClipActions(Version & version) : AbstractVData(version), allEventFlags(version) {}

void swf::ClipActions::fromSWF( buf_type *& buf ) {
    reserved.fromSWF(buf);
    allEventFlags.fromSWF(buf);
    
    //TODO records vector
    
    //TODO 5 or 6?
    //endFlag5.fromSWF(buf);
    //endFlag6.fromSWF(buf);
}

//-----------------------------------------
//                RecordHeader
//-----------------------------------------
void swf::RecordHeader::fromSWF( buf_type *& buf ) {
	tagCodeAndLength.fromSWF(buf);
	
	tag = (tagCodeAndLength.getValue() >> 6) & 0x3ff;
	
	/*
	 check the last 6 bits of U16
	 All 1's means this is a long header
	 */
    
    unsigned int shortLen = tagCodeAndLength.getValue() & 0x3f;
    if ( shortLen == 0x3f ) {
		isShort = false;
		tagLength.fromSWF( buf );
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
void swf::SWFHeader::fromSWF(buf_type *& buf) {
	_compressed = buf[0] == 'C';
	
	buf += 3;//skip 'W' and 'F'
	__version.fromSWF(buf);
	_fileLength.fromSWF(buf);
}

void swf::SWFHeader::continueWith(buf_type *& buf) {
	_frameSize.fromSWF(buf);
	_frameRate.fromSWF(buf);
	_frameCount.fromSWF(buf);
	
	#ifdef DEBUG
    printf("SWFHeader\n");
    printf("\t%i compressed\n", _compressed);
    printf("\t%i version\n", __version.getValue());
	printf("\t%i fileLength\n", _fileLength.getValue());
    //printf("\t%i frameSize\n", _frameSize.getValue());
	printf("\t%f frameRate\n", _frameRate.toFixed8());
	printf("\t%i frameCount\n", _frameCount.getValue());
	#endif
}

bool swf::SWFHeader::compressed() {
    return _compressed;
}

swf::U8 swf::SWFHeader::version() {
    return __version;
}

swf::U32 swf::SWFHeader::fileLength() {
	return _fileLength;
}

swf::RECT swf::SWFHeader::frameSize() {
    return _frameSize;
}

swf::U16 swf::SWFHeader::frameRate() {
    return _frameRate;
}

swf::U16 swf::SWFHeader::frameCount() {
    return _frameCount;
}

//-----------------------------------------
//             1 ShowFrame
//-----------------------------------------
void swf::ShowFrame::fromSWF(buf_type *& buf) {
#ifdef DEBUG
    printf(" 1 ShowFrame\n");
#endif
}

//-----------------------------------------
//             9 SetBackgroundColor
//-----------------------------------------
void swf::SetBackgroundColor::fromSWF(buf_type *& buf) {
    color.fromSWF(buf);
    
#ifdef DEBUG
    printf(" 9 SetBackgroundColor\n");
#endif

}

//-----------------------------------------
//             26 PlaceObject2
//-----------------------------------------
void swf::PlaceObject2::fromSWF(buf_type *& buf) {
    int i = 0;
    hasClipActions    = getUBits(buf, 1, i++);
    hasClipDepth      = getUBits(buf, 1, i++);
    hasName           = getUBits(buf, 1, i++);
    hasRatio          = getUBits(buf, 1, i++);
    hasColorTransform = getUBits(buf, 1, i++);
    hasMatrix         = getUBits(buf, 1, i++);
    hasCharacter      = getUBits(buf, 1, i++);
    moves             = getUBits(buf, 1, i++);
    
    depth.fromSWF(buf);
    if( hasCharacter ) characterId.fromSWF(buf);
    if( hasMatrix ) matrix.fromSWF(buf);
    if( hasColorTransform ) colorTransform.fromSWF(buf);
    if( hasRatio ) ratio.fromSWF(buf);
    if( hasName ) name.fromSWF(buf);
    if( hasClipDepth ) clipDepth.fromSWF(buf);
    if( hasClipActions ) clipActions.fromSWF(buf);
}

//-----------------------------------------
//             69 FileAttributes
//-----------------------------------------
void swf::FileAttributes::fromSWF(buf_type *& buf) {
    _useDirectBlit = *buf & 0x40;
    _useGPU        = *buf & 0x20;
    _hasMetadata   = *buf & 0x10;
    _isAS3         = *buf & 0x08;
    _useNetwork    = *buf & 0x01;
    
    buf += 4;
    
    #ifdef DEBUG
    printf("69 FileAttributes\n");
	printf("\t%i useDirectBlit\n", _useDirectBlit);
	printf("\t%i useGPU\n",        _useGPU);
	printf("\t%i hasMetadata\n",   _hasMetadata);
    printf("\t%i isAS3\n",         _isAS3);
    printf("\t%i useNetwork\n",    _useNetwork);
    #endif
}

//-----------------------------------------
//                  SWF
//-----------------------------------------
void swf::SWF::fromSWF(buf_type *& buf) {
	_buffer = &buf;
	header.fromSWF(buf);
}

void swf::SWF::continueWith(buf_type *& buf) {
	_buffer = &buf;
	//(*_buffer) += 4;//want to do this but outBuf is cropped at the moment
	header.continueWith(buf);
    
    std::vector<RecordHeader> vrh;
    RecordHeader * rh;
    
    std:vector<AbstractTag> vt;
    AbstractTag * t;
    short tv;
    do {
        rh = new RecordHeader();
        rh->fromSWF(buf);
        
        tv = rh->type();
        switch (tv) {
            case 0:
                printf("%2i End\n", tv);
                buf += rh->length();
                break;
            case 1:
                //printf("%2i ShowFrame\n", tv);
                //buf += rh->length();
                
                t = new ShowFrame;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 2:
                printf("%2i DefineShape\n", tv);
                buf += rh->length();
                break;
            case 4:
                printf("%2i PlaceObject\n", tv);
                buf += rh->length();
                break;
            case 5:
                printf("%2i RemoveObject\n", tv);
                buf += rh->length();
                break;
            case 6:
                printf("%2i DefineBits\n", tv);
                buf += rh->length();
                break;
            case 7:
                printf("%2i DefineButton\n", tv);
                buf += rh->length();
                break;
            case 8:
                printf("%2i JPEGTables\n", tv);
                buf += rh->length();
                break;
            case 9:
                //printf("%2i SetBackgroundColor\n", tv);
                //buf += rh->length();
                
                t = new SetBackgroundColor;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 10:
                printf("%i DefineFont\n", tv);
                buf += rh->length();
                break;
            case 11:
                printf("%i DefineText\n", tv);
                buf += rh->length();
                break;
            case 12:
                printf("%i DoAction\n", tv);
                buf += rh->length();
                break;
            case 13:
                printf("%i DefineFontInfo\n", tv);
                buf += rh->length();
                break;
            case 14:
                printf("%i DefineSound\n", tv);
                buf += rh->length();
                break;
            case 15:
                printf("%i StartSound\n", tv);
                buf += rh->length();
                break;
            case 17:
                printf("%i DefineButtonSound\n", tv);
                buf += rh->length();
                break;
            case 18:
                printf("%i SoundbufHead\n", tv);
                buf += rh->length();
                break;
            case 19:
                printf("%i SoundbufBlock\n", tv);
                buf += rh->length();
                break;
            case 20:
                printf("%i DefineBitsLossless\n", tv);
                buf += rh->length();
                break;
            case 21:
                printf("%i DefineBitsJPEG2\n", tv);
                buf += rh->length();
                break;
            case 22:
                printf("%i DefineShape2\n", tv);
                buf += rh->length();
                break;
            case 23:
                printf("%i DefineButtonCxform\n", tv);
                buf += rh->length();
                break;
            case 24:
                printf("%i Protect\n", tv);
                buf += rh->length();
                break;
            case 26:
                printf("%i PlaceObject2\n", tv);
                buf += rh->length();
                
                //t = new PlaceObject2;
                //t -> recordHeader = rh;
                //t -> fromSWF(buf);
                break;
            case 28:
                printf("%i RemoveObject2\n", tv);
                buf += rh->length();
                break;
            case 32:
                printf("%i DefineShape3\n", tv);
                buf += rh->length();
                break;
            case 33:
                printf("%i DefineText2\n", tv);
                buf += rh->length();
                break;
            case 34:
                printf("%i DefineButton2\n", tv);
                buf += rh->length();
                break;
            case 35:
                printf("%i DefineBitsJPEG3\n", tv);
                buf += rh->length();
                break;
            case 36:
                printf("%i DefineBitsLossless2\n", tv);
                buf += rh->length();
                break;
            case 37:
                printf("%i DefineEditText\n", tv);
                buf += rh->length();
                break;
            case 39:
                printf("%i DefineSprite\n", tv);
                buf += rh->length();
                break;
            case 43:
                printf("%i FrameLabel\n", tv);
                buf += rh->length();
                break;
            case 45:
                printf("%i SoundbufHead2\n", tv);
                buf += rh->length();
                break;
            case 46:
                printf("%i DefineMorphShape\n", tv);
                buf += rh->length();
                break;
            case 48:
                printf("%i DefineFont2\n", tv);
                buf += rh->length();
                break;
            case 56:
                printf("%i ExportAssets\n", tv);
                buf += rh->length();
                break;
            case 57:
                printf("%i ImportAssets\n", tv);
                buf += rh->length();
                break;
            case 58:
                printf("%i EnableDebugger\n", tv);
                buf += rh->length();
                break;
            case 59:
                printf("%i DoInitAction\n", tv);
                buf += rh->length();
                break;
            case 60:
                printf("%i DefineVideobuf\n", tv);
                buf += rh->length();
                break;
            case 61:
                printf("%i VideoFrame\n", tv);
                buf += rh->length();
                break;
            case 62:
                printf("%i DefineFontInfo2\n", tv);
                buf += rh->length();
                break;
            case 64:
                printf("%i EnableDebugger2\n", tv);
                buf += rh->length();
                break;
            case 65:
                printf("%i ScriptLimits\n", tv);
                buf += rh->length();
                break;
            case 66:
                printf("%i SetTabIndex\n", tv);
                buf += rh->length();
                break;
            case 69:
                //printf("%i FileAttributes\n", tv);
                //buf += rh->length();
                
                t = new FileAttributes;
                t -> recordHeader = rh;
                t -> fromSWF(buf);
                break;
            case 70:
                printf("%i PlaceObject3\n", tv);
                buf += rh->length();
                break;
            case 71:
                printf("%i ImportAssets2\n", tv);
                buf += rh->length();
                break;
            case 73:
                printf("%i DefineFontAlignZones\n", tv);
                buf += rh->length();
                break;
            case 74:
                printf("%i CSMTextSettings\n", tv);
                buf += rh->length();
                break;
            case 75:
                printf("%i DefineFont3\n", tv);
                buf += rh->length();
                break;
            case 76:
                printf("%i SymbolClass\n", tv);
                buf += rh->length();
                break;
            case 77:
                printf("%i Metadata\n", tv);
                buf += rh->length();
                break;
            case 78:
                printf("%i DefineScalingGrid\n", tv);
                buf += rh->length();
                break;
            case 82:
                printf("%i DoABC\n", tv);
                buf += rh->length();
                break;
            case 83:
                printf("%i DefineShape4\n", tv);
                buf += rh->length();
                break;
            case 84:
                printf("%i DefineMorphShape2\n", tv);
                buf += rh->length();
                break;
            case 86:
                printf("%i DefineSceneAndFrameLabelData\n", tv);
                buf += rh->length();
                break;
            case 87:
                printf("%i DefineBinaryData\n", tv);
                buf += rh->length();
                break;
            case 88:
                printf("%i DefineFontName\n", tv);
                buf += rh->length();
                break;
            case 89:
                printf("%i StartSound2\n", tv);
                buf += rh->length();
                break;
            case 90:
                printf("%i DefineBitsJPEG4\n", tv);
                buf += rh->length();
                break;
            case 91:
                printf("%i DefineFont4\n", tv);
                buf += rh->length();
                break;
                
            default:
                printf("\nEncountered unknown tag\n");
                buf += rh->length();
                break;
        }
        
        //t -> recordHeader = rh;
        //t -> fromSWF(buf);
        //t -> setVersion(<#unsigned char *version#>)
        
        vrh.push_back( *rh );
    } while ( rh->type() != 0 );
}

void swf::SWF::toSWF() {}
